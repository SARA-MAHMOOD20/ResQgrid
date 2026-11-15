// File:       src/core/Environment.h
// Owner:      Rabia
// Purpose:    The disaster map: locations, connections (roads), the adjacency
//             list, charging-station open flags and the network version
//             counter (Report 5.4, 5.9).
// Depends on: DynamicArray, LinkedList, Types
// Used by:    World (World::env), Router, ScenarioLoader, EventProcessor,
//             Rerouter, Movement, GraphLayout, Renderer
// Milestone:  M2
//
// Key idea (Report 5.4): each road is ONE Connection record. Both ends get an
// AdjEntry that points to the SAME record, so blocking a road changes one flag
// and both directions see it. Parallel roads are simply two records.
// Everything here uses internal INDICES (0..n-1), never file IDs.
#ifndef RESQGRID_ENVIRONMENT_H
#define RESQGRID_ENVIRONMENT_H

#include "Types.h"
#include "../containers/DynamicArray.h"
#include "../containers/LinkedList.h"

struct Location {
    int          id = 0;                        // ID from the file
    Str          name;
    LocationType type = LocationType::JUNCTION;
    bool         stationOpen = true;            // only meaningful for CHARGING
};

struct Connection {
    int        id = 0;                          // ID from the file
    int        a = NO_INDEX;                    // location index of one end
    int        b = NO_INDEX;                    // location index of the other end
    int        cost = 0;                        // > 0, in cost units
    ConnStatus status = ConnStatus::OPEN;

    // Given one end, return the other end's index.
    int other(int from) const { return from == a ? b : a; }
};

// One entry in a location's adjacency list.
struct AdjEntry {
    int neighbour = NO_INDEX;   // location index at the other end
    int conn = NO_INDEX;        // index into Environment::connections
};

class Environment {
public:
    DynamicArray<Location>             locations;
    DynamicArray<Connection>           connections;
    DynamicArray<LinkedList<AdjEntry>> adjacency;   // adjacency[locIndex]
    DynamicArray<int>                  chargers;    // indices of CHARGING locations
    int networkVersion = 0;                         // +1 on every UNBLOCK (Report 5.9)

    // TODO(Rabia): append the location, append an empty adjacency list for it,
    // and if it is CHARGING add its index to `chargers`. Return its index.
    // See: report Section 5.4   |   Done when: index == locations.size()-1 afterwards
    int addLocation(const Location& loc);

    // TODO(Rabia): append the connection (a, b already converted to indices) and
    // add AdjEntry{b, c} to adjacency[a] and AdjEntry{a, c} to adjacency[b].
    // Return its index.
    // See: report Section 5.4   |   Done when: the road appears in both endpoints' lists
    int addConnection(const Connection& conn);

    // TODO(Rabia): set status BLOCKED. Return false (no change) if it was already
    // blocked: the caller logs the redundant-event warning.
    // See: report Sections 5.4 and 6.3   |   Done when: blocking one road hides it both ways
    bool block(int connIndex);

    // TODO(Rabia): set status OPEN and increment networkVersion. Return false
    // (no change, no version bump) if it was already open.
    // See: report Sections 5.9 and 6.3   |   Done when: unreachable requests are retried after this
    bool unblock(int connIndex);

    // TODO(Rabia): status == OPEN.
    // See: report Section 6.1   |   Done when: Router skips blocked roads
    bool isOpen(int connIndex) const;

    // TODO(Rabia): set stationOpen on a CHARGING location. Return false if the
    // location is not a charger or the flag already had that value.
    // See: report Section 6.3 (STATION_DOWN/UP)   |   Done when: redundant STATION_DOWN returns false
    bool setStationOpen(int locIndex, bool open);

    // TODO(Rabia): type == CHARGING and stationOpen.
    // See: report Section 4.5   |   Done when: a down station is never chosen as nearest charger
    bool isOpenCharger(int locIndex) const;

    // TODO(Rabia): empty every array, networkVersion = 0.
    // See: report Section 5.4   |   Done when: a second load starts from an empty map
    void clear();

    int locationCount() const   { return locations.size(); }
    int connectionCount() const { return connections.size(); }
    const LinkedList<AdjEntry>& neighbours(int locIndex) const { return adjacency[locIndex]; }
};

#endif // RESQGRID_ENVIRONMENT_H
