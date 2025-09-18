# Doubly Linked List: Flowchart Deep Dive
An extended, educational walkthrough of `main.cpp` using Mermaid for logic and compact ASCII for node states.

(Companion doc for `main.cpp` in this repo. Start with `README.md` for the overview.)_

## Program Flow

```mermaid
flowchart TD
    Start([Start]) --> Construct[Construct DoublyLinkedList]
    Construct --> Create[Create Nodes A, B, C]
    Create --> InsertA[insertAfter(nullptr, A)]
    InsertA --> InsertB[insertAfter(A, B)]
    InsertB --> InsertC[insertAfter(A, C)]
    InsertC --> PrintF[printForward → A <-> C <-> B]
    InsertC --> PrintB[printBackward → B <-> C <-> A]
    PrintF --> End([End])
    PrintB --> End
```

## insertAfter Decision Tree

```mermaid
flowchart TD
    I[insertAfter(curNode, newNode)] --> E{head == nullptr?}
    E -- Yes --> Case1["Case 1 — Empty List:\nhead = tail = newNode\nnewNode.prev = ∅\nnewNode.next = ∅"]
    E -- No --> T{curNode == tail?}
    T -- Yes --> Case2["Case 2 — Append After Tail:\ncurNode.next = newNode\nnewNode.prev = curNode\nnewNode.next = ∅\ntail = newNode"]
    T -- No --> Case3["Case 3 — Middle Insertion:\nsucNode = curNode.next\nnewNode.prev = curNode\nnewNode.next = sucNode\ncurNode.next = newNode\nsucNode.prev = newNode"]
```

## Case Studies

### Case 1: Empty List
New node becomes both head and tail.

Before:
```
head -> ∅
tail -> ∅
```

After:
```
head -> [ A ] <- tail
```

Expanded pointers:
- head = tail = A
- A.prev = ∅
- A.next = ∅

Effect: Node A is the sole element; list length becomes 1.

---

### Case 2: Append After Tail
curNode is the tail; append at the end.

Before:
```
head -> [ A ] <- tail
```

After:
```
head -> [ A ] -> [ B ] <- tail
```

Expanded pointers:
- A.next = B
- B.prev = A
- B.next = ∅
- tail = B

Effect: Node B appended; A.next and B.prev updated; B is new tail.

---

### Case 3:  Middle Insertion
Insert C after A when the list is A -> B.

Before:
```
head -> [ A ] -> [ B ] <- tail
```

After:
```
head -> [ A ] -> [ C ] -> [ B ] <- tail
```

Expanded pointers:
- sucNode = A.next (B)
- C.prev = A
- C.next = B
- A.next = C
- B.prev = C

Effect: Node C spliced between A and B; four pointers rewired (A.next, C.prev, C.next, B.prev).

## Traversals

- Forward:  [ A ] <-> [ C ] <-> [ B ]
- Backward: [ B ] <-> [ C ] <-> [ A ]

Note: Forward traversal follows `next` pointers; backward traversal follows `prev` pointers.

## End-to-End Summary

```mermaid
flowchart TD
    S([Start]) --> A[insertAfter(nullptr, A) — Case 1]
    A --> B[insertAfter(A, B) — Case 2]
    B --> C[insertAfter(A, C) — Case 3]
    C --> F[printForward]
    C --> R[printBackward]
    F --> E([End])
    R --> E
```

## Notes

- Bracketed terms like `[ A ]` are nodes; arrows indicate pointer direction or control flow as context requires.
- The three `insertAfter` cases are mutually exclusive and cover empty, tail-append, and middle splice.
- In this program run: Case 1 → Case 2 → Case 3.

### Ownership and Debug Checks

- Ownership: the list owns nodes that are inserted; `remove(node)` and the destructor free them.
- `insertAfter` returns `bool` (success/failure) and includes guards (null, self-insert, already-linked).
- Additional APIs: `remove(node)` unlinks and deletes in O(1); `detach(node)` unlinks without delete; `clear()` deletes all nodes.
- Optional safety: compile with `-DDLLIST_DEBUG_CHECKS` to enable membership checks and runtime invariants (also runs a small `sanity()` check in `main`).

### Main Program Outline

- Construct empty list → allocate nodes `A`, `B`, `C` (detached).
- Insert: `insertAfter(nullptr, A)`, then `insertAfter(A, B)`, then `insertAfter(A, C)`.
- Print forward and backward; on exit, destructor calls `clear()` to free nodes.
- When compiled with `-DDLLIST_DEBUG_CHECKS`, each mutation is followed by a quick `sanity()` invariant check.

