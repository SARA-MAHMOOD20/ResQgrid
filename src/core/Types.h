// File:       src/core/Types.h
// Owner:      Shared (written together on Day 1, committed by Farwa)
// Purpose:    Every enum used by the simulator, their toString()/fromString()
//             helpers, shared constants, and the Str alias.
// Depends on: nothing (only <string>)
// Used by:    every module
// Milestone:  M0 (enums agreed) / M2 (toString + fromString implemented)
//
// TEAM DECISION (Day 1): std::string IS allowed. We write `using namespace std;`
// once, here, so nobody types std::. Everyone uses Str (never `string`
// directly) so we can swap in our own String class later by changing ONE line.
//
// All enums are `enum class` because Capability and Category share names
// (MEDICAL, FIRE, ...). Write Capability::MEDICAL, Category::MEDICAL.
#ifndef RESQGRID_TYPES_H
#define RESQGRID_TYPES_H

#include <string>

using namespace std;

using Str = string;

// ---------------------------------------------------------------------------
// Shared constants
// ---------------------------------------------------------------------------
const int NO_INDEX = -1;            // "no location / robot / request / connection"
const int INF_DIST = 1000000000;    // Dijkstra "infinity" (Report 6.1)
const int NUM_CAPABILITIES = 5;     // size of per-capability counters (Report 5.9)

// ---------------------------------------------------------------------------
// Enums (Report 9.2 lists the allowed spellings in the input file)
// ---------------------------------------------------------------------------

// LOCATIONS section, field `type`.
enum class LocationType { JUNCTION, BUILDING, HOSPITAL, DEPOT, CHARGING, COMMAND };

// ROBOTS section, field `capability`. GENERAL can serve any category.
// The numeric value (0..4) is used as an index into per-capability arrays.
enum class Capability { MEDICAL, FIRE, SUPPLY, SEARCH, GENERAL };

// REQUESTS section, field `category`. GENERAL is NOT a valid category
// (invalid_02, error E11).
enum class Category { MEDICAL, FIRE, SUPPLY, SEARCH };

// Robot state machine (Report 4.4).
enum class RobotState { AVAILABLE, TRAVELLING, PERFORMING, RETURNING, RECHARGING, OFFLINE };

// Request lifecycle (Report 4.3). A request that is not reported yet has no
// state of its own: see Request::reported.
enum class RequestState { PENDING, ASSIGNED, IN_PROGRESS, COMPLETED, FAILED };

// EVENTS section, field `TYPE` (Report 6.3).
enum class EventType { BLOCK, UNBLOCK, STATION_DOWN, STATION_UP, SET_URGENCY,
                       ROBOT_OFFLINE, ROBOT_ONLINE };

// CONNECTIONS section, field `status`.
enum class ConnStatus { OPEN, BLOCKED };

// Why a request ended FAILED (Report 4.3: "unreachable" or "not served").
enum class FailReason { NONE, UNREACHABLE, NOT_SERVED };

// ---------------------------------------------------------------------------
// toString / fromString
//   toString(x)        -> the exact spelling used in the input file and log.
//   fromString(s, out) -> true and sets `out` if `s` is a valid spelling,
//                         false otherwise (the loader reports the error).
//   Done when: every enum round-trips (fromString(toString(x)) == x).
// ---------------------------------------------------------------------------

// TODO(Farwa): return the file spelling of each value, e.g. "JUNCTION".
// See: report Section 9.2   |   Done when: every enum round-trips
inline Str toString(LocationType v) { (void)v; return "?"; }
inline Str toString(Capability v)   { (void)v; return "?"; }
inline Str toString(Category v)     { (void)v; return "?"; }
inline Str toString(RobotState v)   { (void)v; return "?"; }
inline Str toString(RequestState v) { (void)v; return "?"; }
inline Str toString(EventType v)    { (void)v; return "?"; }
inline Str toString(ConnStatus v)   { (void)v; return "?"; }
inline Str toString(FailReason v)   { (void)v; return "?"; }  // "none", "unreachable", "not_served"

// TODO(Farwa): compare `s` with each spelling; set `out` and return true on a match.
// See: report Section 9.2   |   Done when: every enum round-trips; "HOUSE", "PILOT" return false
inline bool fromString(const Str& s, LocationType& out) { (void)s; (void)out; return false; }
inline bool fromString(const Str& s, Capability& out)   { (void)s; (void)out; return false; }
inline bool fromString(const Str& s, Category& out)     { (void)s; (void)out; return false; }
inline bool fromString(const Str& s, RobotState& out)   { (void)s; (void)out; return false; }
inline bool fromString(const Str& s, RequestState& out) { (void)s; (void)out; return false; }
inline bool fromString(const Str& s, EventType& out)    { (void)s; (void)out; return false; }
inline bool fromString(const Str& s, ConnStatus& out)   { (void)s; (void)out; return false; }

#endif // RESQGRID_TYPES_H
