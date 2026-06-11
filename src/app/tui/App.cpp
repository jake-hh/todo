#include "App.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "TaskListPane.h"
#include "DetailPane.h"


namespace {

// Kept file-local so neither pane needs to know the status encoding.
std::string statusStr(int s) {
    switch (s) {
        case 0:  return "open";
        case 1:  return "in-progress";
        case 2:  return "done";
        case 3:  return "wontfix";
        default: return "?";
    }
}

} // namespace


App::App() {
    // Hardcoded seed tasks — placeholder until FileIO is wired up.
    _store.create("Fix login bug",
                  "Users cannot log in when using OAuth",
                  3, 0, 1800000000LL);

    _store.create("Write unit tests",
                  "Cover TaskStore CRUD operations",
                  2, 1, -1LL);

    _store.create("Update documentation",
                  "Align README with new API",
                  1, 0, 1802000000LL);

    _store.create("Refactor data layer",
                  "Extract serialization into FileIO",
                  2, 2, -1LL);

    _store.create("Deploy to staging",
                  "Push latest build to staging environment",
                  3, 3, -1LL);

    // Build the parallel caches.
    // std::map iterates in ID order so the resulting vectors are sorted by ID.
    for (auto& [id, task] : _store.tasks()) {
        _ids.push_back(id);
        _entries.push_back("[" + statusStr(task.status) + "] " + task.title);
    }
}


void App::run() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    // Both panes share _selected by reference so navigating the list
    // automatically updates the detail view without any explicit sync.
    auto list_pane   = MakeTaskListPane(_entries, _selected);
    auto detail_pane = MakeDetailPane(_store, _ids, _selected);

    // Container::Horizontal routes keyboard focus between the two panes
    // and handles focus cycling.
    auto layout = Container::Horizontal({list_pane, detail_pane});

    // Renderer wraps the layout to add the border/title chrome around each pane.
    // flex makes each pane grow to fill its half of the terminal width.
    auto renderer = Renderer(layout, [&] {
        return hbox({
            vbox({
                text(" Tasks") | bold,
                separator(),
                list_pane->Render() | flex,
            }) | border | flex,
            vbox({
                text(" Details") | bold,
                separator(),
                detail_pane->Render() | flex,
            }) | border | flex,
        });
    });

    // CatchEvent sits above the layout so 'q' is handled globally regardless
    // of which pane has focus.
    auto root = CatchEvent(renderer, [&](Event e) {
        if (e == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(root);
}