Note on Generic Version
- The templated header (`include/doubly_linked_list.hpp`) adds `insertBefore`, `push_front`, and `push_back` for broader usage with any `T`. The core flow remains the same; the additional operations just introduce symmetric “before-head” and convenience insertions. A REPL in `generic_main.cpp` demonstrates these.

### REPL Commands (generic_main.cpp)

- push_front VALUE, push_back VALUE
- insert_after INDEX VALUE, insert_before INDEX VALUE
- remove_at INDEX, detach_at INDEX
- find VALUE, clear, print, print_rev, size
- menu (return to main menu), quit

## insertBefore — Decision Tree and Cases

```mermaid
flowchart TD
    I[insertBefore(curNode, newNode)] --> E{head == nullptr?}
    E -- Yes --> Empty["Empty List:\nhead = tail = newNode\nnewNode.prev = ∅\nnewNode.next = ∅"]
    E -- No --> H{curNode == head?}
    H -- Yes --> BeforeHead["Insert before head:\nnewNode.next = head\nhead.prev = newNode\nhead = newNode\nnewNode.prev = ∅"]
    H -- No --> Middle["Middle insert:\npred = curNode.prev\nnewNode.prev = pred\nnewNode.next = curNode\npred.next = newNode\ncurNode.prev = newNode"]
```

Snapshots

Before:
```
head -> [ A ] -> [ C ] -> [ D ] <- tail
```

After (insert X before C):
```
head -> [ A ] -> [ X ] -> [ C ] -> [ D ] <- tail
```

---

## remove(node) — Decision Tree and Cases

```mermaid
flowchart TD
    R[remove(node)] --> Z{node == ∅?}
    Z -- Yes --> Done[No-op]
    Z -- No --> S{head == tail == node?}
    S -- Yes --> Single["Single-node list:\nhead = tail = ∅"]
    S -- No --> H{node == head?}
    H -- Yes --> RemHead["Remove head:\nhead = head.next\nhead.prev = ∅"]
    H -- No --> T{node == tail?}
    T -- Yes --> RemTail["Remove tail:\ntail = tail.prev\ntail.next = ∅"]
    T -- No --> Mid["Remove middle:\nnode.prev.next = node.next\nnode.next.prev = node.prev"]
```

Snapshots

- Remove head

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After:
```
head -> [ B ] -> [ C ] <- tail
```

- Remove tail

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After:
```
head -> [ A ] -> [ B ] <- tail
```

- Remove middle

Before:
```
head -> [ A ] -> [ X ] -> [ C ] <- tail
```

After:
```
head -> [ A ] -> [ C ] <- tail
```

- Single-node list

Before/After:
```
head -> [ A ] <- tail   ==>   head -> ∅, tail -> ∅
```

---

## clear() — Decision Tree and Effect

```mermaid
flowchart TD
    C[clear()] --> E{head == ∅?}
    E -- Yes --> Done[No-op]
    E -- No --> Loop["Iterate from head, delete nodes"] --> Reset["head = ∅\ntail = ∅"]
```

Effect

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After:
```
head -> ∅
tail -> ∅
```

---

## find(value) — Decision Tree and Example

```mermaid
flowchart TD
    F[find(value)] --> N{head == ∅?}
    N -- Yes --> ReturnNull[return ∅]
    N -- No --> It["cur = head\nwhile cur:\n  if cur.value == value: return cur\n  cur = cur.next\nreturn ∅"]
```

Example

```
head -> [ A ] -> [ C ] -> [ B ] <- tail

find("C")  => returns node [ C ]
find("Z")  => returns ∅
```

---

## reverse() — Decision Tree and Before/After

```mermaid
flowchart TD
    R[reverse()] --> E{empty or single?}
    E -- Yes --> Done[No-op]
    E -- No --> Loop["cur = head\nwhile cur:\n  swap(cur.next, cur.prev)\n  cur = cur.prev  (old next)\n"] --> SwapHT[swap(head, tail)]
```

Before/After

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After:
```
head -> [ C ] -> [ B ] -> [ A ] <- tail
```

## Future Work
 
- Possible extensions beyond what’s diagrammed here: `insertAt(index)`, `removeAt(index)`, iterators, and range-based traversal examples.
