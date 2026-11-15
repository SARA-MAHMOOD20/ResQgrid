// File:       src/main.cpp
// Owner:      Farwa (Sara adds the --gui branch)
// Purpose:    Entry point. Parses the command line, loads the scenario, then
//             either runs the console simulation (log + final statistics) or
//             opens the SFML window. Keep it small: all logic lives in modules.
// Depends on: Simulation, Logger, Statistics, UIApp (only if built with GUI)
// Used by:    the user / evaluator
// Milestone:  M3 (console), M4 (--gui)
//
// Usage: resqgrid <scenario_file> [--gui] [--log <path>] [--quiet]
//   --gui         open the SFML window instead of running in the console
//   --log <path>  also write the log to <path> (e.g. output/run_log.txt)
//   --quiet       do not print log lines to the console (statistics still print)
// Exit codes: 0 ok, 1 bad arguments, 2 scenario file invalid, 3 cannot open log file.
#include <iostream>
#include <cstring>
#include "core/Types.h"
#include "core/Simulation.h"
#ifdef RESQGRID_HAS_GUI
#include "ui/UIApp.h"
#endif

static void printUsage(const char* prog) {
    cerr << "Usage: " << prog << " <scenario_file> [--gui] [--log <path>] [--quiet]\n";
}

int main(int argc, char* argv[]) {
    const char* scenarioPath = nullptr;
    const char* logPath = nullptr;
    bool gui = false;
    bool quiet = false;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--gui") == 0) {
            gui = true;
        } else if (strcmp(argv[i], "--quiet") == 0) {
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
    sim.log().setConsole(!quiet && !gui);
    if (logPath != nullptr && !sim.log().openFile(logPath)) {
        // TODO(Farwa): works once Logger::openFile is implemented (stub returns false).
        cerr << "warning: cannot open log file " << logPath << " (Logger::openFile not implemented yet?)\n";
    }

    bool loaded = sim.load(scenarioPath);

    if (gui) {
#ifdef RESQGRID_HAS_GUI
        UIApp app(sim, loaded);          // shows the error screen itself if !loaded
        return app.run();
#else
        cerr << "error: this build has no GUI (SFML 3 was not found when configuring CMake)\n";
        return 1;
#endif
    }

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
