// File:       src/ui/GraphLayout.cpp
// Owner:      Sara
// Purpose:    Deterministic placement of locations on screen.
// Depends on: GraphLayout.h (Environment, DynamicArray)
// Used by:    Renderer, UIApp
// Milestone:  M1
#include "GraphLayout.h"

void GraphLayout::compute(const Environment& env, float width, float height, unsigned seed) {
    // TODO(Sara): xs/ys sized to env.locationCount(); grid, else force-directed.
    // See: plan Section 6   |   Done when: same picture on every run
    (void)env;
    (void)width;
    (void)height;
    rngState = seed;
}

bool GraphLayout::tryGrid(const Environment& env, float width, float height) {
    // TODO(Sara): see GraphLayout.h.
    // See: plan Section 6   |   Done when: grid scenarios look like grids
    (void)env;
    (void)width;
    (void)height;
    return false;
}

void GraphLayout::forceDirected(const Environment& env, float width, float height, unsigned seed) {
    // TODO(Sara): see GraphLayout.h.
    // See: plan Section 6   |   Done when: provided file is readable on screen
    (void)env;
    (void)width;
    (void)height;
    (void)seed;
}

float GraphLayout::nextRandom() {
    // TODO(Sara): rngState = rngState * 1664525u + 1013904223u; return (rngState >> 8) / 16777216.0f.
    // See: plan Section 6   |   Done when: same sequence for the same seed
    return 0.0f;
}
