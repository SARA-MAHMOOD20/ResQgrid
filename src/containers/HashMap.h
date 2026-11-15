// File:       src/containers/HashMap.h
// Owner:      Rabia
// Purpose:    Map from an external integer ID (as written in the file) to an
//             internal index. Separate chaining: an array of LinkedList
//             buckets, h(k) = k mod m, m a prime about twice the number of
//             entries (Report 5.3). A failed insert is how the loader detects
//             duplicate IDs.
// Depends on: LinkedList, DynamicArray
// Used by:    World (the ID -> index maps), ScenarioLoader, EventProcessor
// Milestone:  M1
//
// Keys are int IDs (positive in valid files, but negative keys must not crash:
// use a non-negative modulo). Insert/find O(1) average, O(n) worst case.
#ifndef RESQGRID_HASH_MAP_H
#define RESQGRID_HASH_MAP_H

#include "DynamicArray.h"
#include "LinkedList.h"

template <typename V>
class HashMap {
public:
    struct Entry {
        int key;
        V   value;
    };

    HashMap() : entryCount(0) {}

    // TODO(Rabia): choose m = smallest prime >= 2 * expectedCount (at least 2),
    // create m empty buckets, entryCount = 0. Call before the first insert.
    // See: report Section 5.3   |   Done when: buckets().size() is prime and >= 2 * expectedCount
    void init(int expectedCount) { (void)expectedCount; }

    // TODO(Rabia): if `key` is already present return false (do NOT overwrite);
    // otherwise append {key, value} to bucket h(key) and return true.
    // See: report Section 5.3   |   Done when: duplicate insert detected
    bool insert(int key, const V& value) { (void)key; (void)value; return false; }

    // TODO(Rabia): walk bucket h(key); on a match copy the value into `out`, return true.
    // See: report Section 5.3   |   Done when: every inserted key is found with its value
    bool find(int key, V& out) const { (void)key; (void)out; return false; }

    // TODO(Rabia): true if `key` is present (can call find).
    // See: report Section 5.3   |   Done when: contains() agrees with find()
    bool contains(int key) const { (void)key; return false; }

    // TODO(Rabia): empty every bucket, entryCount = 0 (keep the bucket array).
    // See: report Section 5.3   |   Done when: size() == 0 and old keys not found
    void clear() {}

    int size() const        { return entryCount; }
    int bucketCount() const { return buckets.size(); }

private:
    // TODO(Rabia): h(k) = k mod m, made non-negative for negative k.
    // See: report Section 5.3   |   Done when: always returns 0..m-1
    int bucketOf(int key) const { (void)key; return 0; }

    DynamicArray<LinkedList<Entry>> buckets;
    int entryCount;
};

#endif // RESQGRID_HASH_MAP_H
