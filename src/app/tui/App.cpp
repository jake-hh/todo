#include "App.h"

#include <set>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>
#include "TaskListPane.h"
#include "DetailPane.h"


App::App() {
    // Hardcode seed tasks with stub deps to demonstrate the tree view.
    {
        unsigned id0 = _store.create("Fix login bug",
                                     "Users cannot log in when using OAuth",
                                     3, 0, 1800000000LL);

        unsigned id1 = _store.create("Write unit tests",
                                     "Cover TaskStore CRUD operations",
                                     2, 1, -1LL);

        unsigned id2 = _store.create("Update documentation",
                                     "Align README with new API",
                                     1, 0, 1802000000LL);

        unsigned id3 = _store.create("Refactor data layer",
                                     "Extract serialization into FileIO",
                                     2, 2, -1LL);

        unsigned id4 = _store.create("Deploy to staging",
                                     "Push latest build to staging environment",
                                     3, 3, -1LL);

        unsigned id5 = _store.create("Provision server",
                                     "Spin up staging VM and configure firewall",
                                     2, 0, -1LL);

        unsigned id6 = _store.create("Set up CI pipeline",
                                     "Configure GitHub Actions for staging deploys",
                                     2, 1, -1LL);

        unsigned id7 = _store.create("Obtain SSL certificate",
                                     "Request cert from CA for staging domain",
                                     1, 0, -1LL);

        // Stub deps: id0 blocked by id1, id1 blocked by id2,
        // id3 blocked by both id1 and id4 (demonstrates a duplicate node).
        // id4 blocked by id5, id5 blocked by id6, id6 blocked by id7
        _store.get(id0).deps.pushBack(id1);
        _store.get(id1).deps.pushBack(id2);
        _store.get(id3).deps.pushBack(id1);
        _store.get(id3).deps.pushBack(id4);
        _store.get(id4).deps.pushBack(id5);
        _store.get(id5).deps.pushBack(id6);
        _store.get(id6).deps.pushBack(id7);
    }

    // Collect IDs of all tasks that are blockers (appear in any dep list).
    std::set<unsigned> allDeps;
    for (auto& [id, task] : _store.tasks())
        for (size_t i = 0; i < task.deps.size(); i++)
            allDeps.insert(task.deps[i]);

    // Start building on tree roots - tasks that no other task depend on.
    for (auto& [id, task] : _store.tasks())
        if (!allDeps.count(id))
            buildTreeFrom(id, 0);
}


void App::buildTreeFrom(unsigned id, int depth) {
    const Task& t = _store.get(id);

    std::string prefix;
    if (depth > 0)
        prefix = std::string(3 * (depth-1) + 0, ' ') + "└─ ";

    _ids.push_back(id);
    _labels.push_back(prefix + t.statusSymbol() + " " + t.title);

    // DFS on each tasks deps (blockers).
    // Duplicate tasks appear under each parent — no visited guard needed.
    for (size_t i = 0; i < t.deps.size(); i++)
        buildTreeFrom(t.deps[i], depth + 1);
}


void App::run() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    // Both panes share _selected by reference so navigating the list
    // automatically updates the detail view without any explicit sync.
    auto list_pane   = MakeTaskListPane(_labels, _selected);
    auto detail_pane = MakeDetailPane(_store, _ids, _selected);

    // Container::Horizontal routes keyboard focus between the two panes
    // and handles focus cycling.
    auto layout = Container::Horizontal({list_pane, detail_pane});

    // Renderer wraps the layout to add the border/title chrome around each pane.
    // Terminal::Size() is queried each frame so the split tracks terminal resizes.
    // Explicit size() avoids ftxui's flex distribution, which allocates space as
    // min_size + extra/2 per pane — causing widths to jump when detail content
    // changes natural width on each selection change.
    auto renderer = Renderer(layout, [&] {
        int w = Terminal::Size().dimx;
        int left_w  = w / 2;
        int right_w = w - left_w;
        return hbox({
            vbox({
                text(" Tasks") | bold,
                separator(),
                list_pane->Render() | flex,
            }) | border | size(WIDTH, EQUAL, left_w),
            vbox({
                text(" Details") | bold,
                separator(),
                detail_pane->Render() | flex,
            }) | border | size(WIDTH, EQUAL, right_w),
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
