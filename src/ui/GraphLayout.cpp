// File:       src/ui/GraphLayout.cpp
// Owner:      Sara
// Purpose:    Deterministic placement of locations on screen.
// Depends on: GraphLayout.h (Environment, DynamicArray), <cmath> for sqrt
// Used by:    Renderer, UIApp
// Milestone:  M1
#include "GraphLayout.h"
#include <cmath>

void GraphLayout::compute(const Environment& env, unsigned seed) {
    int n = env.locationCount();
    xs.assign(n, 0.5f);
    ys.assign(n, 0.5f);
    grid = false;
    cols = 0;
    rows = 0;

    if (n <= 1) {
        return;                         // one location: middle of the map
    }
    if (tryGrid(env)) {
        grid = true;
        return;
    }
    forceDirected(env, seed);
    normalise();
}

bool GraphLayout::tryGrid(const Environment& env) {
    int n = env.locationCount();
    int m = env.connectionCount();
    if (n < 16 || m == 0) {
        return false;                   // small maps look better as springs
    }

    // Find the number of columns that explains the most roads.
    int bestCols = 0;
    int bestFit = 0;
    for (int cols = 3; cols <= n / 3; ++cols) {
        int fit = 0;
        for (int c = 0; c < m; ++c) {
            int low = env.connections[c].a;
            int high = env.connections[c].b;
            if (low > high) {
                int tmp = low;
                low = high;
                high = tmp;
            }
            int diff = high - low;
            bool sameRow = (low / cols == high / cols);
            if ((diff == 1 && sameRow) || diff == cols) {
                fit++;
            }
        }
        if (fit > bestFit) {
            bestFit = fit;
            bestCols = cols;
        }
    }

    // At least 60% of the roads must be grid roads (the rest are shortcuts).
    if (bestFit * 10 < m * 6) {
        return false;
    }

    cols = bestCols;
    rows = (n + bestCols - 1) / bestCols;
    for (int i = 0; i < n; ++i) {
        int col = i % bestCols;
        int row = i / bestCols;
        xs[i] = (float)col / (float)(bestCols - 1);
        ys[i] = (rows > 1) ? (float)row / (float)(rows - 1) : 0.5f;
    }
    return true;
}

void GraphLayout::forceDirected(const Environment& env, unsigned seed) {
    int n = env.locationCount();
    int m = env.connectionCount();
    rngState = seed;

    // Random start positions (same every run because the seed is fixed).
    for (int i = 0; i < n; ++i) {
        xs[i] = nextRandom();
        ys[i] = nextRandom();
    }

    float k = sqrt(1.0f / (float)n);    // ideal distance between locations
    int rounds = (n > 300) ? 100 : 300; // big maps: fewer rounds (n*n work each)
    float temperature = 0.1f;           // biggest move allowed in one round

    DynamicArray<float> moveX;
    DynamicArray<float> moveY;

    for (int round = 0; round < rounds; ++round) {
        moveX.assign(n, 0.0f);
        moveY.assign(n, 0.0f);

        // 1. Every pair pushes apart: force = k*k / distance.
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                float dx = xs[i] - xs[j];
                float dy = ys[i] - ys[j];
                float dist = sqrt(dx * dx + dy * dy);
                if (dist < 0.0001f) {
                    // Same spot: nudge them apart in a fixed direction.
                    dx = 0.001f * (float)(i - j);
                    dy = 0.001f;
                    dist = sqrt(dx * dx + dy * dy);
                }
                float force = k * k / dist;
                moveX[i] += dx / dist * force;
                moveY[i] += dy / dist * force;
                moveX[j] -= dx / dist * force;
                moveY[j] -= dy / dist * force;
            }
        }

        // 2. Every road pulls its ends together: force = distance^2 / k.
        for (int c = 0; c < m; ++c) {
            int a = env.connections[c].a;
            int b = env.connections[c].b;
            float dx = xs[a] - xs[b];
            float dy = ys[a] - ys[b];
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < 0.0001f) {
                continue;
            }
            float force = dist * dist / k;
            moveX[a] -= dx / dist * force;
            moveY[a] -= dy / dist * force;
            moveX[b] += dx / dist * force;
            moveY[b] += dy / dist * force;
        }

        // 3. Move each location, but never further than the temperature.
        for (int i = 0; i < n; ++i) {
            float len = sqrt(moveX[i] * moveX[i] + moveY[i] * moveY[i]);
            if (len < 0.000001f) {
                continue;
            }
            float step = (len < temperature) ? len : temperature;
            xs[i] += moveX[i] / len * step;
            ys[i] += moveY[i] / len * step;
        }

        // Cool down a little every round.
        temperature = 0.1f * (1.0f - (float)(round + 1) / (float)rounds);
        if (temperature < 0.001f) {
            temperature = 0.001f;
        }
    }
}

void GraphLayout::normalise() {
    int n = xs.size();
    float minX = xs[0], maxX = xs[0], minY = ys[0], maxY = ys[0];
    for (int i = 1; i < n; ++i) {
        if (xs[i] < minX) minX = xs[i];
        if (xs[i] > maxX) maxX = xs[i];
        if (ys[i] < minY) minY = ys[i];
        if (ys[i] > maxY) maxY = ys[i];
    }
    float width = maxX - minX;
    float height = maxY - minY;
    for (int i = 0; i < n; ++i) {
        xs[i] = (width > 0.000001f) ? (xs[i] - minX) / width : 0.5f;
        ys[i] = (height > 0.000001f) ? (ys[i] - minY) / height : 0.5f;
    }
}

float GraphLayout::nextRandom() {
    // Linear congruential generator (same constants as Numerical Recipes).
    rngState = rngState * 1664525u + 1013904223u;
    return (float)(rngState >> 8) / 16777216.0f;     // top 24 bits -> 0..1
}
