#include <iostream>
#include <string>
#include <cstddef>
#include <cstdlib>

// simple ANSI colors for a friendly educational header (safe to ignore if unsupported)
static constexpr const char *C_RESET  = "\033[0m";
static constexpr const char *C_BOLD   = "\033[1m";
static constexpr const char *C_CYAN   = "\033[36m";
static constexpr const char *C_GREEN  = "\033[32m";
static constexpr const char *C_MAGENTA= "\033[35m";
#ifdef DLLIST_DEBUG_CHECKS
#include <cassert>
#endif

struct Node {
    std::string value;
    Node *prev;
    Node *next;
    // declare contructor with previous and next set to null
    Node(const std::string &v) : value(v), prev(nullptr), next(nullptr) {
    }
};

struct DoublyLinkedList {
    // list owns nodes you insert; remove() and the destructor delete them
    // list always has a head and a tail
    Node *head;
    Node *tail;
    std::size_t size_; //size keeps track of the number of nodes in the list

    // declare constructor with head and tail set to null
    DoublyLinkedList() : head(nullptr), tail(nullptr), size_(0) {
    }
    //calls clear() to delete all nodes this is for memory management
    ~DoublyLinkedList() {
        clear();
    }

    /*
     * head != nullptr → list already has stuff in it.
     * curNode == nullptr → but you told me “insert after nothing
     * Someone calls insertAfter(nullptr, newNode) on a list that already has elements.
     * Guard triggers → error printed → no insertion → program doesn’t crash.
     */

    // insert a node after a given node
    // returns true on success; updates pointers for empty, tail, and middle cases
    // educational: we check for common mistakes first (nulls, self-insert, already-linked, membership)
    bool insertAfter(Node *curNode, Node *newNode) {

        // Guard: prevent null curNode in non-empty list (would cause crash)
        if (head != nullptr && curNode == nullptr) {
            std::cerr << "insertAfter error: curNode is null for non-empty list\n";
            return false;
        }

        // guard: new node must exist
        if (newNode == nullptr) {
            std::cerr << "insertAfter error: newNode is null\n";
            return false;
        }
        // guard: cannot insert a node after itself
        if (newNode == curNode) {
            std::cerr << "insertAfter error: newNode cannot be the same as curNode\n";
            return false;
        }
        // guard: newNode must be detached (not already linked in any list)
        if (newNode->prev != nullptr || newNode->next != nullptr) {
            std::cerr << "insertAfter error: newNode is already linked\n";
            return false;
        }
        // optional guard: ensure curNode belongs to this list when not empty
        #ifdef DLLIST_DEBUG_CHECKS
        if (head != nullptr) {
            bool found = false;
            for (Node *t = head; t != nullptr; t = t->next) {
                if (t == curNode) { found = true; break; }
            }
            if (!found) {
                std::cerr << "insertAfter error: curNode not in this list\n";
                return false;
            }
        }
        #endif

        /**
        * Case 1: list is empty
        * @brief If a list is empty, head and tail are null.
        *        The new node becomes both.
        *
        * Before insertion:
        *   head -> null
        *   tail -> null
        *
        * After inserting newNode:
        *   head -> newNode <- tail
        *           |
        *        newNode->next = null
        *        newNode->prev = null
        */

        if (head == nullptr) {
            head = tail = newNode;
            ++size_;
            return true;
        }

        // Case 2: insert after tail
        if (curNode == tail) {
            curNode->next = newNode;
            newNode->prev = curNode;
            newNode->next = nullptr; // ensure new tail has no next
            tail = newNode;
            ++size_; //increment size to account for new node
            return true;
        }

        /* Case 2: insert after tail
         * Before insertion:
         *   ... <-> curNode == tail -> null
         *
         * After insertion:
         *   ... <-> curNode <-> newNode
         *
         * Expanded pointers:
         *   curNode->next = newNode
         *   newNode->prev = curNode
         *   newNode->next = null
         *   tail = newNode
         *
         *   So the chain becomes:
         *   ... <-> curNode (old tail) <-> newNode (new tail)
         */

        // Case 3: insert in middle
        Node *sucNode = curNode->next; // sucNode --> curNode
        newNode->prev = curNode;  // curNode -> newNode
        newNode->next = sucNode;  // newNode -> sucNode
        curNode->next = newNode;  // curNode -> newNode
        sucNode->prev = newNode;  // sucNode <- newNode
        ++size_;
        return true;
    }

