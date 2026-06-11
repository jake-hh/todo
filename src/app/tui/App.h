#pragma once

#include <string>
#include <vector>
#include "../data/TaskStore.h"


/**
 * @brief Root application component.
 *
 * Owns TaskStore, builds the two-pane layout, and runs the ftxui event loop.
 */
class App {
private:
    TaskStore _store;
    std::vector<unsigned> _ids;        // cached task IDs in list order; index matches _entries
    std::vector<std::string> _entries; // cached strings shown in the list pane
    int _selected = 0;                 // index of the currently highlighted task

public:
    App();

    /** @brief Start the event loop. Blocks until the user quits. */
    void run();
};
