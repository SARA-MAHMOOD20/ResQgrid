// File:       src/containers/IndexedMaxHeap.h
// Owner:      Sara
// Purpose:    Indexed binary max-heap of request indices: always gives the most
//             important PENDING request and supports change-key for
//             SET_URGENCY (Report 5.6).
// Depends on: DynamicArray
// Used by:    Coordinator (owns the pending heap), EventProcessor (SET_URGENCY
//             change-key), Rerouter / EventProcessor (put released requests back)
// Milestone:  M1
//
// Items are request indices 0..capacity-1. The caller computes the key with
// Request::priorityKey() = A * urgency - report_time, and passes the request's
// ID as `tieId`. Order: LARGER key first; equal keys -> SMALLER tieId first.
//
// Arrays:
//   heap[i]    = item stored at heap position i   (size = number in heap)
//   pos[item]  = heap position of item, or -1 if not in the heap
//   key[item], tie[item] = current priority of item
// Invariant: for every item in the heap, heap[pos[item]] == item, and every
// parent ranks at least as high as its children. Update pos[] on EVERY swap.
//
// Header-only like the template containers (not a template itself, so that it
// can be tested without Request).
#ifndef RESQGRID_INDEXED_MAX_HEAP_H
#define RESQGRID_INDEXED_MAX_HEAP_H

#include "DynamicArray.h"

class IndexedMaxHeap {
public:
    IndexedMaxHeap() : count(0) {}

    // TODO(Sara): size pos[] (all -1), key[], tie[] and heap[] for `capacity`
    // items (= number of requests); count = 0. Called once after loading.
    // See: report Section 5.6   |   Done when: contains(i) is false for every i after init
    void init(int capacity) { (void)capacity; }

    // TODO(Sara): store key/tie, put item at the end, siftUp. Ignore (or return
    // false for) an item that is already in the heap.
    // See: report Section 5.6   |   Done when: extractMax returns items in priority order
    bool insert(int item, long long itemKey, int tieId) {
        (void)item; (void)itemKey; (void)tieId;
        return false;
    }

    // TODO(Sara): return the root item (NO_INDEX = -1 if empty), move the last
    // item to the root, set pos[root item] = -1, siftDown(0).
    // See: report Section 5.6   |   Done when: extractMax returns items in priority order
    int extractMax() { return -1; }

    // TODO(Sara): root item without removing it, -1 if empty. O(1).
    // See: report Section 5.6   |   Done when: peekMax() equals the next extractMax()
    int peekMax() const { return -1; }

    // TODO(Sara): set key[item] = newKey, then siftUp or siftDown from pos[item].
    // Return false if the item is not in the heap.
    // See: report Sections 5.6 and 6.3 (SET_URGENCY)   |   Done when: test SET_URGENCY moves a request up
    bool changeKey(int item, long long newKey) { (void)item; (void)newKey; return false; }

    // TODO(Sara): pos[item] != -1 (and item within range).
    // See: report Section 5.6   |   Done when: true after insert, false after extractMax
    bool contains(int item) const { (void)item; return false; }

    // TODO(Sara): empty the heap (set pos[] back to -1 for every item that was in it).
    // See: report Section 5.6   |   Done when: isEmpty() and contains() false for all
    void clear() {}

    int  size() const    { return count; }
    bool isEmpty() const { return count == 0; }

    // Read-only access for the UI's pending list (heap order, not sorted).
    int itemAt(int heapPos) const { return heap[heapPos]; }

private:
    // TODO(Sara): true if item a must come before item b
    // (key[a] > key[b], or keys equal and tie[a] < tie[b]).
    // See: report Section 5.6   |   Done when: equal urgency + time -> smaller request ID first
    bool higher(int a, int b) const { (void)a; (void)b; return false; }

    // TODO(Sara): swap heap[i] and heap[j] AND update pos[] for both items.
    // See: report Section 5.6 (invariant)   |   Done when: heap[pos[x]] == x after every operation
    void swapAt(int i, int j) { (void)i; (void)j; }

    // TODO(Sara): standard sift-up / sift-down using higher().
    // See: report Section 5.6   |   Done when: O(log n) per operation
    void siftUp(int i)   { (void)i; }
    void siftDown(int i) { (void)i; }

    DynamicArray<int>       heap;
    DynamicArray<int>       pos;
    DynamicArray<long long> key;
    DynamicArray<int>       tie;
    int count;
};

#endif // RESQGRID_INDEXED_MAX_HEAP_H
