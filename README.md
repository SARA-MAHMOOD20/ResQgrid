<!--
File:       README.md
Owner:      Farwa
Purpose:    How to build, run and contribute to ResQGrid; who owns what.
Depends on: docs/report.pdf, docs/ResQGrid_Development_Plan.pdf, docs/project_spec.pdf
Used by:    everyone, and the evaluator ("clean clone builds and runs from README alone")
Milestone:  M0 (skeleton) -> M5 (final build/run/format sections)
-->

# ResQGrid: AI Rescue Robots

ResQGrid (ResQ = rescue, Grid = the city graph) is a C++17 rescue-coordination simulator for our Data
Structures final project (team: Sara, Rabia, Farwa). It loads a disaster map from a text file:
locations, roads (some blocked), rescue robots, rescue requests and timed events. Then it advances
the world one discrete time step at a time. Every step it applies events, admits new requests,
reroutes robots whose roads closed, assigns waiting requests to robots, moves the robots, and
updates their battery. It writes a structured log line for every state change and prints final
statistics at the end. All data structures are our own: **no STL containers**.

The design is final and is described in **docs/report.pdf**. This repository is a **skeleton**:
every class and function is declared, but real logic is marked `TODO(<Owner>)`. The skeleton
compiles and runs on day 1.

### Features
- Loads any scenario in the spec format and validates it, reporting **every** error with its line number.
- Map stored as an adjacency list with shared connection records: blocking a road takes effect in both directions at once.
- Dijkstra (binary min-heap, lazy deletion) over OPEN roads only, run **from the request** so one pass covers all robots.
- Pending requests in an **indexed max-heap** with an aging key (no starvation) and change-key for `SET_URGENCY`.
- Deterministic assignment with documented tie-breaks, battery feasibility with a charger reserve, and conflict counting.
- Battery use for moving and working, recharging at open stations, and a low-battery return rule.
- All 7 event types (`BLOCK`, `UNBLOCK`, `STATION_DOWN`, `STATION_UP`, `SET_URGENCY`, `ROBOT_OFFLINE`, `ROBOT_ONLINE`).
- Rerouting policy for robots on, or heading towards, a blocked road or a down charging station.
- Plain-text, one-line-per-change log (console and/or file) plus final statistics.
- Optional SFML 3 window: map, moving robots, step / play / pause / speed controls, panels, final-stats screen and error screen.

---

## Team decisions (Day 1)

