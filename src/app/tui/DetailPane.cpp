#include "DetailPane.h"

#include <algorithm>
#include <string>
#include <ftxui/dom/elements.hpp>


ftxui::Component MakeDetailPane(const TaskStore& store,
                                const std::vector<unsigned>& ids,
                                int& selected) {
    using namespace ftxui;

    // Renderer re-runs its lambda on every frame, so the detail view always
    // reflects the current selection without any manual sync.
    return Renderer([&store, &ids, &selected] {
        if (ids.empty()) return text("(no tasks)");

        // Clamp in case selected is momentarily out of range during a deletion.
        int sel = std::max(0, std::min(selected, static_cast<int>(ids.size()) - 1));
        const Task& t = store.get(ids[sel]);

        // Helper to render a bold label followed by its value on one line.
        auto field = [](const std::string& label, const std::string& value) -> Element {
            return hbox({text(label + ": ") | bold, text(value)});
        };

        // Flatten the deps SmartArray into a comma-separated string.
        std::string deps;
        if (t.deps.isEmpty()) {
            deps = "none";
        } else {
            for (size_t i = 0; i < t.deps.size(); i++) {
                if (i > 0) deps += ", ";
                deps += std::to_string(t.deps[i]);
            }
        }

        return vbox({
            field(" ID",          std::to_string(t.id)),
            field(" Title",       t.title),
            field(" Status",      t.statusStr()),
            field(" Priority",    t.priorityStr()),
            field(" Created",     t.createdAtStr()),
            field(" Due",         t.dueDateStr()),
            field(" Description", t.description),
            field(" Deps",        deps),
        });
    });
}
