// Header-only terminal UI helpers for pretty, colorized output (opt-in via PRETTY_UI)
#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <wchar.h>

#if defined(_WIN32)
  #include <io.h>
#else
  #include <unistd.h>
#endif

namespace ui {

struct Options {
    bool pretty_enabled{false};
    bool clear_between_menus{false};
    bool color_enabled{false};
    bool keep_legacy{true};
    bool utf8{true};
};

inline bool is_tty() {
#if defined(_WIN32)
    return _isatty(_fileno(stdout)) != 0;
#else
    return ::isatty(fileno(stdout)) != 0;
#endif
}

inline Options detect() {
    Options o;
    const char* p = std::getenv("PRETTY_UI");
    o.pretty_enabled = (p && *p && std::string(p) != "0");
    const char* c = std::getenv("CLEAR_MENU");
    o.clear_between_menus = (c && *c && std::string(c) != "0");
    o.color_enabled = o.pretty_enabled && is_tty();
    const char* k = std::getenv("KEEP_LEGACY");
    o.keep_legacy = !(k && *k=='0');
    // UTF-8 detection via locale envs
    auto has_utf8 = [](const char* v){ if(!v) return false; std::string s(v); for (auto &ch: s) ch=static_cast<char>(std::tolower(static_cast<unsigned char>(ch))); return s.find("utf-8")!=std::string::npos || s.find("utf8")!=std::string::npos; };
    o.utf8 = has_utf8(std::getenv("LC_ALL")) || has_utf8(std::getenv("LC_CTYPE")) || has_utf8(std::getenv("LANG"));
    return o;
}

// --- UTF-8 display width helpers ---
inline int utf8_next_cp(const std::string& s, std::size_t& i) {
    unsigned char c = static_cast<unsigned char>(s[i]);
    if ((c & 0x80u) == 0) { return s[i++]; }
    int cp = 0; int extra = 0;
    if ((c & 0xE0u) == 0xC0u) { cp = c & 0x1Fu; extra=1; }
    else if ((c & 0xF0u) == 0xE0u) { cp = c & 0x0Fu; extra=2; }
    else if ((c & 0xF8u) == 0xF0u) { cp = c & 0x07u; extra=3; }
    else { ++i; return '?'; }
    ++i;
    for (int k=0; k<extra && i<s.size(); ++k,++i) {
        unsigned char cc = static_cast<unsigned char>(s[i]);
        if ((cc & 0xC0u) != 0x80u) return '?';
        cp = (cp<<6) | (cc & 0x3Fu);
    }
    return cp;
}

// Strip ANSI color codes
inline std::string strip_colors(const std::string& s) {
    std::string res;
    res.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\x1b') {
            std::size_t pos = s.find('m', i);
            if (pos != std::string::npos) { i = pos; continue; }
        }
        res += s[i];
    }
    return res;
}

inline int display_width(const std::string& s, bool utf8=true) {
    std::string stripped = strip_colors(s);
    if (!utf8) return static_cast<int>(stripped.size());
    int w=0; std::size_t i=0; while (i<stripped.size()) {
        int cp = utf8_next_cp(stripped, i);
        int cw = wcwidth(static_cast<wchar_t>(cp));
        w += (cw > 0 ? cw : 1);
    }
    return w;
}

struct Style { std::string reset, bold, dim, ul, green, cyan, red, magenta, gray, blue, yellow; };
inline Style styles(bool enabled) {
    if (!enabled) return {};
    return {"\x1b[0m","\x1b[1m","\x1b[2m","\x1b[4m","\x1b[32m","\x1b[36m","\x1b[31m","\x1b[35m","\x1b[90m","\x1b[34m","\x1b[33m"};
}

inline void clear_screen(const Options& o) {
    if (o.pretty_enabled && o.clear_between_menus) std::cout << "\x1b[2J\x1b[H";
}

inline void sep(const Options& o, const char* ch = "═") { // box-drawing by default
    if (!o.pretty_enabled) return;
    for (int i=0;i<44;++i) std::cout << ch; std::cout << "\n";
}

inline void banner(const Options& o, const std::string& title) {
    if (!o.pretty_enabled) return;
    auto s = styles(o.color_enabled);
    sep(o, o.utf8 ? "═" : "=");
    std::cout << "  " << s.bold << title << s.reset << "\n";
    sep(o, o.utf8 ? "═" : "=");
}

