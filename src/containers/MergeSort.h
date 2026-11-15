// File:       src/containers/MergeSort.h
// Owner:      Farwa
// Purpose:    Stable merge sort on a DynamicArray. The loader sorts events and
//             requests by time; stability keeps same-time records in FILE
//             order without an extra sequence field (Report 5.7).
// Depends on: DynamicArray
// Used by:    ScenarioLoader (events, requests by time; robot indices by ID)
// Milestone:  M1
//
// Usage:
//     bool earlier(const Event& a, const Event& b) { return a.time < b.time; }
//     mergeSort(world.events, earlier);
// `less(a, b)` must return true only when a must come STRICTLY before b.
// When less(a,b) and less(b,a) are both false, a and b keep their order.
#ifndef RESQGRID_MERGE_SORT_H
#define RESQGRID_MERGE_SORT_H

#include "DynamicArray.h"

// TODO(Farwa): merge the sorted runs a[lo..mid) and a[mid..hi) into tmp, then
// copy back. Take from the LEFT run when the two are equal (that is what makes it stable).
// See: report Section 5.7   |   Done when: test equal keys keep their original order
template <typename T, typename Less>
void mergeRuns(DynamicArray<T>& a, DynamicArray<T>& tmp, int lo, int mid, int hi, Less less) {
    (void)a; (void)tmp; (void)lo; (void)mid; (void)hi; (void)less;
}

// TODO(Farwa): recursive (or bottom-up) merge sort of a[lo..hi) using mergeRuns.
// See: report Section 5.7   |   Done when: O(n log n); sorted output
template <typename T, typename Less>
void mergeSortRange(DynamicArray<T>& a, DynamicArray<T>& tmp, int lo, int hi, Less less) {
    (void)a; (void)tmp; (void)lo; (void)hi; (void)less;
}

// TODO(Farwa): allocate tmp once (same size as a), call mergeSortRange(a, tmp, 0, a.size(), less).
// See: report Section 5.7   |   Done when: test equal keys keep their original order
template <typename T, typename Less>
void mergeSort(DynamicArray<T>& a, Less less) {
    (void)a; (void)less;
}

#endif // RESQGRID_MERGE_SORT_H
