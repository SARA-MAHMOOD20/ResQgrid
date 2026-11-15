// File:       src/containers/DynamicArray.h
// Owner:      Farwa
// Purpose:    Growable array (our replacement for std::vector). Main storage
//             for locations, connections, robots, requests and events; every
//             entity is referred to by its index 0..n-1 (Report 5.2).
// Depends on: nothing
// Used by:    almost every other file
// Milestone:  M1 (merge by 16 Nov, everyone needs it)
//
// Rules: push_back is O(1) amortised (capacity doubles when full); operator[]
// is O(1). The array owns its memory: copy constructor and assignment make a
// deep copy, the destructor frees it. No bounds check in operator[] (callers
// use valid indices); add an assert while debugging if you like.
#ifndef RESQGRID_DYNAMIC_ARRAY_H
#define RESQGRID_DYNAMIC_ARRAY_H

template <typename T>
class DynamicArray {
public:
    DynamicArray() : data(nullptr), count(0), cap(0) {}

    // TODO(Farwa): allocate `initialCapacity` slots (size stays 0).
    // See: report Section 5.2   |   Done when: test pushes 10,000 ints and reads them back
    explicit DynamicArray(int initialCapacity) : data(nullptr), count(0), cap(0) {
        (void)initialCapacity;
    }

    // TODO(Farwa): deep copy of other's elements (new buffer, copy each element).
    // See: report Section 5.2   |   Done when: copying then changing the copy leaves the original unchanged
    DynamicArray(const DynamicArray& other) : data(nullptr), count(0), cap(0) {
        (void)other;
    }

    // TODO(Farwa): self-assignment check, free old buffer, deep copy, return *this.
    // See: report Section 5.2   |   Done when: a = a; and a = b; both work with no leak
    DynamicArray& operator=(const DynamicArray& other) {
        (void)other;
        return *this;
    }

    // TODO(Farwa): delete[] the buffer.
    // See: plan Section 7 "every new has a matching delete"   |   Done when: no leaks (valgrind / ASan)
    ~DynamicArray() {}

    // TODO(Farwa): if full, grow to max(1, 2*cap) (copy elements), then append.
    // See: report Section 5.2   |   Done when: test pushes 10,000 ints and reads them back
    void push_back(const T& value) { (void)value; }

    // TODO(Farwa): drop the last element (size - 1); return false if empty.
    // Both heaps need it to shrink after removing the root.
    // See: report Sections 5.5 and 5.6   |   Done when: size goes down by one; false on an empty array
    bool pop_back() { return false; }

    // TODO(Farwa): return data[index].
    // See: report Section 5.2   |   Done when: values read back equal values pushed
    T& operator[](int index) {
        (void)index;
        static T dummy{};       // stub only: remove when implemented
        return dummy;
    }
    const T& operator[](int index) const {
        (void)index;
        static T dummy{};       // stub only: remove when implemented
        return dummy;
    }

    // TODO(Farwa): make capacity at least newCap (never shrinks; size unchanged).
    // The loader calls this with the section count so no regrowth happens.
    // See: report Section 5.2   |   Done when: reserve(100) then 100 push_backs never reallocates
    void reserve(int newCap) { (void)newCap; }

    // TODO(Farwa): make size exactly n with every element equal to value
    // (Router uses it for dist[] / pred[]; Coordinator for counters).
    // See: report Section 6.1   |   Done when: assign(5, -1) gives five -1 values
    void assign(int n, const T& value) { (void)n; (void)value; }

    // TODO(Farwa): set size to 0 (keep or free the buffer, your choice: document it).
    // See: report Section 5.2   |   Done when: size() == 0 after clear()
    void clear() {}

    int  size() const     { return count; }
    int  capacity() const { return cap; }
    bool isEmpty() const  { return count == 0; }

private:
    T*  data;
    int count;  // number of elements in use
    int cap;    // number of slots allocated
};

#endif // RESQGRID_DYNAMIC_ARRAY_H
