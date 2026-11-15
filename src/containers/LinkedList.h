// File:       src/containers/LinkedList.h
// Owner:      Farwa
// Purpose:    Singly linked list with head and tail pointers. Used for
//             adjacency lists (Report 5.4), hash-map buckets (Report 5.3) and
//             as the storage behind Queue (Report 5.8).
// Depends on: nothing
// Used by:    Queue, HashMap, Environment (adjacency), anything that iterates
// Milestone:  M1 (merge by 16 Nov, everyone needs it)
//
// How to iterate (no STL iterators):
//     for (const LinkedList<T>::Node* n = list.first(); n != nullptr; n = n->next)
//         use(n->value);
#ifndef RESQGRID_LINKED_LIST_H
#define RESQGRID_LINKED_LIST_H

template <typename T>
class LinkedList {
public:
    struct Node {
        T     value;
        Node* next;
        explicit Node(const T& v) : value(v), next(nullptr) {}
    };

    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // TODO(Farwa): deep copy, node by node, keeping the same order.
    // Needed because DynamicArray<LinkedList<...>> copies lists when it grows.
    // See: report Section 5.4   |   Done when: copy has same values; changing it leaves the original unchanged
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) { (void)other; }

    // TODO(Farwa): self-assignment check, clear(), deep copy, return *this.
    // See: report Section 5.4   |   Done when: a = a; and a = b; both work with no leak
    LinkedList& operator=(const LinkedList& other) { (void)other; return *this; }

    // TODO(Farwa): call clear().
    // See: plan Section 6 "destructor frees every node"   |   Done when: no leaks
    ~LinkedList() {}

    // TODO(Farwa): insert at the front in O(1); update tail if the list was empty.
    // See: plan Section 6   |   Done when: test push, pop, iterate, no leaks
    void pushFront(const T& value) { (void)value; }

    // TODO(Farwa): insert at the back in O(1) using tail.
    // See: plan Section 6   |   Done when: iteration order equals push order
    void pushBack(const T& value) { (void)value; }

    // TODO(Farwa): if empty return false; else copy head value into `out`,
    // delete the head node, fix tail when the list becomes empty, return true.
    // See: report Section 5.8   |   Done when: test push, pop, iterate, no leaks
    bool popFront(T& out) { (void)out; return false; }

    // TODO(Farwa): delete every node; head = tail = nullptr; count = 0.
    // See: plan Section 6   |   Done when: no leaks
    void clear() {}

    // First node for iteration (nullptr when empty). Front value: first()->value.
    Node*       first()       { return head; }
    const Node* first() const { return head; }

    int  size() const    { return count; }
    bool isEmpty() const { return count == 0; }

private:
    Node* head;
    Node* tail;
    int   count;
};

#endif // RESQGRID_LINKED_LIST_H