// Boxed menu with Unicode borders
inline void menu_box(const Options& o, const std::string& title,
                     const std::vector<std::string>& lines) {
    if (!o.pretty_enabled) return;
    auto s = styles(o.color_enabled);

    // Determine max inner width from title and all lines, then add padding.
    int width = display_width(title, o.utf8);
    for (const auto& ln : lines) {
        width = std::max(width, display_width(ln, o.utf8));
    }
    width += 2; // 1 space padding on each side

    if (o.utf8) {
        const char* TL = "\xE2\x95\x94"; const char* TR = "\xE2\x95\x97";
        const char* BL = "\xE2\x95\x9A"; const char* BR = "\xE2\x95\x9D";
        const char* H  = "\xE2\x95\x90"; const char* V  = "\xE2\x95\x91";
        const char* LS = "\xE2\x95\xA0"; const char* RS = "\xE2\x95\xA3";
        std::string hline; hline.reserve(static_cast<std::size_t>(width)*3);
        for (int i=0;i<width;++i) hline += H;

    std::cout << TL << hline << TR << "\n";
        int tlen_vis = display_width(title, o.utf8);
        int lpad = (width > tlen_vis) ? (width - tlen_vis) / 2 : 0;
        int rpad = (width > tlen_vis) ? width - tlen_vis - lpad : 0;
        std::cout << V << std::string(lpad, ' ') << s.cyan << s.bold << title << s.reset
                  << std::string(rpad, ' ') << V << "\n";
        std::cout << LS << hline << RS << "\n";
        for (auto ln : lines) {
            if (!ln.empty() && ln.front() == '[') {
                auto pos = ln.find(']');
                if (pos != std::string::npos) {
                    ln = s.yellow + ln.substr(0, pos + 1) + s.reset + ln.substr(pos + 1);
                }
            }
            int row_vis = display_width(ln, o.utf8);
            int rpad2 = (width > row_vis + 2) ? (width - row_vis - 2) : 0;
            std::cout << V << " " << ln << std::string(rpad2, ' ') << " " << V << "\n";
        }
        std::cout << BL << hline << BR << "\n";
    } else {
        std::string hline(static_cast<std::size_t>(width), '-');
        std::cout << "+" << hline << "+\n";
        int tlen_vis = display_width(title, false);
        int lpad = (width > tlen_vis) ? (width - tlen_vis) / 2 : 0;
        int rpad = (width > tlen_vis) ? width - tlen_vis - lpad : 0;
        std::cout << "|" << std::string(lpad, ' ') << title << std::string(rpad, ' ') << "|\n";
        std::cout << "+" << hline << "+\n";
        for (const auto& ln : lines) {
            int row_vis = display_width(ln, false);
            int rpad2 = (width > row_vis + 2) ? (width - row_vis - 2) : 0;
            std::cout << "| " << ln << std::string(rpad2, ' ') << " |\n";
        }
        std::cout << "+" << hline << "+\n";
    }
}

// Pretty list state: [HEAD] [0] value ⇄ [1] value [TAIL]
inline void render_state(const Options& o, const std::vector<std::string>& items) {
    if (!o.pretty_enabled) return;
    auto s = styles(o.color_enabled);
    std::cout << s.bold << "List state:" << s.reset << "\n  ";
    if (items.empty()) { std::cout << s.gray << "(empty)" << s.reset << "\n"; return; }
    for (std::size_t i=0;i<items.size();++i) {
        bool head = (i==0), tail = (i+1==items.size());
        if (head) std::cout << (o.utf8? s.blue + "[HEAD] ": s.blue + "<HEAD> ") << s.reset;
        std::cout << s.magenta << "[" << i << "]" << s.reset << " "
                  << s.cyan << items[i] << s.reset;
        if (!tail) std::cout << " " << s.gray << (o.utf8? "⇄" : "<->") << s.reset << " ";
        if (tail) std::cout << " " << (o.utf8? s.blue + "[TAIL]": s.blue + "<TAIL>") << s.reset;
        if (i+1<items.size()) std::cout << " ";
    }
    std::cout << "\n";
}

inline void print_forward(const Options& o, const std::vector<std::string>& items) {
    // Legacy line for tests
    if (o.keep_legacy) {
        for (std::size_t i=0;i<items.size();++i) {
            if (i) std::cout << " <-> ";
            std::cout << items[i];
        }
        std::cout << "\n";
    }
    render_state(o, items);
}

