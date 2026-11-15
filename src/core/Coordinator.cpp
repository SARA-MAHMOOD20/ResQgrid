// File:       src/core/Coordinator.cpp
// Owner:      Sara
// Purpose:    Assignment phase (Algorithm 2) and the pending heap.
// Depends on: Coordinator.h (World, IndexedMaxHeap, Router, Logger)
// Used by:    Simulation
// Milestone:  M3
#include "Coordinator.h"

Coordinator::Coordinator(World& w, Router& r, Logger& l) : world(w), router(r), log(l) {}

void Coordinator::init() {
    // TODO(Sara): pendingHeap.init(world.requests.size()); reset counters.
    // See: report Section 5.6   |   Done when: contains(i) false for every request
}

void Coordinator::addPending(int requestIndex) {
    // TODO(Sara): insert with priorityKey(world.params.agingInterval) and the request ID as tie.
    // See: report Section 5.6   |   Done when: equal key -> smaller ID first
    (void)requestIndex;
}

void Coordinator::assignStep() {
    // TODO(Sara): Algorithm 2, see Coordinator.h.
    // See: report Section 6.2   |   Done when: Scenario 03 t=0 assignment order matches
}

void Coordinator::countFreeRobots() {
    // TODO(Sara): zero freeCount, add 1 per AVAILABLE robot at index (int)capability.
    // See: report Section 5.9   |   Done when: counts match the robot panel in the UI
}

bool Coordinator::someoneCanServe(Category cat) const {
    // TODO(Sara): freeCount[(int)cat] > 0 || freeCount[(int)Capability::GENERAL] > 0.
    // See: report Section 5.9   |   Done when: Scenario 01 Requests 3 and 4 wait for Robot 2
    (void)cat;
    return false;
}

int Coordinator::batteryNeeded(int distToRequest, int distToCharger, int duration) const {
    // TODO(Sara): b_c * (distToRequest + distToCharger) + b_w * duration (Report 4.5).
    // See: report Section 4.5   |   Done when: Scenario 01: 13 for Req 2, 15 for Req 1 (Robot 2)
    (void)distToRequest;
    (void)distToCharger;
    (void)duration;
    return 0;
}

int Coordinator::chooseRobot(int requestIndex, bool& anyCapableReachable) {
    // TODO(Sara): candidate filter + better(); log REJECT for battery failures.
    // See: report Section 6.2   |   Done when: Scenario 03 tie-breaks match
    (void)requestIndex;
    anyCapableReachable = false;
    return NO_INDEX;
}

bool Coordinator::better(int robotA, int distA, int robotB, int distB) const {
    // TODO(Sara): cost, then specialist, then battery, then ID.
    // See: report Section 6.2   |   Done when: Scenario 03 matches
    (void)robotA;
    (void)distA;
    (void)robotB;
    (void)distB;
    return false;
}

void Coordinator::assign(int requestIndex, int robotIndex) {
    // TODO(Sara): see Coordinator.h.
    // See: report Section 6.2   |   Done when: "t=0 ASSIGN request=2 robot=3 route=7-5-6 cost=6"
    (void)requestIndex;
    (void)robotIndex;
}
