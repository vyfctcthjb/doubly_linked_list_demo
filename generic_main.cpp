// Generic demo with a simple, colorful menu so the user can pick what to run.
// Options:
//   1) Integer demo (push_back + insertBefore)
//   2) String demo (push_front + insertAfter)
//   3) Interactive REPL (strings): push_front/back, insert_before/after, remove_at, detach_at, clear, print
// The goal is educational: everything prints and is easy to follow.
//
// Notes about colors:
// - We use ANSI escape codes to colorize the menu and prompts. Most terminals (macOS Terminal, iTerm, Linux) support them.
// - If your terminal does not support colors, you'll simply see normal text.

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <cstdlib>
#include "include/doubly_linked_list.hpp"
#include "include/ui.hpp"

using edu::DoublyLinkedList;
using edu::Node;

// ANSI color helpers
static constexpr const char* C_RESET  = "\033[0m";
static constexpr const char* C_BOLD   = "\033[1m";
static constexpr const char* C_CYAN   = "\033[36m";
static constexpr const char* C_GREEN  = "\033[32m";
static constexpr const char* C_YELLOW = "\033[33m";
// static constexpr const char* C_MAGENTA= "\033[35m"; // unused
static constexpr const char* C_BLUE   = "\033[34m";
static constexpr const char* C_RED    = "\033[31m";

static bool g_debug_on = false; // persistent debug flag for REPL

static bool teach_mode_enabled() {
    const char* p = std::getenv("TEACH_MODE");
    if (!p) return false; std::string v(p); return v=="1" || v=="true" || v=="on";
}

template<typename T>
static std::string to_str(const T& v) { std::ostringstream ss; ss << v; return ss.str(); }
static std::string to_str(const std::string& v) { return v; }

template<typename T>
static std::vector<std::string> collect_forward_vec(const DoublyLinkedList<T>& L) {
    std::vector<std::string> v; for (auto* p=L.head; p; p=p->next) v.push_back(to_str<T>(p->value)); return v;
}
template<typename T>
static std::vector<std::string> collect_backward_vec(const DoublyLinkedList<T>& L) {
    std::vector<std::string> v; for (auto* p=L.tail; p; p=p->prev) v.push_back(to_str<T>(p->value)); return v;
}
static std::string join_with(const std::vector<std::string>& v, const std::string& sep) {
    std::ostringstream oss; for (std::size_t i=0;i<v.size();++i){ if(i) oss<<sep; oss<<v[i]; } return oss.str();
}
static bool pretty_enabled() {
    const char* p = std::getenv("PRETTY_UI");
    if (!p) return false;
    std::string v(p);
    return v == "1" || v == "true" || v == "on";
}

static bool clear_menu_enabled() {
    const char* p = std::getenv("CLEAR_MENU");
    if (!p) return false;
    std::string v(p);
    return v == "1" || v == "true" || v == "on";
}

template<typename T>
static void print_list_pretty(const DoublyLinkedList<T>& L) {
    ui::Options o = ui::detect();
    std::vector<std::string> vals; vals.reserve(32);
    for (Node<T>* p=L.head; p; p=p->next) { std::ostringstream ss; ss << p->value; vals.push_back(ss.str()); }
    std::cout << "Current list: ";
    ui::render_list(o, vals);
}

// Debug helpers for teaching: dump node addresses and links
template<typename T>
static void dump_links(const DoublyLinkedList<T>& L) {
    // Aligned, readable dump; always available when debug is on
    ui::Options o = ui::detect();
    auto s = ui::styles(o.color_enabled);
    std::cout << (o.pretty_enabled ? s.bold + std::string("Dump:") + s.reset : std::string("Dump:")) << "\n";
    int idx = 0;
    for (Node<T>* p = L.head; p; p = p->next, ++idx) {
        std::ostringstream line;
        line << "  Node[" << idx << "] " << p->value;
        std::ostringstream prev; prev << (const void*)p->prev;
        std::ostringstream self; self << (const void*)p;
        std::ostringstream next; next << (const void*)p->next;
        line << std::string( (line.str().size() < 24) ? (24 - line.str().size()) : 1, ' ');
        line << "prev=" << prev.str()
             << "  self=" << self.str()
             << "  next=" << next.str();
        std::cout << line.str() << "\n";
    }
}