inline void print_backward(const Options& o, const std::vector<std::string>& items) {
    if (o.keep_legacy) {
        for (std::size_t i=0;i<items.size();++i) {
            if (i) std::cout << " <-> ";
            std::cout << items[i];
        }
        std::cout << "\n";
    }
    render_state(o, items);
}

inline void print_size(const Options& o, std::size_t n) {
    if (o.keep_legacy) std::cout << n << "\n";
    if (o.pretty_enabled) {
        auto s = styles(o.color_enabled);
        std::cout << s.cyan << "Size: " << n << s.reset << "\n";
    }
}

inline void render_list(const Options& o, const std::vector<std::string>& items) {
    if (!o.pretty_enabled) return;
    auto s = styles(o.color_enabled);
    if (items.empty()) { std::cout << s.gray << "(empty)" << s.reset << "\n"; return; }
    for (std::size_t i=0;i<items.size();++i) {
        bool head = (i==0), tail = (i+1==items.size());
        std::cout << (head ? s.magenta : "") << "[" << i << "]" << s.reset << " ";
        std::cout << "[" << (head? s.ul:std::string()) << items[i] << s.reset << "]";
        if (!tail) std::cout << " " << s.magenta << "⇄" << s.reset << " ";
        if (tail) {
            std::cout << "   " << s.gray << (head? "[HEAD|TAIL]":"[TAIL]") << s.reset;
            if (!head) std::cout << " " << s.gray << "[HEAD @ 0]" << s.reset;
        }
    }
    std::cout << "\n";
}

inline void ok(const Options& o, const std::string& msg="ok") {
    auto s = styles(o.color_enabled);
    if (o.pretty_enabled) std::cout << s.green << "✔ " << msg << s.reset << "\n"; else std::cout << msg << "\n";
}
inline void fail(const Options& o, const std::string& msg="fail") {
    auto s = styles(o.color_enabled);
    if (o.pretty_enabled) std::cerr << s.red << "✘ " << msg << s.reset << "\n"; else std::cerr << msg << "\n";
}

inline void render_stats(const Options& o, std::size_t nodes,
                         const std::string& head, const std::string& tail) {
    if (!o.pretty_enabled) return;
    auto s = styles(o.color_enabled);
    std::cout << s.gray << "Stats: Nodes=" << nodes
              << ", Head=" << (head.empty()?"∅":head)
              << ", Tail=" << (tail.empty()?"∅":tail)
              << s.reset << "\n";
}

// Optional pause between sections for human-friendly flow (env: PAUSE_AFTER_SECTIONS=1)
inline void pause_if_enabled(const Options& o, const char* msg = "Press Enter to continue...") {
    const char* p = std::getenv("PAUSE_AFTER_SECTIONS");
    if (!p || *p=='0') return;
    auto s = styles(o.color_enabled);
    std::cout << (o.pretty_enabled ? s.dim : "") << msg << s.reset;
    std::cout.flush();
    std::string _; std::getline(std::cin, _);
}

// Optional paginator for long help (env: PAGINATE_HELP=1)
inline void paginate_lines(const Options& o, const std::vector<std::string>& lines, std::size_t page=20) {
    const char* p = std::getenv("PAGINATE_HELP");
    if (!(p && *p!='0')) { for (const auto& s: lines) std::cout << s << "\n"; return; }
    std::size_t count = 0;
    for (const auto& s: lines) {
        std::cout << s << "\n";
        if (++count % page == 0) pause_if_enabled(o, "More — press Enter...");
    }
}

// Validated numeric menu prompt
inline int prompt_menu_choice(const Options& o, int lo, int hi) {
    auto st = styles(o.color_enabled);
    for (;;) {
        // Brief prompt
        std::cout << (o.pretty_enabled ? st.green + std::string("➤ Select an option [") + std::to_string(lo) + "–" + std::to_string(hi) + "]: " + st.reset : std::string("Select (") + std::to_string(lo) + "-" + std::to_string(hi) + "): ");
        std::cout.flush();
        std::string s; if (!std::getline(std::cin, s)) return hi; // default to Quit
        std::istringstream iss(s); int v=0; if (iss >> v && v>=lo && v<=hi) return v;
        if (o.pretty_enabled) std::cout << st.red << "✘ Invalid choice." << st.reset << "\n"; else std::cout << "Invalid choice.\n";
    }
}

} // namespace ui
