// File:       src/main.cpp
// Owner:      Farwa
// Purpose:    Entry point. Parses the command line, loads the scenario, then
//             runs the console simulation (log + final statistics). Keep it
//             small: all logic lives in modules.
// Depends on: Simulation, Logger, Statistics
// Used by:    the user / evaluator
// Milestone:  M3 (console)
//
// Usage: resqgrid <scenario_file> [--log <path>] [--quiet]
//   --log <path>  also write the log to <path> (e.g. output/run_log.txt)
//   --quiet       do not print log lines to the console (statistics still print)
// Exit codes: 0 ok, 1 bad arguments, 2 scenario file invalid, 3 cannot open log file.
#include <iostream>
#include <cstring>
#include "core/Types.h"
#include "core/Simulation.h"

static void printUsage(const char* prog) {
    cerr << "Usage: " << prog << " <scenario_file> [--log <path>] [--quiet]\n";
}

int main(int argc, char* argv[]) {
    const char* scenarioPath = nullptr;
    const char* logPath = nullptr;
    bool quiet = false;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--quiet") == 0) {
            quiet = true;
        } else if (strcmp(argv[i], "--log") == 0) {
            if (i + 1 >= argc) { cerr << "error: --log needs a path\n"; printUsage(argv[0]); return 1; }
            logPath = argv[++i];
        } else if (argv[i][0] == '-') {
            cerr << "error: unknown option " << argv[i] << "\n"; printUsage(argv[0]); return 1;
        } else if (scenarioPath == nullptr) {
            scenarioPath = argv[i];
        } else {
            cerr << "error: more than one scenario file given\n"; printUsage(argv[0]); return 1;
        }
    }
    if (scenarioPath == nullptr) { printUsage(argv[0]); return 1; }

    Simulation sim;
    sim.log().setConsole(!quiet);
    if (logPath != nullptr && !sim.log().openFile(logPath)) {
        // TODO(Farwa): works once Logger::openFile is implemented (stub returns false).
        cerr << "warning: cannot open log file " << logPath << " (Logger::openFile not implemented yet?)\n";
    }

    bool loaded = sim.load(scenarioPath);

    if (!loaded) {
        cerr << "Scenario file is invalid: " << sim.getLoader().errorCount() << " error(s)\n";
        sim.getLoader().printMessages(cerr);
        return 2;
    }
    sim.getLoader().printMessages(cerr);  // warnings only, at this point

    sim.runAll();
    sim.getStats().print(cout);
    // TODO(Farwa): also sim.getStats().writeToLog(sim.log()) once Statistics is real.
    return 0;
}