// --- Helper: print menu and get choice ---
static int prompt_menu() {
    if (clear_menu_enabled()) { std::system("clear >/dev/null 2>&1"); }
    ui::Options o = ui::detect();
    if (pretty_enabled()) {
        std::vector<std::string> lines = {
            "[1] Integer demo   (1,2,3)",
            "[2] String demo    (A,B,C)",
            "[3] Interactive REPL (strings)",
            "[4] Quit"
        };
        ui::menu_box(o, "Doubly Linked List — Main Menu", lines);
    } else {
        std::cout << "\n" << C_CYAN << C_BOLD
                  << "==============================\n"
                  << " Doubly Linked List — Demo\n"
                  << "==============================" << C_RESET << "\n";
        std::cout << C_YELLOW << " 1 " << C_RESET << " Integer demo (1,2,3)\n"
                  << C_YELLOW << " 2 " << C_RESET << " String demo (A,B,C)\n"
                  << C_YELLOW << " 3 " << C_RESET << " Interactive REPL (strings)\n"
                  << C_YELLOW << " 4 " << C_RESET << " Quit\n";
    }
    return ui::prompt_menu_choice(o, 1, 4);
}

static void run_int_demo();
static void run_string_demo();
static void run_repl();

int main() {
    for (;;) {
        switch (prompt_menu()) {
            case 1: run_int_demo(); break;
            case 2: run_string_demo(); break;
            case 3: run_repl(); break;
            default: return 0;
        }
    }
}

// --- Demo 1: integers ---
static void run_int_demo() {
    ui::Options o = ui::detect();
    ui::banner(o, "=== Integer Demo ===");

    DoublyLinkedList<int> li;
    li.push_back(1);
    li.push_back(3);
    Node<int>* two = new Node<int>(2);
    li.insertBefore(li.tail, two); // [1] <-> [2] <-> [3]

    auto f = collect_forward_vec(li);
    auto r = collect_backward_vec(li);

    // Helpers handle legacy + pretty
    // DEPRECATED: ui::print_forward(o, f);
    // DEPRECATED: ui::print_backward(o, r);

    if (o.pretty_enabled) {
        std::cout << "Current list: ";
        ui::render_list(o, f);
        ui::render_stats(o, f.size(), f.empty()?"":f.front(), f.empty()?"":f.back());
    }
    ui::pause_if_enabled(o);
}

// --- Demo 2: strings ---
static void run_string_demo() {
    ui::Options o = ui::detect();
    ui::banner(o, "=== String Demo ===");

    DoublyLinkedList<std::string> ls;
    ls.push_front("C");
    ls.push_front("A");
    Node<std::string>* b = new Node<std::string>("B");
    ls.insertAfter(ls.head, b); // [A] <-> [B] <-> [C]

    auto f = collect_forward_vec(ls);
    auto r = collect_backward_vec(ls);

    // DEPRECATED: ui::print_forward(o, f);
    // DEPRECATED: ui::print_backward(o, r);

    if (o.pretty_enabled) {
        std::cout << "Current list: ";
        ui::render_list(o, f);
        ui::render_stats(o, f.size(), f.empty()?"":f.front(), f.empty()?"":f.back());
    }
    ui::pause_if_enabled(o);
}