    // insert a node before a given node
    // returns true on success; handles empty list, inserting before head, and middle cases
    bool insertBefore(Node *curNode, Node *newNode) {
        // Guard: prevent null curNode in non-empty list (would cause crash)
        if (head != nullptr && curNode == nullptr) {
            std::cerr << "insertBefore error: curNode is null for non-empty list\n";
            return false;
        }
        // guard: new node must exist
        if (newNode == nullptr) {
            std::cerr << "insertBefore error: newNode is null\n";
            return false;
        }
        // guard: cannot insert a node before itself
        if (newNode == curNode) {
            std::cerr << "insertBefore error: newNode cannot be the same as curNode\n";
            return false;
        }
        // guard: newNode must be detached (not already linked in any list)
        if (newNode->prev != nullptr || newNode->next != nullptr) {
            std::cerr << "insertBefore error: newNode is already linked\n";
            return false;
        }
        // optional guard: ensure curNode belongs to this list when not empty
        #ifdef DLLIST_DEBUG_CHECKS
        if (head != nullptr) {
            bool found = false;
            for (Node *t = head; t != nullptr; t = t->next) {
                if (t == curNode) { found = true; break; }
            }
            if (!found) {
                std::cerr << "insertBefore error: curNode not in this list\n";
                return false;
            }
        }
        #endif

        // Case 1: list is empty → newNode becomes head and tail
        if (head == nullptr) {
            head = tail = newNode;
            ++size_;
            return true;
        }
        // Case 2: insert before head → update head pointer
        if (curNode == head) {
            newNode->next = head;
            newNode->prev = nullptr;
            head->prev = newNode;
            head = newNode;
            ++size_;
            return true;
        }
        // Case 3: insert in middle → rewire predecessor and current
        Node *pred = curNode->prev; // predecessor of curNode
        newNode->prev = pred;       // link newNode back to predecessor
        newNode->next = curNode;    // link newNode forward to curNode
        pred->next = newNode;       // predecessor now points to newNode
        curNode->prev = newNode;    // curNode now points back to newNode
        ++size_;
        return true;
    }

    /*
    * Before insertion:
    *   ... <-> curNode <-> sucNode <-> ...
    *
    * After insertion:
    *   ... <-> curNode <-> newNode <-> sucNode <-> ...
    */

    // print the list from head to tail
    void printForward() const { // const for safety so we don't accidentally modify the list
        Node *cur = head;
        while (cur) {
            std::cout << cur->value; // print the value of the current node
            if (cur->next) std::cout << " <-> "; // print " <-> " if there is a next node
            cur = cur->next; // move to the next node
        }
        std::cout << std::endl;
    }
    // pretty, colored variant used only for the educational demo when enabled via env var
    void printForwardColored() const {
        Node *cur = head;
        while (cur) {
            std::cout << C_BOLD << C_CYAN << cur->value << C_RESET; // colored node value
            if (cur->next) std::cout << " " << C_MAGENTA << "<->" << C_RESET << " ";
            cur = cur->next;
        }
        std::cout << std::endl;
    }
    // print the list from tail to head
    void printBackward() const { //const for safety so we don't accidentally modify the list
        Node *cur = tail;
        while (cur) {
            std::cout << cur->value;
            if (cur->prev) std::cout << " <-> ";
            cur = cur->prev;
        }
        std::cout << std::endl;
    }
    // pretty, colored variant used only for the educational demo when enabled via env var
    void printBackwardColored() const {
        Node *cur = tail;
        while (cur) {
            std::cout << C_BOLD << C_CYAN << cur->value << C_RESET;
            if (cur->prev) std::cout << " " << C_MAGENTA << "<->" << C_RESET << " ";
            cur = cur->prev;
        }
        std::cout << std::endl;
    }

    void clear() {
        Node *cur = head;
        while (cur) {
            Node *next = cur->next;
            cur->prev = nullptr;
            cur->next = nullptr;
            delete cur;
            cur = next;
        }
        head = nullptr;
        tail = nullptr;
        size_ = 0;
    }

    // return the number of elements currently stored in the list.
    // std::size_t is the standard unsigned type used for sizes/indices.
    std::size_t size() const {
        return size_;  // just give back the internal counter
    }

    // check if the list is empty.
    // returns true when size_ == 0, false otherwise.
    // cleaner than writing (list.size() == 0) everywhere.
    bool empty() const {
        return size_ == 0;
    }
    
    // disable copying and moving: this list owns raw Node* and a default
    // shallow copy/move would duplicate head/tail pointers and cause double-delete.
    // deleting these operations makes ownership rules explicit and safe.
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
    DoublyLinkedList(DoublyLinkedList&&) = delete;
    DoublyLinkedList& operator=(DoublyLinkedList&&) = delete;

