// File:       src/core/Environment.cpp
// Owner:      Rabia
// Purpose:    Building the map and changing road / station status.
// Depends on: Environment.h (DynamicArray, LinkedList, Types)
// Used by:    see Environment.h
// Milestone:  M2
#include "Environment.h"

int Environment::addLocation(const Location& loc) {
    // TODO(Rabia): push location, push empty LinkedList<AdjEntry>, record charger index.
    // See: report Section 5.4   |   Done when: chargers holds exactly the CHARGING locations
    (void)loc;
    return NO_INDEX;
}

int Environment::addConnection(const Connection& conn) {
    // TODO(Rabia): push connection; add one AdjEntry at each end pointing to the same index.
    // See: report Section 5.4   |   Done when: blocking one road hides it both ways
    (void)conn;
    return NO_INDEX;
}

bool Environment::block(int connIndex) {
    // TODO(Rabia): OPEN -> BLOCKED, return true; already BLOCKED -> return false.
    // See: report Section 6.3   |   Done when: blocking one road hides it both ways
    (void)connIndex;
    return false;
}

bool Environment::unblock(int connIndex) {
    // TODO(Rabia): BLOCKED -> OPEN, ++networkVersion, return true; already OPEN -> return false.
    // See: report Sections 5.9 and 6.3   |   Done when: networkVersion grows only on real unblocks
    (void)connIndex;
    return false;
}

bool Environment::isOpen(int connIndex) const {
    // TODO(Rabia): return connections[connIndex].status == ConnStatus::OPEN.
    // See: report Section 6.1   |   Done when: Router never uses a BLOCKED road
    (void)connIndex;
    return false;
}

bool Environment::setStationOpen(int locIndex, bool open) {
    // TODO(Rabia): only for CHARGING locations; return false if unchanged.
    // See: report Section 6.3   |   Done when: redundant STATION_DOWN returns false
    (void)locIndex;
    (void)open;
    return false;
}

bool Environment::isOpenCharger(int locIndex) const {
    // TODO(Rabia): type == CHARGING && stationOpen.
    // See: report Section 4.5   |   Done when: down stations are skipped by nearestOpenCharger
    (void)locIndex;
    return false;
}

void Environment::clear() {
    // TODO(Rabia): clear all four arrays, networkVersion = 0.
    // See: report Section 5.4   |   Done when: locationCount() == 0 afterwards
}
