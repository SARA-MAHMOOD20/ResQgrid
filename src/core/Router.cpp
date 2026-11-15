// File:       src/core/Router.cpp
// Owner:      Rabia
// Purpose:    Dijkstra over open connections and route reconstruction.
// Depends on: Router.h (Environment, MinHeap, Queue, DynamicArray)
// Used by:    see Router.h
// Milestone:  M2
#include "Router.h"

Router::Router(const Environment& environment) : env(environment) {}

void Router::run(int source) {
    // TODO(Rabia): Algorithm 1 with lazy deletion (skip popped entries with d > dist[u]).
    // See: report Section 6.1   |   Done when: Scenario 01 key costs all match
    src = source;
    ++runCount;
}

int Router::distanceTo(int loc) const {
    // TODO(Rabia): return dist[loc] (INF_DIST before any run).
    // See: report Section 6.1   |   Done when: Scenario 01 key costs all match
    (void)loc;
    return INF_DIST;
}

bool Router::reachable(int loc) const {
    // TODO(Rabia): distanceTo(loc) < INF_DIST.
    // See: report Section 4.6   |   Done when: Scenario 02 cut-off requests are unreachable
    (void)loc;
    return false;
}

bool Router::routeFrom(int start, Queue<int>& out) const {
    // TODO(Rabia): out.clear(); walk predConn from start until src, enqueue each connection.
    // See: report Section 6.1   |   Done when: Scenario 01 Robot 1 route is 1-3-2-5
    (void)start;
    (void)out;
    return false;
}

bool Router::routeTo(int target, Queue<int>& out) const {
    // TODO(Rabia): collect connections from target back to src, enqueue them reversed.
    // See: report Sections 4.5 and 4.7   |   Done when: Scenario 01 Robot 2 route is 8-6-5-7
    (void)target;
    (void)out;
    return false;
}

int Router::nearestOpenCharger() const {
    // TODO(Rabia): scan env.chargers, keep the open one with the smallest dist.
    // See: report Section 4.5   |   Done when: down stations are never returned
    return NO_INDEX;
}

Str Router::routeToString(int startLoc, const Queue<int>& route) const {
    // TODO(Rabia): start at startLoc, for each connection step to conn.other(cur), append "-<id>".
    // See: report Section 9.5   |   Done when: prints "2-4-7-5" for the Scenario 01 reroute
    (void)startLoc;
    (void)route;
    return "";
}