    // remove a node from the list in O(1)
    // returns true on success; list deletes the node because it owns nodes
    // handles removing head, tail, or a middle node by fixing neighbor links
    bool remove(Node* target) {
        // guard: target must exist
        if (target == nullptr) {
            std::cerr << "remove error: target is null\n";
            return false;
        }
        // guard: list must not be empty
        if (head == nullptr) {
            std::cerr << "remove error: list is empty\n";
            return false;
        }

        #ifdef DLLIST_DEBUG_CHECKS
        bool found = false;
        for (Node* t = head; t != nullptr; t = t->next) {
            if (t == target) { found = true; break; }
        }
        if (!found) {
            std::cerr << "remove error: target not in this list\n";
            return false;
        }
        #endif

        // splice out of the chain: fix previous neighbor or move head
        if (target->prev) {
            target->prev->next = target->next;
        } else {
            head = target->next;
        }

        // fix next neighbor or move tail
        if (target->next) {
            target->next->prev = target->prev;
        } else {
            tail = target->prev;
        }

        // scrub links for safety, then delete and decrement size
        target->prev = nullptr;
        target->next = nullptr;
        delete target;
        --size_;
        return true;
    }
    // detach a node from the list in O(1)
    // unlike remove(), this does NOT delete the node; it just unlinks it
    // the caller keeps ownership of the detached node (prev/next set to nullptr)
    bool detach(Node* target) {
        // guard: target must exist
        if (target == nullptr) {
            std::cerr << "detach error: target is null\n";
            return false;
        }
        // guard: list must not be empty
        if (head == nullptr) {
            std::cerr << "detach error: list is empty\n";
            return false;
        }
        // optional guard: ensure target belongs to this list when checks are enabled
        #ifdef DLLIST_DEBUG_CHECKS
        bool found = false;
        for (Node* t = head; t != nullptr; t = t->next) {
            if (t == target) { found = true; break; }
        }
        if (!found) {
            std::cerr << "detach error: target not in this list\n";
            return false;
        }
        #endif

        // unlink from previous neighbor or move head
        if (target->prev) {
            target->prev->next = target->next;
        } else {
            head = target->next;
        }

        // unlink from next neighbor or move tail
        if (target->next) {
            target->next->prev = target->prev;
        } else {
            tail = target->prev;
        }

        // leave target detached for caller reuse
        target->prev = nullptr;
        target->next = nullptr;
        --size_;
        return true;
    }

    // find the first node whose value equals v
    // returns a pointer to the node or nullptr if not found
    Node* find(const std::string& v) const {
        Node* cur = head;
        while (cur) {
            if (cur->value == v) return cur;
            cur = cur->next;
        }
        return nullptr;
    }
};
#ifdef DLLIST_DEBUG_CHECKS
static void sanity(DoublyLinkedList& L) {
    std::size_t f = 0; for (Node* p=L.head; p; p=p->next) ++f;
    std::size_t b = 0; for (Node* p=L.tail; p; p=p->prev) ++b;
    assert(f == L.size());
    assert(b == L.size());
    if (L.empty()) {
        assert(L.head == nullptr && L.tail == nullptr);
    } else {
        assert(L.head->prev == nullptr && L.tail->next == nullptr);
    }
}
#endif
static bool color_enabled() {
    const char* env = std::getenv("DLLIST_COLOR");
    return env && std::string(env) == "1";
}

static void print_banner(bool color) {
    const std::string title = " Doubly Linked List — Educational Demo";
    const std::string bar(title.size(), '=');
    if (color) std::cout << C_CYAN << C_BOLD;
    std::cout << bar << "\n" << title << "\n" << bar;
    if (color) std::cout << C_RESET;
    std::cout << "\n";
}

int main() {
    const bool use_color = color_enabled();
    print_banner(use_color);

    DoublyLinkedList list;                 // construct an empty list (head=nullptr, tail=nullptr, size_=0)
    // create nodes
    Node *a = new Node("A");              // allocate a detached node labeled "A"
    Node *b = new Node("B");              // allocate a detached node labeled "B"
    Node *c = new Node("C");              // allocate a detached node labeled "C"

    // insert nodes
    list.insertAfter(nullptr, a);          // empty list, head=tail=A
    #ifdef DLLIST_DEBUG_CHECKS
    sanity(list);                          // debug-only: verify size and link invariants
    #endif
    list.insertAfter(a, b);// insert B after A
    #ifdef DLLIST_DEBUG_CHECKS
    sanity(list);                          // debug-only: verify size and link invariants
    #endif
    list.insertAfter(a, c);// insert C after A (so A->C->B)
    #ifdef DLLIST_DEBUG_CHECKS
    sanity(list);                          // debug-only: verify size and link invariants
    #endif

    // print forward and backward (color when enabled, plain otherwise)
    if (use_color) {
        std::cout << C_GREEN << "Forward:   " << C_RESET;
        list.printForwardColored();
        std::cout << C_GREEN << "Backward:  " << C_RESET;
        list.printBackwardColored();
    } else {
        std::cout << "Forward:   ";
        list.printForward();                   // A <-> C <-> B
        std::cout << "Backward:  ";
        list.printBackward();                  // B <-> C <-> A
    }

    // list goes out of scope here; destructor runs and clear() frees all nodes
}
