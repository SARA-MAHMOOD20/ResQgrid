// File:       src/core/Coordinator.h
// Owner:      Sara
// Purpose:    Owns the pending-request priority queue and runs the assignment
//             phase (Report 6.2, Algorithm 2): picks the best robot for each
//             request in priority order, logs battery rejections, counts
//             conflicts.
// Depends on: World (Robot, Request, Environment), IndexedMaxHeap, Router, Logger
// Used by:    Simulation (EventProcessor / Rerouter use pending() through it)
// Milestone:  M3 (started in M2)
//
// Candidate (Report 6.2): AVAILABLE, canServe(category), reachable, and passes
// the battery feasibility rule (Report 4.5):
//     battery >= b_c * (d(robot, req) + d(req, nearest open charger)) + b_w * duration
//     (no open charger reachable -> reserve term is 0, warning logged; Report 11)
// Winner among candidates, in order:
//     1. smallest travel cost   2. specialist before GENERAL
//     3. higher battery         4. smaller robot ID
// A request with more than one candidate counts as one CONFLICT.
#ifndef RESQGRID_COORDINATOR_H
#define RESQGRID_COORDINATOR_H

#include "World.h"
#include "Router.h"
#include "../containers/DynamicArray.h"
#include "../containers/IndexedMaxHeap.h"
#include "../output/Logger.h"

class Coordinator {
public:
    Coordinator(World& world, Router& router, Logger& log);

    // TODO(Sara): pending.init(world.requests.size()). Call once after loading.
    // See: report Section 5.6   |   Done when: heap ready for every request index
    void init();

    // Phase 2 helper: request becomes PENDING and enters the heap with its key.
    // TODO(Sara): pending.insert(i, requests[i].priorityKey(A), requests[i].id).
    // See: report Sections 4.2 and 5.6   |   Done when: Scenario 03 t=0 assignment order matches
    void addPending(int requestIndex);

    // TODO(Sara): Algorithm 2 (Report 6.2):
    //   countFreeRobots(); deferred = empty
    //   while heap not empty and some robot AVAILABLE:
    //     r = extractMax
    //     if no free robot can serve r.category, or (r.unreachable and network
    //        version unchanged since r.unreachableVersion): defer, continue
    //     router.run(r.location)  (ONE Dijkstra, from the request)
    //     best = chooseRobot(r)   (logs each battery rejection)
    //     if best: assign, build route from pred (router.routeFrom), log ASSIGN
    //     else: mark unreachable if no capable robot had a route; defer
    //   put every deferred request back into the heap
    // No head-of-line blocking: an unservable request never stops the loop.
    // See: report Section 6.2 (Algorithm 2)   |   Done when: Scenario 03 t=0 assignment order matches
    void assignStep();

    IndexedMaxHeap&       pending()       { return pendingHeap; }
    const IndexedMaxHeap& pending() const { return pendingHeap; }

    int conflictCount() const         { return conflicts; }
    int batteryRejectionCount() const { return batteryRejections; }

private:
    // TODO(Sara): recount freeCount[cap] = number of AVAILABLE robots per capability.
    // See: report Section 5.9 (free-robot counters)   |   Done when: O(1) check before each Dijkstra
    void countFreeRobots();

    // TODO(Sara): true if some AVAILABLE robot has this capability or GENERAL.
    // See: report Section 5.9   |   Done when: SUPPLY request waits while only GENERAL robot is busy (Scenario 01)
    bool someoneCanServe(Category cat) const;

    // TODO(Sara): battery needed per the feasibility rule in Report 4.5.
    // See: report Section 4.5   |   Done when: Scenario 01 gives need=13 and need=15 for Robot 2
    int batteryNeeded(int distToRequest, int distToCharger, int duration) const;

    // TODO(Sara): scan robots, apply the candidate filter and tie-breaks, log
    // each REJECT, count a conflict if > 1 candidate. Return robot index or NO_INDEX.
    // Set `anyCapableReachable` so assignStep knows whether to mark unreachable.
    // See: report Section 6.2   |   Done when: Scenario 03 tie-breaks match
    int chooseRobot(int requestIndex, bool& anyCapableReachable);

    // TODO(Sara): true if robot a beats robot b (rules 1-4 above), given their distances.
    // See: report Section 6.2   |   Done when: Scenario 03 "two robots at equal distance" matches
    bool better(int robotA, int distA, int robotB, int distB) const;

    // TODO(Sara): link both sides, request -> ASSIGNED, robot -> TRAVELLING
    // (a zero-length route still goes through TRAVELLING; it starts work next step,
    // Report 4.1), fill the route, decrement freeCount, log.
    // See: report Sections 4.3, 4.4 and 6.2   |   Done when: Scenario 01 t=0 lines match
    void assign(int requestIndex, int robotIndex);

    World&         world;
    Router&        router;
    Logger&        log;
    IndexedMaxHeap pendingHeap;
    int freeCount[NUM_CAPABILITIES] = {0, 0, 0, 0, 0};
    int conflicts = 0;
    int batteryRejections = 0;
};

#endif // RESQGRID_COORDINATOR_H
