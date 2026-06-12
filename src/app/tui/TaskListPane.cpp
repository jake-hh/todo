#include "TaskListPane.h"

#include <ftxui/component/event.hpp>


ftxui::Component MakeTaskListPane(std::vector<std::string>& labels, int& selected) {
    using namespace ftxui;

    // Menu handles arrow key navigation and updates `selected` in place.
    // It takes raw pointers so changes to `labels` are reflected immediately.
    auto menu = Menu(&labels, &selected);

    // CatchEvent intercepts j/k before Menu sees them and re-dispatches as
    // arrow keys, adding vim-style navigation without replacing the default behaviour.
    return CatchEvent(menu, [menu](Event e) {
        if (e == Event::Character('j')) return menu->OnEvent(Event::ArrowDown);
        if (e == Event::Character('k')) return menu->OnEvent(Event::ArrowUp);
        return false;
    });
}
