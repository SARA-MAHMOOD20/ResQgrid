// File:       src/core/EventProcessor.cpp
// Owner:      Rabia
// Purpose:    Applying dynamic events (phase 1).
// Depends on: EventProcessor.h (World, IndexedMaxHeap, Logger)
// Used by:    Simulation
// Milestone:  M3
#include "EventProcessor.h"

EventProcessor::EventProcessor(World& w, IndexedMaxHeap& p, Logger& l)
    : world(w), pending(p), log(l) {}

bool EventProcessor::applyEventsAt(int t) {
    // TODO(Rabia): loop over events with time == t starting at world.nextEvent; switch on type.
    // See: report Sections 4.2 and 6.3   |   Done when: Scenario 04 events logged in the right order
    (void)t;
    world.networkChangedThisStep = false;
    return false;
}

void EventProcessor::applyBlock(const Event& e) {
    // TODO(Rabia): env.block(e.refIndex); log EVENT BLOCK or WARN if redundant.
    // See: report Section 6.3   |   Done when: Scenario 01 t=4 "EVENT BLOCK connection=4 (2-5)"
    (void)e;
}

void EventProcessor::applyUnblock(const Event& e) {
    // TODO(Rabia): env.unblock(e.refIndex) (bumps networkVersion); log or warn.
    // See: report Sections 5.9 and 6.3   |   Done when: Scenario 02 unreachable request is retried afterwards
    (void)e;
}

void EventProcessor::applyStationDown(const Event& e) {
    // TODO(Rabia): env.setStationOpen(e.refIndex, false); log or warn.
    // See: report Sections 4.7 and 6.3   |   Done when: robots recharging there are redirected in phase 3
    (void)e;
}

void EventProcessor::applyStationUp(const Event& e) {
    // TODO(Rabia): env.setStationOpen(e.refIndex, true); log or warn.
    // See: report Section 6.3   |   Done when: the station is a valid nearest charger again
    (void)e;
}

void EventProcessor::applySetUrgency(const Event& e) {
    // TODO(Rabia): see the rules in EventProcessor.h; use Request::priorityKey for changeKey.
    // See: report Sections 5.6 and 6.3   |   Done when: provided file t=75 raises request 21
    (void)e;
}

void EventProcessor::applyRobotOffline(const Event& e) {
    // TODO(Rabia): robot -> OFFLINE (leave the road? decide with Sara and document);
    // release its request back to PENDING with remainingWork kept; ++reassignments.
    // See: report Sections 4.3 and 6.3   |   Done when: Scenario 04 "robot offline while working" matches
    (void)e;
}

void EventProcessor::applyRobotOnline(const Event& e) {
    // TODO(Rabia): OFFLINE -> AVAILABLE; warning if it was not offline.
    // See: report Section 6.3   |   Done when: robot is a candidate again in the same step's assignment
    (void)e;
}
