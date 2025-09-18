// Generic doubly linked list (header-only) for educational use.
// - Templated Node<T> and DoublyLinkedList<T>
// - insertAfter / insertBefore (bool), push_front / push_back, remove / detach, clear
// - Optional membership checks under DLLIST_DEBUG_CHECKS

#pragma once

#include <cstddef>
#include <iostream>

#ifdef DLLIST_DEBUG_CHECKS
#include <cassert>
#endif

namespace edu {

template <typename T>
struct Node {
    T value;
    Node<T>* prev;
    Node<T>* next;
    Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
};

template <typename T>
struct DoublyLinkedList {
    // list owns nodes you insert; remove() and the destructor delete them
    Node<T>* head;
    Node<T>* tail;
    std::size_t size_;

    DoublyLinkedList() : head(nullptr), tail(nullptr), size_(0) {}
    ~DoublyLinkedList() { clear(); }

    // insert a node after a given node
    // returns true on success; updates pointers for empty, tail, and middle cases
    bool insertAfter(Node<T>* curNode, Node<T>* newNode) {
        if (head != nullptr && curNode == nullptr) {
            std::cerr << "insertAfter error: curNode is null for non-empty list\n";
            return false;
        }
        if (newNode == nullptr) {
            std::cerr << "insertAfter error: newNode is null\n";
            return false;
        }
        if (newNode == curNode) {
            std::cerr << "insertAfter error: newNode cannot be the same as curNode\n";
            return false;
        }
        if (newNode->prev != nullptr || newNode->next != nullptr) {
            std::cerr << "insertAfter error: newNode is already linked\n";
            return false;
        }
#ifdef DLLIST_DEBUG_CHECKS
        if (head != nullptr) {
            bool found = false;
            for (Node<T>* t = head; t != nullptr; t = t->next) {
                if (t == curNode) { found = true; break; }
            }
            if (!found) {
                std::cerr << "insertAfter error: curNode not in this list\n";
                return false;
            }
        }
#endif

        if (head == nullptr) {
            head = tail = newNode;
            ++size_;
            return true;
        }
        if (curNode == tail) {
            curNode->next = newNode;
            newNode->prev = curNode;
            newNode->next = nullptr;
            tail = newNode;
            ++size_;
            return true;
        }
        Node<T>* sucNode = curNode->next;
        newNode->prev = curNode;
        newNode->next = sucNode;
        curNode->next = newNode;
        sucNode->prev = newNode;
        ++size_;
        return true;
    }

    // insert a node before a given node
    // returns true on success; handles empty, head, and middle cases
    bool insertBefore(Node<T>* curNode, Node<T>* newNode) {
        if (head != nullptr && curNode == nullptr) {
            std::cerr << "insertBefore error: curNode is null for non-empty list\n";
            return false;
        }
        if (newNode == nullptr) {
            std::cerr << "insertBefore error: newNode is null\n";
            return false;
        }
        if  (newNode == curNode) {
            std::cerr << "insertBefore error: newNode cannot be the same as curNode\n";
            return false;
        }
        if (newNode->prev != nullptr || newNode->next != nullptr) {
            std::cerr << "insertBefore error: newNode is already linked\n";
            return false;
        }
#ifdef DLLIST_DEBUG_CHECKS
        if (head != nullptr) {
            bool found = false;
            for (Node<T>* t = head; t != nullptr; t = t->next) {
                if (t == curNode) { found = true; break; }
            }
            if (!found) {
                std::cerr << "insertBefore error: curNode not in this list\n";
                return false;
            }
        }
#endif
        if (head == nullptr) {
            head = tail = newNode;
            ++size_;
            return true;
        }
        if (curNode == head) {
            newNode->next = head;
            newNode->prev = nullptr;
            head->prev = newNode;
            head = newNode;
            ++size_;
            return true;
        }
        Node<T>* pred = curNode->prev;
        newNode->prev = pred;
        newNode->next = curNode;
        pred->next = newNode;
        curNode->prev = newNode;
        ++size_;
        return true;
    }

    // convenience: construct and append/prepend values
    bool push_back(const T& v) {
        Node<T>* n = new Node<T>(v);
        if (head == nullptr) {
            head = tail = n;
            ++size_;
            return true;
        }
        n->prev = tail;
        tail->next = n;
        tail = n;
        ++size_;
        return true;
    }

    bool push_front(const T& v) {
        Node<T>* n = new Node<T>(v);
        if (head == nullptr) {
            head = tail = n;
            ++size_;
            return true;
        }
        n->next = head;
        head->prev = n;
        head = n;
        ++size_;
        return true;
    }

    bool remove(Node<T>* target) {
        if (target == nullptr) {
            std::cerr << "remove error: target is null\n";
            return false;
        }
        if (head == nullptr) {
            std::cerr << "remove error: list is empty\n";
            return false;
        }
#ifdef DLLIST_DEBUG_CHECKS
        bool found = false;
        for (Node<T>* t = head; t != nullptr; t = t->next) {
            if (t == target) { found = true; break; }
        }
        if (!found) {
            std::cerr << "remove error: target not in this list\n";
            return false;
        }
#endif
        if (target->prev) { target->prev->next = target->next; } else { head = target->next; }
        if (target->next) { target->next->prev = target->prev; } else { tail = target->prev; }
        target->prev = nullptr; target->next = nullptr;
        delete target;
        --size_;
        return true;
    }

    bool detach(Node<T>* target) {
        if (target == nullptr) {
            std::cerr << "detach error: target is null\n";
            return false;
        }
        if (head == nullptr) {
            std::cerr << "detach error: list is empty\n";
            return false;
        }
#ifdef DLLIST_DEBUG_CHECKS
        bool found = false;
        for (Node<T>* t = head; t != nullptr; t = t->next) {
            if (t == target) { found = true; break; }
        }
        if (!found) {
            std::cerr << "detach error: target not in this list\n";
            return false;
        }
#endif
        if (target->prev) { target->prev->next = target->next; } else { head = target->next; }
        if (target->next) { target->next->prev = target->prev; } else { tail = target->prev; }
        target->prev = nullptr; target->next = nullptr;
        --size_;
        return true;
    }

    void clear() {
        Node<T>* cur = head;
        while (cur) {
            Node<T>* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr; tail = nullptr; size_ = 0;
    }

    Node<T>* find(const T& v) const {
        Node<T>* cur = head;
        while (cur) {
            if (cur->value == v) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // simple printers (requires operator<< for T)
    void printForward() const {
        Node<T>* cur = head;
        while (cur) {
            std::cout << cur->value;
            if (cur->next) std::cout << " <-> ";
            cur = cur->next;
        }
        std::cout << std::endl;
    }
    void printBackward() const {
        Node<T>* cur = tail;
        while (cur) {
            std::cout << cur->value;
            if (cur->prev) std::cout << " <-> ";
            cur = cur->prev;
        }
        std::cout << std::endl;
    }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
    DoublyLinkedList(DoublyLinkedList&&) = delete;
    DoublyLinkedList& operator=(DoublyLinkedList&&) = delete;
};

#ifdef DLLIST_DEBUG_CHECKS
template <typename T>
static void sanity(DoublyLinkedList<T>& L) {
    std::size_t f = 0; for (Node<T>* p=L.head; p; p=p->next) ++f;
    std::size_t b = 0; for (Node<T>* p=L.tail; p; p=p->prev) ++b;
    assert(f == L.size());
    assert(b == L.size());
    if (L.empty()) { assert(L.head==nullptr && L.tail==nullptr); }
    else { assert(L.head->prev==nullptr && L.tail->next==nullptr); }
}
#endif

} // namespace edu
