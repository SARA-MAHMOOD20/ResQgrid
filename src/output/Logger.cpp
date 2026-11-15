// File:       src/output/Logger.cpp
// Owner:      Farwa
// Purpose:    Formatting and writing log lines; ring buffer for the UI.
// Depends on: Logger.h (Types)
// Used by:    see Logger.h
// Milestone:  M2
#include "Logger.h"
#include <iostream>

Logger::Logger()
    : toConsole(true), toFile(false), currentTime(0), recentStart(0), recentSize(0) {}

Logger::~Logger() {
    // TODO(Farwa): close the file if it is open.
    // See: plan Section 7 (resources freed)   |   Done when: the log file is complete after exit
}

bool Logger::openFile(const Str& path) {
    // TODO(Farwa): file.open(path); toFile = file.is_open(); return toFile.
    // See: report Section 9.5   |   Done when: --log output/x.txt creates the file
    (void)path;
    return false;
}

void Logger::write(const Str& body) {
    // Day-1 stub from the plan ("until Logger is real, it prints the line to the console").
    // TODO(Farwa): also write to the file when toFile, and call remember(line).
    // See: report Section 9.5   |   Done when: Scenario 01 log looks like Report 9.5
    if (toConsole) {
        cout << "t=" << currentTime << " " << body << "\n";
    }
}

// Each helper builds the body string and calls write(). Use to_string() for ints
// and toString() (Types.h) for enums. Formats are in the README "Log format".

void Logger::assign(int requestId, int robotId, const Str& route, int cost) {
    // TODO(Farwa): "ASSIGN request=<r> robot=<b> route=<route> cost=<c>"
    // See: report Section 9.5   |   Done when: Scenario 01 t=0 lines match
    (void)requestId; (void)robotId; (void)route; (void)cost;
}

void Logger::reject(int requestId, int robotId, int need, int have) {
    // TODO(Farwa): "REJECT request=<r> robot=<b> reason=battery need=<n> have=<h>"
    // See: report Sections 4.5 and 9.5   |   Done when: Scenario 01 gives 2 rejections
    (void)requestId; (void)robotId; (void)need; (void)have;
}

void Logger::robotState(int robotId, RobotState from, RobotState to, int targetLocId) {
    // TODO(Farwa): "STATE robot=<b> <FROM>-><TO>" plus " target=<loc>" when targetLocId != NO_INDEX
    // See: report Section 9.5   |   Done when: "t=0 STATE robot=2 AVAILABLE->RETURNING target=7"
    (void)robotId; (void)from; (void)to; (void)targetLocId;
}

void Logger::requestState(int requestId, RequestState from, RequestState to) {
    // TODO(Farwa): "STATE request=<r> <FROM>-><TO>"
    // See: report Section 4.3   |   Done when: every request transition appears once
    (void)requestId; (void)from; (void)to;
}

void Logger::newRequest(int requestId, int locationId, Category cat, int urgency) {
    // TODO(Farwa): "REQUEST request=<r> at=<loc> category=<CAT> urgency=<u>"
    // See: report Section 4.2 (phase 2)   |   Done when: one line per request at its report time
    (void)requestId; (void)locationId; (void)cat; (void)urgency;
}

void Logger::event(EventType type, int refId, const Str& detail) {
    // TODO(Farwa): "EVENT <TYPE> <ref-name>=<id> <detail>", e.g. "EVENT BLOCK connection=4 (2-5)"
    // See: report Section 9.5   |   Done when: Scenario 04 events logged in the right order
    (void)type; (void)refId; (void)detail;
}

void Logger::reroute(int robotId, int onConnId, int backToLocId, const Str& route, int cost) {
    // TODO(Farwa): "REROUTE robot=<b> on=<conn> back_to=<loc> route=<route> cost=<c>"
    // (omit on= / back_to= when the robot was not on the blocked road: pass NO_INDEX)
    // See: report Sections 4.7 and 9.5   |   Done when: "t=4 REROUTE robot=1 on=4 back_to=2 route=2-4-7-5 cost=9"
    (void)robotId; (void)onConnId; (void)backToLocId; (void)route; (void)cost;
}

void Logger::release(int requestId, int robotId, const Str& reason) {
    // TODO(Farwa): "RELEASE request=<r> robot=<b> reason=<offline|no_route>"
    // See: report Sections 4.3 and 4.7   |   Done when: counted as a reassignment in Statistics
    (void)requestId; (void)robotId; (void)reason;
}

void Logger::arrive(int robotId, int locationId) {
    // TODO(Farwa): "ARRIVE robot=<b> at=<loc>"
    // See: report Section 4.1   |   Done when: Scenario 01 Robot 3 arrives at 6 at t=5
    (void)robotId; (void)locationId;
}

void Logger::complete(int requestId, int robotId, int response) {
    // TODO(Farwa): "COMPLETE request=<r> robot=<b> response=<steps>"
    // See: report Section 9.5   |   Done when: "t=7 COMPLETE request=2 robot=3 response=7"
    (void)requestId; (void)robotId; (void)response;
}

void Logger::fail(int requestId, FailReason reason) {
    // TODO(Farwa): "FAIL request=<r> reason=<unreachable|not_served>"
    // See: report Section 4.3   |   Done when: every unfinished request gets one FAIL line at the end
    (void)requestId; (void)reason;
}

void Logger::warning(const Str& message) {
    // TODO(Farwa): "WARN <message>"
    // See: report Section 9.4 (warnings)   |   Done when: redundant events print a WARN line
    (void)message;
}

void Logger::invalidTransition(const Str& what, int id, const Str& from, const Str& to) {
    // TODO(Farwa): "INVALID <what>=<id> <from>-><to>"   (what = "robot" or "request")
    // See: report Sections 4.3 and 4.4   |   Done when: rejected transitions are visible in the log
    (void)what; (void)id; (void)from; (void)to;
}

const Str& Logger::recent(int i) const {
    // TODO(Farwa): return recentLines[(recentStart + i) % RECENT_CAPACITY].
    // See: plan Section 6 (Logger keeps the last N lines)   |   Done when: UI shows the newest lines
    (void)i;
    static const Str empty;
    return empty;
}

void Logger::remember(const Str& line) {
    // TODO(Farwa): append to the ring buffer; overwrite the oldest when full.
    // See: plan Section 6   |   Done when: after 250 lines, recent(0) is line 51
    (void)line;
}
