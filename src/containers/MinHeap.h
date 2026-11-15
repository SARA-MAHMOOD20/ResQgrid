// File:       src/containers/MinHeap.h
// Owner:      Rabia
// Purpose:    Array-based binary min-heap. Dijkstra stores (distance, location)
//             entries in it and uses LAZY DELETION: a shorter distance pushes
//             a new entry instead of updating the old one; outdated entries are
//             skipped when popped (Report 5.5).
// Depends on: DynamicArray
// Used by:    Router
// Milestone:  M1
//
// T must support `a < b` (Router defines DistEntry with operator<).
// push / popMin O(log n); top / isEmpty O(1). Root is at index 0; children of
// i are 2i+1 and 2i+2; parent is (i-1)/2.
#ifndef RESQGRID_MIN_HEAP_H
#define RESQGRID_MIN_HEAP_H

#include "DynamicArray.h"

template <typename T>
class MinHeap {
public:
    // TODO(Rabia): append at the end, then siftUp from the last index.
    // See: report Section 5.5   |   Done when: pops in sorted order
    void push(const T& value) { (void)value; }

    // TODO(Rabia): if empty return false; copy root into `out`, move the last
    // element to the root, items.pop_back(), siftDown(0), return true.
    // See: report Section 5.5   |   Done when: pops in sorted order
    bool popMin(T& out) { (void)out; return false; }

    // TODO(Rabia): copy the smallest element into `out` without removing it; false if empty.
    // See: report Section 5.5   |   Done when: top() equals the next popMin()
    bool top(T& out) const { (void)out; return false; }

    // TODO(Rabia): make the heap empty (reused by every Dijkstra run).
    // See: report Section 8 (space: one Dijkstra run, reused)   |   Done when: isEmpty() after clear()
    void clear() {}

    // TODO(Rabia): return whether the heap holds no elements.
    // See: report Section 5.5   |   Done when: false after push, true after popping everything
    bool isEmpty() const { return true; }

    // TODO(Rabia): number of elements currently in the heap.
    // See: report Section 5.5   |   Done when: grows by 1 per push, shrinks by 1 per popMin
    int size() const { return 0; }

private:
    // TODO(Rabia): standard sift-up / sift-down using `<`.
    // See: report Section 5.5   |   Done when: heap property holds after every push/pop
    void siftUp(int i)   { (void)i; }
    void siftDown(int i) { (void)i; }

    DynamicArray<T> items;
};

#endif // RESQGRID_MIN_HEAP_H
