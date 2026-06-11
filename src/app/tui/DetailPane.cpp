#include "DetailPane.h"

#include <algorithm>
#include <ctime>
#include <string>
#include <ftxui/dom/elements.hpp>


namespace {

// These helpers are file-local so the pane owns the display mapping and
// callers never need to know the numeric encoding.

std::string statusStr(int s) {
    switch (s) {
        case 0:  return "open";
        case 1:  return "in-progress";
        case 2:  return "done";
        case 3:  return "wontfix";
        default: return "?";
    }
}


std::string priorityStr(int p) {
    switch (p) {
        case 0:  return "wishlist";
        case 1:  return "low";
        case 2:  return "medium";
        case 3:  return "high";
        default: return "?";
    }
}


// Formats a Unix epoch timestamp as DD/MM/YY. Returns "none" for -1 (unset).
std::string dateStr(int64_t t) {
    if (t == -1) return "none";
    time_t tt = static_cast<time_t>(t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%d/%m/%y", std::localtime(&tt));
    return buf;
}

} // namespace


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
            for (size_t i = 0; i < t.deps.size(); ++i) {
                if (i > 0) deps += ", ";
                deps += std::to_string(t.deps[i]);
            }
        }

        return vbox({
            field(" ID",          std::to_string(t.id)),
            field(" Title",       t.title),
            field(" Status",      statusStr(t.status)),
            field(" Priority",    priorityStr(t.priority)),
            field(" Created",     dateStr(t.createdAt)),
            field(" Due",         dateStr(t.dueDate)),
            field(" Description", t.description),
            field(" Deps",        deps),
        });
    });
}
