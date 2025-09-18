<p align="center">
  <img src="https://img.shields.io/badge/D-red?style=for-the-badge" alt="D">
  <img src="https://img.shields.io/badge/U-black?style=for-the-badge" alt="U">
  <img src="https://img.shields.io/badge/B-red?style=for-the-badge" alt="B">
  <img src="https://img.shields.io/badge/L-black?style=for-the-badge" alt="L">
  <img src="https://img.shields.io/badge/I-red?style=for-the-badge" alt="I">
  <img src="https://img.shields.io/badge/N-black?style=for-the-badge" alt="N">
  <img src="https://img.shields.io/badge/X-red?style=for-the-badge" alt="X">
</p>

<pre>
  <p align="center">
╔════════════════════════════════════════════════════════════════════════════════════╗
║                                                                                    ║
║                 ██████╗ ██╗   ██╗██████╗ ██╗     ██╗███╗   ██╗██╗  ██╗             ║
║                 ██╔══██╗██║   ██║██╔══██╗██║     ██║████╗  ██║╚██╗██╔╝             ║
║                 ██║  ██║██║   ██║██████╔╝██║     ██║██╔██╗ ██║ ╚███╔╝              ║
║                 ██║  ██║██║   ██║██╔══██╗██║     ██║██║╚██╗██║ ██╔██╗              ║
║                 ██████╔╝╚██████╔╝██████╔╝███████╗██║██║ ╚████║██╔╝ ██╗             ║
║                 ╚═════╝  ╚═════╝ ╚═════╝ ╚══════╝╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝             ║
║                                                                                    ║
║                     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓                         ║
║                     ▓ Doubly Linked List Implementation  ▓                         ║
║                     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓                         ║
║                                 Justin Guida                                       ║
╚════════════════════════════════════════════════════════════════════════════════════╝
    <p align="center">
</pre>

<p align="center">
  <img alt="C++" src="https://img.shields.io/badge/C++-blue?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img alt="Platform" src="https://img.shields.io/badge/Mac%20%26%20Linux-black?style=for-the-badge&logo=linux&logoColor=white">
  <img alt="Editor" src="https://img.shields.io/badge/Clion-green?style=for-the-badge&logo=clion&logoColor=white">
  <img alt="Build" src="https://img.shields.io/badge/Makefile-orange?style=for-the-badge&logo=gnu&logoColor=white">
</p>


# Doubly Linked List Demo

A C++ demo project that implements a generic doubly linked list with a simple terminal UI.
Features:
- Insert, remove, and traverse nodes
- Colorized output and menu navigation
- Debug checks for safe operations

An educational walkthrough of doubly-linked list operations with:
- Clean C++ implementations (insertAfter, insertBefore, remove, detach, clear)
- Flowcharts (Mermaid + ASCII) of control flow and pointer changes
- A generic templated list + interactive REPL demo
- Polished scripts that work after clone or zip

`main.cpp` has easy-to-read code and comments so you can understand how the basic operations work.


