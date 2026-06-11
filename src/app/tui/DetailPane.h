#pragma once

#include <vector>
#include <ftxui/component/component.hpp>
#include "../data/TaskStore.h"


/**
 * @brief Creates a read-only detail view for the currently selected task.
 * @param store    The task store (read-only).
 * @param ids      Ordered list of task IDs matching the list pane.
 * @param selected Index into ids for the currently selected task.
 * @return ftxui component that re-renders on each frame.
 */
ftxui::Component MakeDetailPane(const TaskStore& store,
                                const std::vector<unsigned>& ids,
                                int& selected);
