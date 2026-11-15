// File:       src/containers/Queue.h
// Owner:      Farwa
// Purpose:    FIFO queue built on LinkedList. A robot's planned route is a
//             Queue of connection indices: arriving at a location removes the
//             front element in O(1); rerouting replaces the whole queue
//             (Report 5.8).
// Depends on: LinkedList
// Used by:    Robot (route), Movement, Rerouter, Router (builds routes)
// Milestone:  M1
#ifndef RESQGRID_QUEUE_H
#define RESQGRID_QUEUE_H

#include "LinkedList.h"

template <typename T>
class Queue {
public:
    // TODO(Farwa): add to the back (list.pushBack).
    // See: report Section 5.8   |   Done when: test FIFO order kept
    void enqueue(const T& value) { (void)value; }

    // TODO(Farwa): remove the front into `out` (list.popFront); false if empty.
    // See: report Section 5.8   |   Done when: test FIFO order kept
    bool dequeue(T& out) { (void)out; return false; }

    // TODO(Farwa): copy the front value into `out` without removing it; false if empty.
    // See: report Section 5.8   |   Done when: front() then dequeue() give the same value
    bool front(T& out) const { (void)out; return false; }

    // TODO(Farwa): remove everything (list.clear).
    // See: report Section 5.8   |   Done when: isEmpty() after clear()
    void clear() {}

    bool isEmpty() const { return items.isEmpty(); }
    int  size() const    { return items.size(); }

    // Read-only walk over the queue from front to back (Rerouter scans routes,
    // the Renderer draws them, Router::routeToString prints them).
    const typename LinkedList<T>::Node* first() const { return items.first(); }

private:
    LinkedList<T> items;
};

#endif // RESQGRID_QUEUE_H