| Question | Decision |
|---|---|
| `std::string` allowed? | **Yes.** `Types.h` has `using namespace std;` (don't write `std::`) and `using Str = string;`. Always write `Str`, so we can swap in our own String class later by editing one line. |
| SFML version | **SFML 3.0** (3.0.2 tested). Use only the 3.0 API (`openFromFile`, `pollEvent()` returning an optional, `sf::VideoMode({w, h})`). |
| Log format | Plain text, one line per change, `t=<step> <KIND> key=value ...` (see [Log format](#log-format)). |
| Allowed std headers | `<iostream>`, `<fstream>`, `<string>`, `<cstring>`, `<cstdlib>`, `<chrono>`. SFML may use STL internally; that's fine. |

---

## Build and run

### Linux (Ubuntu / Debian)
```bash
sudo apt install build-essential cmake libsfml-dev     # Ubuntu 26.04 ships SFML 3.0.x
cmake -S . -B build
cmake --build build -j
ctest --test-dir build              # runs the 4 test programs
./build/resqgrid data/scenario_provided.txt
```
If your distribution only ships SFML 2.x, build SFML 3.0 from source (github.com/SFML/SFML, tag
`3.0.2`) and pass `-DSFML_DIR=<install>/lib/cmake/SFML`. You can also configure with
`-DRESQGRID_GUI=OFF` to build the console program and tests without SFML.

### Windows
1. Install Visual Studio 2022 (Desktop development with C++), which includes CMake.
2. Download **SFML 3.0.x** for your exact compiler from sfml-dev.org (for example the Visual C++ 17 (2022) 64-bit build) and unzip it, e.g. to `C:\SFML-3.0.2`.
   Alternative: `vcpkg install sfml`, then pass the vcpkg toolchain file to CMake.
3. In a "Developer PowerShell for VS 2022":
   ```powershell
   cmake -S . -B build -DSFML_DIR=C:/SFML-3.0.2/lib/cmake/SFML
   cmake --build build --config Debug
   ctest --test-dir build -C Debug
   .\build\Debug\resqgrid.exe data\scenario_provided.txt
   ```
4. For `--gui`, copy the SFML DLLs from `C:\SFML-3.0.2\bin` next to `resqgrid.exe`, or add that folder to `PATH`.

With MinGW, the SFML download must match your MinGW version exactly; otherwise use MSVC.

**SFML is optional at configure time.** If CMake cannot find SFML 3, it prints a warning and builds
everything except the window. `--gui` then exits with a clear error.

### Command line
```
resqgrid <scenario_file> [--gui] [--log <path>] [--quiet]
```
| Example | What it does |
|---|---|
| `resqgrid data/scenario_provided.txt` | console run: log lines + final statistics |
| `resqgrid data/scenario_provided.txt --gui` | opens the SFML window |
| `resqgrid data/scenario_05_stress.txt --log output/stress_log.txt --quiet` | log to a file only, then print statistics |
| `resqgrid data/invalid/invalid_02_bad_values_and_references.txt` | prints every validation error, exit code 2 |

Exit codes: `0` ok, `1` bad arguments (or `--gui` in a build without SFML), `2` invalid scenario file.

Day-1 behaviour: the program runs and prints `=== Final statistics (not implemented yet) ===`.
Every test prints its `TODO` items and passes.

---

## Folder structure and owners

```
resqgrid/
|-- CMakeLists.txt                     Sara
|-- README.md                          Farwa
|-- .gitignore                         Sara
|-- src/
|   |-- main.cpp                       Farwa   (Sara adds the --gui branch)
|   |-- containers/                    header-only
|   |   |-- DynamicArray.h             Farwa
|   |   |-- LinkedList.h               Farwa
|   |   |-- Queue.h                    Farwa
|   |   |-- MergeSort.h                Farwa
|   |   |-- HashMap.h                  Rabia
|   |   |-- MinHeap.h                  Rabia
|   |   `-- IndexedMaxHeap.h           Sara
|   |-- core/
|   |   |-- Types.h                    Shared (committed by Farwa)
|   |   |-- World.h                    Shared (committed by Rabia)
|   |   |-- Robot.h / .cpp             Farwa
|   |   |-- Request.h / .cpp           Farwa
|   |   |-- Environment.h / .cpp       Rabia
|   |   |-- Router.h / .cpp            Rabia
|   |   |-- ScenarioLoader.h / .cpp    Rabia
|   |   |-- EventProcessor.h / .cpp    Rabia
|   |   |-- Rerouter.h / .cpp          Rabia
|   |   |-- Coordinator.h / .cpp       Sara
|   |   |-- Movement.h / .cpp          Sara
|   |   `-- Simulation.h / .cpp        Sara
|   |-- output/
|   |   |-- Logger.h / .cpp            Farwa
|   |   `-- Statistics.h / .cpp        Farwa
|   `-- ui/                            Renderer and UIApp need SFML 3; GraphLayout does not
|       |-- GraphLayout.h / .cpp       Sara
|       |-- Renderer.h / .cpp          Sara
|       `-- UIApp.h / .cpp             Sara
|-- tests/                             each file has its own main() and CMake target
|   |-- test_containers.cpp            Farwa   (DynamicArray, LinkedList, Queue, MergeSort, Types)
|   |-- test_loader.cpp                Rabia   (HashMap, ScenarioLoader)
|   |-- test_router.cpp                Rabia   (MinHeap, Environment, Router)
|   `-- test_coordinator.cpp           Sara    (IndexedMaxHeap, Coordinator, Scenario 01 run)
|-- data/                              scenario files (do not edit, except the scenario_04 fix below)
|-- docs/                              report.pdf, ResQGrid_Development_Plan.pdf, project_spec.pdf
`-- output/                            logs are written here (gitignored except .gitkeep)
```

Every source file starts with a header comment (`File / Owner / Purpose / Depends on / Used by /
Milestone`). Every stub body has:
```cpp
// TODO(<Owner>): what to implement
// See: report Section x.y   |   Done when: <check from the plan>
```
To find your work: `grep -rn "TODO(Farwa)" src tests` (or `Rabia`, `Sara`).

## Who does what

| Person | Files | Count | Character of the work |
|---|---|---|---|
| **Farwa** | DynamicArray.h, LinkedList.h, Queue.h, MergeSort.h, Types.h, Robot.h/.cpp, Request.h/.cpp, Logger.h/.cpp, Statistics.h/.cpp, main.cpp, test_containers.cpp, README.md | **16** | Many short, self-contained files; easy to test in isolation |
| **Rabia** | HashMap.h, MinHeap.h, World.h, Environment.h/.cpp, Router.h/.cpp, ScenarioLoader.h/.cpp, EventProcessor.h/.cpp, Rerouter.h/.cpp, test_loader.cpp, test_router.cpp | **15** | Graph, Dijkstra, file parsing and validation, events |
| **Sara** | IndexedMaxHeap.h, Coordinator.h/.cpp, Movement.h/.cpp, Simulation.h/.cpp, GraphLayout.h/.cpp, Renderer.h/.cpp, UIApp.h/.cpp, test_coordinator.cpp, CMakeLists.txt | **15** | Coordination core plus the whole SFML UI |

Rabia also applies the scenario_04 data fix (see [Known issues](#known-issues)).

## Dependency order

Each layer only uses layers above it, and everything within a layer can be built in parallel.
All headers already exist, so code against the signatures even before the `.cpp` behind them is written.

| Layer | Modules |
|---|---|
| Day 1 | Types.h, World.h, all headers (signatures only): **done in this skeleton** |
| L1 | DynamicArray, LinkedList, Queue, MergeSort (Farwa); HashMap, MinHeap (Rabia); IndexedMaxHeap (Sara) |
| L2 | Robot, Request, Logger (Farwa); Environment (Rabia); GraphLayout (Sara) |
| L3 | Statistics (Farwa); ScenarioLoader, Router (Rabia); Renderer (Sara) |
| L4 | EventProcessor, Rerouter (Rabia); Coordinator, Movement (Sara) |
| L5 | Simulation (Sara) |
| L6 | main.cpp console (Farwa); UIApp (Sara) |

**What you can start on immediately**

| Person | Independent work (needs nobody else's code) |
|---|---|
| Farwa | DynamicArray and LinkedList first (everyone needs them, **aim to merge on 16 Nov**), then Queue, MergeSort, test_containers.cpp, then Robot and Request. |
| Rabia | HashMap and MinHeap (with a temporary raw array until DynamicArray is merged), the line tokenizer and the error-collection part of the loader. |
| Sara | IndexedMaxHeap with its test; SFML window, GraphLayout and Renderer on a hardcoded 5-location graph. |

How to avoid waiting on each other: until Logger is real, `Logger::write` already prints to the
console (that's the day-1 stub). Until Router is real, Coordinator can be tested with a
hand-filled distance array.

## Milestones (15-26 Nov 2026)

| | Dates | Farwa | Rabia | Sara | Check that proves it |
|---|---|---|---|---|---|
| **M0** | 15 Nov | Kickoff | Kickoff | Kickoff, CMake | Empty program builds on all laptops |
| **M1** | 16-17 Nov | DynamicArray, LinkedList, Queue, MergeSort, test_containers | HashMap, MinHeap | IndexedMaxHeap, SFML window + layout on fake graph | All container tests pass; window shows a fake map |
| **M2** | 18-19 Nov | Robot, Request (state checks), Logger | Environment, ScenarioLoader, Router, test_loader, test_router | Renderer draws the loaded map; start Coordinator | 3 invalid files rejected with all errors; Scenario 01 route costs match its comments; provided file draws on screen |
| **M3** | 20-21 Nov | Statistics; main.cpp console mode | EventProcessor, Rerouter | Coordinator, Movement, Simulation | Scenario 01 runs to the end and matches the hand trace (Report, Appendix A); Scenario 03 matches |
| **M4** | 22 Nov | Help test; fix bugs in own files | Scenarios 02 and 04 match | UIApp connected to step(): play / pause / step / speed, panels | All 5 scenarios match their expected outcome; GUI runs the provided file |
| **M5** | 23-24 Nov | Timing experiments (56/120/250/500/1000 locations), results table; README | Stress run, invariant checks | Final-stats screen, error screen, polish | Two runs give identical logs; no battery < 0; no blocked road used |
| **M6** | 25-26 Nov | Report sections | Report sections | Assemble report, final build, submit | Clean clone builds and runs from README alone |

Don't start the next milestone's integration until the current check passes.

---

## Simulation rules (summary)

The full rules are in **docs/report.pdf, Section 4**. They are fixed: don't change them in code.

**One step at time t: 7 phases, always in this order** (Report 4.2):
1. **Events**: apply every event with time t, in file order (Report 6.3).
2. **New requests**: requests with `report_time == t` become PENDING and enter the priority queue, in file order.
3. **Reroute check**: only if a road or station changed in phase 1, and only *after all* events of the step, so a block and unblock in the same step cause no rerouting (Report 4.7).
4. **Assignment**: match pending requests to available robots (Report 6.2, Algorithm 2).
5. **Low battery**: an idle robot below `low_battery_percent` (default 30%) of its maximum goes to the nearest open charger.
6. **Action**: robots act in increasing ID order (move, work or recharge); battery is updated.
7. **Record**: log lines and statistics for the step.

**Movement** (Report 4.1): a robot travels `speed` cost units per step and can cross a location
mid-step. A robot that reaches its request location starts work in the **next** step. Work lasts
`duration_steps`.

**Battery** (Report 4.5): moving costs `battery_per_cost_unit` per unit, and working costs
`battery_per_work_step` per step. Recharging adds `recharge_per_step` per step at an **open**
station, up to the maximum; the robot isn't a candidate until it is full.
**Feasibility:** a robot may accept a request only if
`battery >= b_c * (d(robot, req) + d(req, nearest open charger)) + b_w * duration`.
Every rejection is logged.

**Request priority** (Report 5.6): key = `aging_interval * urgency - report_time` (larger first,
`aging_interval` defaults to 50), then smaller request ID. The key never needs rebuilding as time
passes.

**Robot tie-breaks** (Report 6.2). A candidate is AVAILABLE, capable (or GENERAL), reachable and
feasible. Among candidates:
1. smallest travel cost;
2. specialist before GENERAL;
3. higher battery;
4. smaller robot ID.

More than one candidate counts as a **conflict**. An unservable request never blocks the queue:
skip it and continue.

**No-route rule** (Report 4.6): the request stays PENDING and is marked *unreachable*. It is retried
only after the road network changes (an UNBLOCK bumps the network version).

**Rerouting** (Report 4.7):
- If the robot is **on** the blocked road, it turns back to the location it came from (paying
  battery for that distance), then reroutes.
- If a blocked road is **later** in its route, it reroutes from its next location.
- If no feasible route exists, the request returns to PENDING (a reassignment) and the robot becomes
  AVAILABLE, or RETURNING if its battery is low.

A charging station that goes down is handled the same way.

**Termination** (Report 4.8): stop at the first step where `t == max_steps`, or where every request
is COMPLETED or FAILED with nothing left to report and no events left. Unfinished requests become
FAILED with a reason (`unreachable` or `not_served`).

---

## Input format

Full specification: **docs/report.pdf, Section 9**. `data/scenario_provided.txt` must load unchanged.

- Lines starting with `#` and blank lines are ignored. Fields are separated by spaces, and names contain no spaces.
- Optional `SCENARIO <name>`; `PARAM <key> <value>` lines; each section is `KEYWORD <count>` followed by exactly `count` records; the file ends with `END`.

| Section | Record fields |
|---|---|
| `PARAM` | `max_steps`, `speed`, `battery_per_cost_unit`, `battery_per_work_step`, `recharge_per_step`; our optional extensions `aging_interval` (default 50) and `low_battery_percent` (default 30) |
| `LOCATIONS` | `id name type`; type is JUNCTION, BUILDING, HOSPITAL, DEPOT, CHARGING or COMMAND |
| `CONNECTIONS` | `id location_a location_b cost status`; cost > 0; status is OPEN or BLOCKED; undirected |
| `ROBOTS` | `id start_location capability max_battery initial_battery`; capability is MEDICAL, FIRE, SUPPLY, SEARCH or GENERAL; 0 <= initial <= max |
| `REQUESTS` | `id report_time location category urgency duration_steps`; category is MEDICAL, FIRE, SUPPLY or SEARCH; urgency 1-5; time >= 0; duration > 0 |
| `EVENTS` | `time TYPE ref [value]`: `BLOCK`/`UNBLOCK <connection>`, `STATION_DOWN`/`STATION_UP <location>`, `SET_URGENCY <request> <urgency>`, `ROBOT_OFFLINE`/`ROBOT_ONLINE <robot>` |

**Validation** (Report 9.4):
- **Errors reject the file.** All errors are listed with their line numbers: count mismatch, missing
  END, bad fields or numbers, duplicate IDs, unknown names, missing references, self-loops, values
  out of range, and station events on a non-charger.
- **Warnings don't reject it:** unknown PARAM key, an event after `max_steps`, and redundant events
  at run time.

## Log format

One line per state change. Every line is `t=<step> <KIND>` followed by `key=value` fields,
separated by single spaces. All numbers are **file IDs**, never internal indices.

```
t=0 ASSIGN request=2 robot=3 route=7-5-6 cost=6
t=0 REJECT request=1 robot=2 reason=battery need=15 have=12
t=0 STATE robot=2 AVAILABLE->RETURNING target=7
t=4 EVENT BLOCK connection=4 (2-5)
t=4 REROUTE robot=1 on=4 back_to=2 route=2-4-7-5 cost=9
t=7 COMPLETE request=2 robot=3 response=7
```

| KIND | Fields | Written by |
|---|---|---|
| `REQUEST` | `request= at= category= urgency=` | Simulation (phase 2) |
| `EVENT` | `<TYPE> <connection/location/request/robot>=<id> [detail]` | EventProcessor |
| `ASSIGN` | `request= robot= route= cost=` | Coordinator |
| `REJECT` | `request= robot= reason=battery need= have=` | Coordinator |
| `STATE` | `robot=<id> FROM->TO [target=]` or `request=<id> FROM->TO` | all modules |
| `REROUTE` | `robot= [on= back_to=] route= cost=` | Rerouter |
| `RELEASE` | `request= robot= reason=offline/no_route` | EventProcessor, Rerouter |
| `ARRIVE` | `robot= at=` | Movement |
| `COMPLETE` | `request= robot= response=` | Movement |
| `FAIL` | `request= reason=unreachable/not_served` | Simulation (end of run) |
| `WARN` | free text | any |
| `INVALID` | `robot/request=<id> FROM->TO` (rejected transition) | any |
| `STATS` | `key=value ...` final statistics | Statistics |

The report's example (Report 9.5) pads the KIND column with spaces. We use single spaces so logs
are easy to `diff` and parse.

---

## Git workflow

- `main` is **protected**: nobody pushes to it directly.
- One branch per module: `<name>/<module>`, e.g. `farwa/dynamic-array`, `rabia/router`, `sara/coordinator`.
- Merge by **pull request** with **one approving review** from another member. Read the code you approve: everyone must explain every file at the demo.
- Commit messages: short, present tense, e.g. `Add Dijkstra with lazy deletion`.
- Pull from `main` every morning before starting work. Never change someone else's file without telling them; open an issue or comment on their PR instead.
- **Before opening a PR:**
  1. `cmake --build build` has no errors (and no new warnings);
  2. your own test passes (`ctest --test-dir build`);
  3. no STL containers:
     ```bash
     grep -rnE '#include *<(vector|list|map|set|queue|stack|deque|unordered_map|unordered_set|array|forward_list)>' src tests \
       && echo "STL container found: remove it" || echo "OK: no STL containers"
     ```
  4. every `new` has a matching `delete` (run a test under valgrind or `-fsanitize=address`).

## Known issues

- **For Rabia: `data/scenario_04_dynamic_events.txt`.** Connections **35-37** duplicate 32-34
  (and 36 is an OPEN copy of the BLOCKED road 33). Delete records 35, 36 and 37 and change the
  header to `CONNECTIONS 34`. Without this, two of the file's own tests (BLOCK 29 at t=3, and the
  UNBLOCK/BLOCK 33 pair at t=8) never trigger. **Only Rabia makes this change, in her PR. Nobody
  else should edit the file.**

## Open points to settle as a team

These are not answered by the report. Decide them together, then write the answer here:
- Tie-break when two open chargers are equally near (`Router::nearestOpenCharger`). Suggestion: smaller location ID.
- What `ROBOT_OFFLINE` does to a robot that is partway along a road.
- Definition of robot utilization in the statistics. Suggestion: busy steps / (robots x steps run).
- Whether a missing spec `PARAM` (e.g. no `speed` line) is an error or uses a default.
