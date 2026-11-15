// File:       src/ui/GraphLayout.h
// Owner:      Sara
// Purpose:    Computes an (x, y) screen position for every location. The input
//             has no coordinates, so: detect a grid if the graph is one, else a
//             SEEDED force-directed layout, so the picture is identical on
//             every run.
// Depends on: Environment (read only), DynamicArray  (no SFML: testable without a window)
// Used by:    Renderer, UIApp
// Milestone:  M1 (on a hardcoded 5-location fake graph), M2 (loaded map)
#ifndef RESQGRID_GRAPH_LAYOUT_H
#define RESQGRID_GRAPH_LAYOUT_H

#include "../core/Types.h"
#include "../core/Environment.h"
#include "../containers/DynamicArray.h"

class GraphLayout {
public:
    // TODO(Sara): fill xs/ys for every location, inside [margin, width-margin] x
    // [margin, height-margin]. Try tryGrid() first; otherwise forceDirected(seed).
    // See: plan Section 6 (GraphLayout)   |   Done when: same picture on every run
    void compute(const Environment& env, float width, float height, unsigned seed = 42);

    float x(int loc) const { return xs[loc]; }
    float y(int loc) const { return ys[loc]; }
    int   count() const    { return xs.size(); }

private:
    // TODO(Sara): decide whether the graph is a grid (e.g. every location has
    // <= 4 neighbours and a rows x cols arrangement fits); if so place it and return true.
    // See: plan Section 6   |   Done when: a generated grid scenario shows as a grid
    bool tryGrid(const Environment& env, float width, float height);

    // TODO(Sara): spring/repulsion iterations starting from seeded random
    // positions (use nextRandom(), not rand(), so results never depend on the platform).
    // See: plan Section 6   |   Done when: same picture on every run
    void forceDirected(const Environment& env, float width, float height, unsigned seed);

    // TODO(Sara): small linear congruential generator returning [0, 1).
    // See: plan Section 6   |   Done when: same sequence for the same seed
    float nextRandom();

    DynamicArray<float> xs;
    DynamicArray<float> ys;
    unsigned rngState = 42;
};

#endif // RESQGRID_GRAPH_LAYOUT_H
