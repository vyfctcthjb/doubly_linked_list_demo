# Doubly Linked List — Additional Operations
*insertBefore, remove, clear, find — decision trees and ASCII snapshots*

This page extends the main demo with common operations designed for teaching. It shows how to reason about cases and pointer rewiring before you code.

Status note

- Implemented in `main.cpp`: `insertAfter`, `insertBefore`, `remove`, `detach`, `clear`, `find`.
- Implemented in `include/doubly_linked_list.hpp`: `insertAfter`, `insertBefore`, `push_front`, `push_back`, `remove`, `detach`, `clear`, `find`.
- Conceptual (not yet in code): `reverse`.

## API Quick Reference

- Ownership: the list owns inserted nodes; `remove(node)` and the destructor delete them. `detach(node)` unlinks without delete.
- Return values: operations here return `bool` on success where applicable (`remove`, `detach`).
- Debug mode: define `DLLIST_DEBUG_CHECKS` to enable membership checks and a small invariant checker in the demo `main`.

## What You’ll Learn

- How to implement `insertBefore`, `remove`, `clear`, and `find` with correct edge-case handling.
- How pointer updates maintain list integrity at head, tail, and middle positions.
- How to visualize each mutation with compact ASCII diagrams and Mermaid decision trees.

---

## insertBefore(curNode, newNode)

High-level logic

```mermaid
flowchart TD
    I[insertBefore(curNode, newNode)] --> E{head == nullptr?}
    E -- Yes --> Empty["Empty List:\nhead = tail = newNode\nnewNode.prev = ∅\nnewNode.next = ∅"]
    E -- No --> H{curNode == head?}
    H -- Yes --> BeforeHead["Insert before head:\nnewNode.next = head\nhead.prev = newNode\nhead = newNode\nnewNode.prev = ∅"]
    H -- No --> Middle["Middle insert:\npred = curNode.prev\nnewNode.prev = pred\nnewNode.next = curNode\npred.next = newNode\ncurNode.prev = newNode"]
```

Case snapshots (template implementation in `include/doubly_linked_list.hpp`)

- Case A — Empty list

Before:
```
head -> ∅
tail -> ∅
```

After:
```
head -> [ X ] <- tail
```

- Case B — Before head

Before:
```
head -> [ A ] -> [ B ] <- tail
```

After (insert X before A):
```
head -> [ X ] -> [ A ] -> [ B ] <- tail
```

- Case C — Middle

Before:
```
head -> [ A ] -> [ C ] -> [ D ] <- tail
```

After (insert X before C):
```
head -> [ A ] -> [ X ] -> [ C ] -> [ D ] <- tail
```

---

## remove(node)

High-level logic

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

Case snapshots (implemented in both `main.cpp` and the template header)

- Case A — Remove head

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After (remove A):
```
head -> [ B ] -> [ C ] <- tail
```

- Case B — Remove tail

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After (remove C):
```
head -> [ A ] -> [ B ] <- tail
```

- Case C — Remove middle

Before:
```
head -> [ A ] -> [ X ] -> [ C ] <- tail
```

After (remove X):
```
head -> [ A ] -> [ C ] <- tail
```

- Case D — Single-node list

Before:
```
head -> [ A ] <- tail
```

After (remove A):
```
head -> ∅
tail -> ∅
```

---

## detach(node)

Unlinks a node in O(1) without deleting it (caller retains ownership of the detached node). Returns `bool`.

High-level logic

```mermaid
flowchart TD
    D[detach(node)] --> Z{node == ∅?}
    Z -- Yes --> Done[No-op]
    Z -- No --> E{head == ∅?}
    E -- Yes --> Err[error: empty]
    E -- No --> Unlink["Splice out head/tail/middle\nupdate neighbors, head/tail"] --> Scrub[prev=∅, next=∅] --> Dec[--size_]
```

Case snapshot

Before:
```
head -> [ A ] -> [ X ] -> [ C ] <- tail
```

After (detach X):
```
head -> [ A ] -> [ C ] <- tail

// caller still holds pointer X, now detached:
X.prev = ∅, X.next = ∅
```

---

## Debug Checks

- Define `DLLIST_DEBUG_CHECKS` at compile time to enable membership safety for `insertAfter`, `remove`, and `detach`, plus a small runtime invariant sanity check in the demo `main`.
- Example: `g++ -DDLLIST_DEBUG_CHECKS main.cpp -o dll_demo`

## clear()

High-level logic

```mermaid
flowchart TD
    C[clear()] --> E{head == ∅?}
    E -- Yes --> Done[No-op]
    E -- No --> Loop["Iterate from head, delete nodes"] --> Reset["head = ∅\ntail = ∅"]
```

Effect snapshot (implemented in both `main.cpp` and the template header)

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After:
```
head -> ∅
tail -> ∅
```

Note: If using manual memory management (like in the demo), ensure you `delete` each node as you traverse.

---

## find(value)

High-level logic

```mermaid
flowchart TD
    F[find(value)] --> N{head == ∅?}
    N -- Yes --> ReturnNull[return ∅]
    N -- No --> It["cur = head\nwhile cur:\n  if cur.value == value: return cur\n  cur = cur.next\nreturn ∅"]
```

Behavior and complexity

- Returns the first node whose `value` matches; otherwise returns ∅ (null).
- Time complexity: O(n). Space complexity: O(1).

Example snapshot

```
head -> [ A ] -> [ C ] -> [ B ] <- tail

find("C")  => returns node [ C ]
find("Z")  => returns ∅
```

---

## reverse()

Reverses the list in place by swapping each node’s next/prev pointers, then swapping head/tail at the end.

High-level logic

```mermaid
flowchart TD
    R[reverse()] --> E{empty or single?}
    E -- Yes --> Done[No-op]
    E -- No --> Loop["cur = head\nwhile cur:\n  swap(cur.next, cur.prev)\n  cur = cur.prev  (old next)\n"] --> SwapHT[swap(head, tail)]
```

Before/After snapshot

Before:
```
head -> [ A ] -> [ B ] -> [ C ] <- tail
```

After:
```
head -> [ C ] -> [ B ] -> [ A ] <- tail
```

Notes
- Swapping next/prev for each node flips link direction.
- Walking via cur = cur.prev after swap advances along the original next.
- Finally, swap head and tail.

## Implementation Notes

- Maintain strict symmetry between `next` and `prev` updates. Whenever you set one, check the corresponding opposite link.
- Always update `head`/`tail` when inserting/removing at boundaries.
- For safety, consider setting removed node’s `next` and `prev` to ∅ before deletion.
- Tests to add later: boundary cases for each operation, long chains, repeated values, and alternating head/tail removals.

## Future Extensions

- insertAt(index), removeAt(index)
- iterator support and range-based traversal examples

### REPL Availability

The generic demo (`generic_main.cpp`) exposes these operations interactively via commands:

- push_front VALUE, push_back VALUE
- insert_after INDEX VALUE, insert_before INDEX VALUE
- remove_at INDEX, detach_at INDEX
- clear, print, print_rev, size
- menu, quit