## Table of contents
- [Quick Start](#quick-start-works-for-everyone)
- [Demos](#demos)
- [UI Customization](#ui-customization-optional)
- [Program Flow](#program-flow-high-level)
- [API](#api-core--generic)
- [Build](#build)
- [Interactive REPL](#interactive-repl-generic-demo)
- [Repository](#repository)
- [Troubleshooting](#troubleshooting-macos)

---
## Interactive REPL/Commands
<img width="609" height="645" alt="Screenshot 2025-09-18 at 1 04 39 AM" src="https://github.com/user-attachments/assets/a91b5a20-f9f2-4184-af8c-1949409c56e1" />

## Demo of Commands

<img width="650" height="444" alt="Screenshot 2025-09-18 at 1 06 12 AM" src="https://github.com/user-attachments/assets/ee70b1f6-3561-4ad0-80d2-8b943b689922" />


## Simple Demo Integers
<img width="442" height="211" alt="Screenshot 2025-09-18 at 1 08 14 AM" src="https://github.com/user-attachments/assets/24186558-d06c-4483-ba96-2878a40ce318" />

## Simple Demo Strings
<img width="439" height="212" alt="Screenshot 2025-09-18 at 1 09 51 AM" src="https://github.com/user-attachments/assets/8b54a961-1f47-4489-b3b7-b101045a4804" />

## Quick Start (works for everyone)


**Recommended (git clone)**
```bash
git clone https://github.com/jguida941/doubly_linked_list_demo.git
cd doubly_linked_list_demo
bash scripts/bootstrap_mac.command     # macOS one-time (exec + quarantine + noexec check)
make run                               # educational demo
make run-generic                       # interactive REPL (templated list)
```

**ZIP users on macOS**
```bash
# Right-click the unzipped folder → "New Terminal at Folder"
bash scripts/bootstrap_mac.command
make run
```

**Prefer a double-click app?** After running the bootstrap once, you can also use:
- macOS: double-click `macos/Run Demo.app` (educational)
- macOS: double-click `macos/Run Interactive Demo.app` (interactive REPL)

If Finder says “could not be executed due to access privileges”, your folder is likely on a noexec volume (exFAT/NTFS/SMB). Move it to your home drive (APFS), e.g. `~/Dev/doubly_linked_list_demo`, re-run `bootstrap_mac.command`.

**Windows**: double-click `scripts/Run_Interactive_Demo_Tests.bat` or run the `.ps1` in PowerShell.

---

## Demos

```bash
make run          # Educational demo: prints Forward/Backward once
make run-generic  # Interactive REPL (templated list)
```

### UI Customization (Optional)

The interactive demo (`generic_demo`) can be customized using environment variables:

-   `PRETTY_UI=1`: Enables a modern, colorful UI with borders and banners.
-   `CLEAR_MENU=1`: Clears the screen between main menu displays.
-   `PAUSE_AFTER_SECTIONS=1`: Pauses the program after completing a demo, waiting for the user to press Enter.
-   `PAGINATE_HELP=1`: Paginates the `help` command output in the REPL.
-   `KEEP_LEGACY=0`: Disables the original, plainer output format in the REPL.
-   `TEACH_MODE=1`: Enables extra explanatory output for some REPL commands.

**Example:**
```bash
PRETTY_UI=1 make run-generic
```

**Expected output (educational):**

```
Forward:   A ⇄ C ⇄ B
Backward:  B ⇄ C ⇄ A
```

---

## Program Flow (high-level)

```mermaid
flowchart TD
    Start([Start]) --> NewList["Construct List"]
    NewList --> A["insertAfter(nullptr, A)"]
    A --> B["insertAfter(A, B)"]
    B --> C["insertAfter(A, C)"]
    C --> PrintF["printForward → A ⇄ C ⇄ B"]
    C --> PrintB["printBackward → B ⇄ C ⇄ A"]
```
**ASCII Fallback:**
```
Start -> Construct List -> insertAfter(nullptr, A) -> insertAfter(A, B) -> insertAfter(A, C)
  -> printForward → A ⇄ C ⇄ B
  -> printBackward → B ⇄ C ⇄ A
```

### insertAfter Decision Tree

```mermaid
flowchart TD
    subgraph "insertAfter(cur, new)"
        E{"head == ∅?"} -->|Yes| Empty["head=tail=new"]
        E -->|No| T{"cur == tail?"}
        T -->|Yes| Append["Append to end, new becomes tail"]
        T -->|No| Mid["Insert in middle"]
    end
```
**ASCII Fallback:**
```
insertAfter(cur, new)
  |
  +-- if head is null?
  |   |
  |   +-- Yes: head=tail=new
  |
  +-- No: if cur is tail?
      |
      +-- Yes: Append to end, new becomes tail
      |
      +-- No: Insert in middle
```

---

## API (core & generic)
- **Core demo** (string nodes; `main.cpp`)
  - `bool insertAfter(Node* cur, Node* n)`
  - `bool remove(Node* n)` - deletes
  - `bool detach(Node* n)` - unlinks only
  - `void clear()` - destructor uses this
  - `printForward()`, `printBackward()`, `size()`, `empty()`
  - Optional `DLLIST_DEBUG_CHECKS` for invariants/membership checks
- **Generic** (template; `include/doubly_linked_list.hpp`)
  - `DoublyLinkedList<T>` with `Node<T>`
  - Adds `insertBefore`, `push_front`, `push_back`
  - Same removal/detach/clear/traversals

---

## Build

**Make (macOS/Linux):**
```bash
make all         # builds educational + generic demos
make run         # educational demo
make run-generic # templated demo + REPL
make test        # smoke tests
make clean
```

**CMake (optional):**
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./doubly_linked_list_demo
./generic_demo
```

---

## Interactive REPL (generic demo)

**Commands:**
- `help` (or `commands`): Displays the help menu.
- `push_front VALUE` (alias: `pf`): Adds a value to the front of the list.
- `push_back VALUE` (alias: `pb`): Adds a value to the end of the list.
- `insert_after INDEX VALUE`: Inserts a value after the specified index.
- `insert_before INDEX VALUE`: Inserts a value before the specified index.
- `remove_at INDEX` (alias: `rm`): Removes the node at the specified index.
- `detach_at INDEX`: Detaches the node at the specified index without deleting it.
- `clear`: Removes all nodes from the list.
- `find VALUE`: Searches for a value in the list and returns the index.
- `print`: Prints the list from head to tail.
- `print_rev`: Prints the list from tail to head.
- `size`: Displays the number of nodes in the list.
- `status`: Displays the current status of the list and UI settings.
- `debug on|off`: Enables or disables debug mode.
- `dump`: Dumps the memory addresses of the nodes (requires debug mode to be on).
- `cls`: Clears the screen.
- `menu` (alias: `m`): Returns to the main menu.
- `quit` (alias: `q`): Exits the application.

**Example:**
```
> print
A ⇄ B ⇄ C
> insert_before 0 Y
ok
> print
Y ⇄ A ⇄ B ⇄ C
```

---

## Repository

```
.
├─ main.cpp
├─ generic_main.cpp
├─ include/
│  └─ doubly_linked_list.hpp
├─ docs/
│  ├─ FLOWCHART_MAIN.md
│  └─ LIST_OPERATIONS.md
├─ scripts/
│  ├─ bootstrap_mac.command
│  ├─ Run_Educational_Demo.command
│  ├─ Run_Interactive_Demo_Tests.command
│  ├─ Run_Interactive_Demo_Tests.ps1
│  ├─ Run_Interactive_Demo_Tests.bat
│  ├─ Setup_Permissions.command
│  └─ Setup-Permissions.ps1
├─ tests/
│  ├─ smoke.sh
│  ├─ test_generic_repl.sh
│  └─ test_dll_demo.sh
├─ macos/
│  ├─ Run Demo.app                # double-click educational demo
│  └─ Run Interactive Demo.app    # double-click interactive REPL (PRETTY_UI)
├─ Makefile
├─ CMakeLists.txt
└─ README.md
```

---

## Troubleshooting (macOS)
- **Quarantine/Gatekeeper**: run `scripts/bootstrap_mac.command`, or right-click a `.command` → Open.
- **Noexec volume**: move repo under `~/Dev/...` (APFS) then run `bootstrap_mac.command`.
- **Finder opens `.command` in editor**: Get Info → “Open With: Terminal” → “Change All”.