// --- REPL (strings) ---
static void run_repl() {
    ui::banner(ui::detect(), "=== Interactive REPL ===");
    DoublyLinkedList<std::string> ls;          // empty list for the session
    auto node_at = [](DoublyLinkedList<std::string>& L, std::size_t idx) -> Node<std::string>* { std::size_t i=0; for (Node<std::string>* p=L.head; p; p=p->next, ++i) { if (i==idx) return p; } return nullptr; };
    std::cout << "\n" << C_BLUE << C_BOLD << "Interactive REPL (strings)" << C_RESET
              << " — type 'help' for commands, 'menu' to return to main menu.\n";
    std::string line;
    while (std::cout << C_GREEN << "> " << C_RESET && std::getline(std::cin, line)) {
        if (line.empty()) continue; std::istringstream iss(line); std::string cmd; iss >> cmd;
        if (cmd == "help" || cmd == "commands") {
            bool pretty = pretty_enabled();
            auto o = ui::detect();
            if (pretty) ui::banner(o, "Commands");
            std::vector<std::string> lines = {
              "Insert:",
              "  push_front VALUE",
              "  push_back VALUE",
              "  insert_after INDEX VALUE",
              "  insert_before INDEX VALUE",
              "",
              "Remove:",
              "  remove_at INDEX",
              "  detach_at INDEX",
              "  clear",
              "",
              "Inspect:",
              "  find VALUE",
              "  print",
              "  print_rev",
              "  size",
              "  dump    (pointers)",
              "",
              "System:",
              "  debug on|off",
              "  cls     (clear screen)",
              "  menu    (return to main menu)",
              "  quit    (also returns to menu)",
              "",
              "Aliases: pf=push_front, pb=push_back, rm=remove_at, q=quit, m=menu",
              "Note: 'dump' requires 'debug on'"
            };
            ui::paginate_lines(o, lines, 18);
            ui::pause_if_enabled(o);
        } else if (cmd == "push_front") {
            // Require a non-empty VALUE; do not create blank nodes
            std::string value; if (!std::getline(iss >> std::ws, value) || value.empty()) { std::cout << "fail: missing VALUE\n"; continue; }
            ls.push_front(value); std::cout << "ok\n"; if (pretty_enabled()) { std::cout << C_GREEN << "✔ ok" << C_RESET << "\n"; print_list_pretty(ls); }
            if (pretty_enabled()) { ui::render_stats(ui::detect(), ls.size(), ls.head?ls.head->value:std::string(), ls.tail?ls.tail->value:std::string()); }
        }
        else if (cmd == "push_back") {
            // Require a non-empty VALUE; do not create blank nodes
            std::string value; if (!std::getline(iss >> std::ws, value) || value.empty()) { std::cout << "fail: missing VALUE\n"; continue; }
            ls.push_back(value); std::cout << "ok\n"; if (pretty_enabled()) { std::cout << C_GREEN << "✔ ok" << C_RESET << "\n"; print_list_pretty(ls); }
            if (pretty_enabled()) { ui::render_stats(ui::detect(), ls.size(), ls.head?ls.head->value:std::string(), ls.tail?ls.tail->value:std::string()); }
        }
        else if (cmd == "insert_after") {
            // Supports two forms:
            //   insert_after INDEX VALUE  → insert after node at index
            //   insert_after VALUE       → insert after tail
            std::vector<std::string> before = collect_forward_vec(ls);
            std::string first;
            if (!(iss >> first)) { std::cout << "fail: usage: insert_after INDEX VALUE | insert_after VALUE\n"; continue; }
            std::string value;
            Node<std::string>* cur = nullptr;
            bool is_index = !first.empty() && std::all_of(first.begin(), first.end(), [](unsigned char ch){ return std::isdigit(ch); });
            if (is_index) {
                std::size_t idx = 0; try { idx = static_cast<std::size_t>(std::stoull(first)); } catch (...) { std::cout << "fail: bad index\n"; continue; }
                if (!std::getline(iss >> std::ws, value) || value.empty()) { std::cout << "fail: missing VALUE\n"; continue; }
                cur = node_at(ls, idx);
            } else {
                value = first;
                cur = ls.tail; // after last element (or nullptr if empty; header will handle empty case)
            }
            if (value.empty()) { std::cout << "fail: empty VALUE\n"; continue; }
            Node<std::string>* nn = new Node<std::string>(value);
            bool ok = ls.insertAfter(cur, nn);
            if (!ok) { delete nn; }
            std::cout << (ok?"ok":"fail") << "\n"; if (pretty_enabled()) { std::cout << (ok?C_GREEN:C_RED) << (ok?"✔ ok":"✘ fail") << C_RESET << "\n"; if (ok) print_list_pretty(ls); }
            if (ok && teach_mode_enabled()) {
                std::cout << "Before:  " << join_with(before, " ⇄ ") << "\n";
                if (is_index) std::cout << "Insert:  " << value << " after index " << (cur?"":"∅") << (cur?"":"") << "\n"; else std::cout << "Insert:  " << value << " after tail\n";
                std::cout << "After:   " << join_with(collect_forward_vec(ls), " ⇄ ") << "\n";
            }
            if (ok && pretty_enabled()) { ui::render_stats(ui::detect(), ls.size(), ls.head?ls.head->value:std::string(), ls.tail?ls.tail->value:std::string()); }
        }
        else if (cmd == "insert_before") {
            // Supports two forms:
            //   insert_before INDEX VALUE → insert before node at index
            //   insert_before VALUE      → insert before head
            std::vector<std::string> before = collect_forward_vec(ls);
            std::string first;
            if (!(iss >> first)) { std::cout << "fail: usage: insert_before INDEX VALUE | insert_before VALUE\n"; continue; }
            std::string value;
            Node<std::string>* cur = nullptr;
            bool is_index = !first.empty() && std::all_of(first.begin(), first.end(), [](unsigned char ch){ return std::isdigit(ch); });
            if (is_index) {
                std::size_t idx = 0; try { idx = static_cast<std::size_t>(std::stoull(first)); } catch (...) { std::cout << "fail: bad index\n"; continue; }
                if (!std::getline(iss >> std::ws, value) || value.empty()) { std::cout << "fail: missing VALUE\n"; continue; }
                cur = node_at(ls, idx);
            } else {
                value = first;
                cur = ls.head; // before head (or nullptr if empty; header will handle empty case)
            }
            if (value.empty()) { std::cout << "fail: empty VALUE\n"; continue; }
            Node<std::string>* nn = new Node<std::string>(value);
            bool ok = ls.insertBefore(cur, nn);
            if (!ok) { delete nn; }
            std::cout << (ok?"ok":"fail") << "\n"; if (pretty_enabled()) { std::cout << (ok?C_GREEN:C_RED) << (ok?"✔ ok":"✘ fail") << C_RESET << "\n"; if (ok) print_list_pretty(ls); }
            if (ok && teach_mode_enabled()) {
                std::cout << "Before:  " << join_with(before, " ⇄ ") << "\n";
                if (is_index) std::cout << "Insert:  " << value << " before index\n"; else std::cout << "Insert:  " << value << " before head\n";
                std::cout << "After:   " << join_with(collect_forward_vec(ls), " ⇄ ") << "\n";
            }
            if (ok && pretty_enabled()) { ui::render_stats(ui::detect(), ls.size(), ls.head?ls.head->value:std::string(), ls.tail?ls.tail->value:std::string()); }
        }
        else if (cmd == "remove_at") { std::size_t idx; iss >> idx; Node<std::string>* cur = node_at(ls, idx); bool ok = cur ? ls.remove(cur) : false; std::cout << (ok?"ok":"fail") << "\n"; if (pretty_enabled()) { std::cout << (ok?C_GREEN:C_RED) << (ok?"✔ ok":"✘ fail") << C_RESET << "\n"; if (ok) print_list_pretty(ls); ui::render_stats(ui::detect(), ls.size(), ls.head?ls.head->value:std::string(), ls.tail?ls.tail->value:std::string()); } }
        else if (cmd == "detach_at") { std::size_t idx; iss >> idx; Node<std::string>* cur = node_at(ls, idx); bool ok = cur ? ls.detach(cur) : false; std::cout << (ok?"ok":"fail") << "\n"; if (pretty_enabled()) { std::cout << (ok?C_GREEN:C_RED) << (ok?"✔ ok":"✘ fail") << C_RESET << "\n"; if (ok) print_list_pretty(ls); ui::render_stats(ui::detect(), ls.size(), ls.head?ls.head->value:std::string(), ls.tail?ls.tail->value:std::string()); } }
        else if (cmd == "clear") { ls.clear(); std::cout << "ok\n"; if (pretty_enabled()) { std::cout << C_GREEN << "✔ ok" << C_RESET << "\n"; print_list_pretty(ls); ui::render_stats(ui::detect(), ls.size(), std::string(), std::string()); } }
        else if (cmd == "find") {
            std::string value; if (!std::getline(iss >> std::ws, value) || value.empty()) { std::cout << "fail: missing VALUE\n"; }
            else {
                // search and report first index
                std::size_t i = 0; Node<std::string>* p = ls.head; Node<std::string>* hit = nullptr;
                while (p) { if (p->value == value) { hit = p; break; } p = p->next; ++i; }
                if (hit) std::cout << "found at " << i << "\n"; else std::cout << "not found\n";
            }
        }
        else if (cmd == "print") { 
            ui::Options o = ui::detect();
            std::vector<std::string> v; for (auto* p=ls.head;p;p=p->next) v.push_back(p->value);
            if (o.keep_legacy) { for (auto* p=ls.head;p;p=p->next) { std::cout << p->value; if (p->next) std::cout << " <-> "; } std::cout << "\n"; }
            if (o.pretty_enabled) ui::render_list(o, v);
        }
        else if (cmd == "print_rev") { 
            ui::Options o = ui::detect();
            std::vector<std::string> v; for (auto* p=ls.tail;p;p=p->prev) v.push_back(p->value);
            if (o.keep_legacy) { for (auto* p=ls.tail;p;p=p->prev) { std::cout << p->value; if (p->prev) std::cout << " <-> "; } std::cout << "\n"; }
            if (o.pretty_enabled) ui::render_list(o, v);
        }
        else if (cmd == "size") { ui::print_size(ui::detect(), ls.size()); }
        else if (cmd == "status") {
            auto o = ui::detect();
            auto yes = [](bool b){ return b?"on":"off"; };
            std::cout
              << "Size: " << ls.size() << "\n"
              << "Head: " << (ls.head?ls.head->value:std::string("∅")) << "\n"
              << "Tail: " << (ls.tail?ls.tail->value:std::string("∅")) << "\n"
              << "PRETTY_UI: " << yes(o.pretty_enabled) << "\n"
              << "KEEP_LEGACY: " << yes(o.keep_legacy) << "\n"
              << "PAUSE_AFTER: " << (std::getenv("PAUSE_AFTER_SECTIONS")?"on":"off") << "\n"
              << "CLEAR_MENU: " << yes(o.clear_between_menus) << "\n"
              << "UTF8: " << yes(o.utf8) << "\n"
              << "debug: " << yes(g_debug_on) << "\n";
        }
        else if (cmd == "pf") { std::string value; if (!std::getline(iss >> std::ws, value) || value.empty()) { std::cout << "fail: missing VALUE\n"; } else { ls.push_front(value); std::cout << "ok\n"; if (pretty_enabled()) { std::cout << C_GREEN << "✔ ok" << C_RESET << "\n"; print_list_pretty(ls); } } }
        else if (cmd == "pb") { std::string value; if (!std::getline(iss >> std::ws, value) || value.empty()) { std::cout << "fail: missing VALUE\n"; } else { ls.push_back(value); std::cout << "ok\n"; if (pretty_enabled()) { std::cout << C_GREEN << "✔ ok" << C_RESET << "\n"; print_list_pretty(ls); } } }
        else if (cmd == "rm") { std::size_t idx; iss >> idx; Node<std::string>* cur = node_at(ls, idx); bool ok = cur ? ls.remove(cur) : false; std::cout << (ok?"ok":"fail") << "\n"; if (pretty_enabled()) { std::cout << (ok?C_GREEN:C_RED) << (ok?"✔ ok":"✘ fail") << C_RESET << "\n"; if (ok) print_list_pretty(ls); } }
        else if (cmd == "m") { break; }
        else if (cmd == "q") { break; }
        else if (cmd == "debug") { std::string onoff; if (!(iss >> onoff)) { g_debug_on = true; std::cout << "ok\n"; }
            else if (onoff == "on") { g_debug_on = true; std::cout << "ok\n"; }
            else if (onoff == "off") { g_debug_on = false; std::cout << "ok\n"; }
            else { std::cout << "fail: usage debug on|off\n"; } }
        else if (cmd == "dump") {
            auto o = ui::detect();
            if (g_debug_on) { dump_links(ls); ui::pause_if_enabled(o); }
            else { std::cout << "fail\n"; ui::fail(o, "debug is off (use 'debug on')"); }
        }
        else if (cmd == "cls") { std::system("clear >/dev/null 2>&1"); }
        else if (cmd == "menu" || cmd == "quit") { break; }
        else { std::cout << "fail\n"; ui::fail(ui::detect(), "unknown command (type 'help')"); }
    }
}
