// File:       src/ui/GraphLayout.h
// Owner:      Sara
// Purpose:    Computes a screen position for every location. The input has
//             no coordinates, so: if the map is a grid (the provided and
//             stress scenarios are), place it as a grid; otherwise use a
//             SEEDED force-directed layout, so the picture is identical on
//             every run.
// Depends on: Environment (read only), DynamicArray  (no SFML: testable without a window)
// Used by:    Renderer, UIApp
// Milestone:  M1 (fake graph), M2 (loaded map)
//
// Positions are NORMALISED: x and y are between 0 and 1. The Renderer scales
// them to the map area of the window, so resizing needs no recomputation.
// All connections are used (OPEN or BLOCKED), so the picture never jumps when
// a road is blocked.
#ifndef RESQGRID_GRAPH_LAYOUT_H
#define RESQGRID_GRAPH_LAYOUT_H

#include "../core/Types.h"
#include "../core/Environment.h"
#include "../containers/DynamicArray.h"

class GraphLayout {
public:
    // Fills a position for every location of `env`.
    void compute(const Environment& env, unsigned seed = 42);

    float x(int loc) const { return xs[loc]; }
    float y(int loc) const { return ys[loc]; }
    int   count() const    { return xs.size(); }
    bool  isGrid() const   { return grid; }
    int   gridCols() const { return cols; }     // only meaningful when isGrid()
    int   gridRows() const { return rows; }

private:
    // Grid test: locations are numbered row by row (file order), so a road
    // between neighbours in a row joins index i and i+1, and a road between
    // rows joins i and i+cols. If at least 60% of roads fit one value of
    // `cols`, it is a grid. Only tried for 16+ locations.
    bool tryGrid(const Environment& env);

    // Simple spring layout: every pair of locations pushes apart, every road
    // pulls its two ends together; moves get smaller each round ("cooling").
    void forceDirected(const Environment& env, unsigned seed);

    // Stretch the positions so they use the whole 0..1 range on both axes.
    void normalise();

    // Our own random numbers (same sequence on every computer). Returns 0..1.
    float nextRandom();

    DynamicArray<float> xs;
    DynamicArray<float> ys;
    unsigned rngState = 42;
    bool     grid = false;
    int      cols = 0;
    int      rows = 0;
};

#endif // RESQGRID_GRAPH_LAYOUT_H
