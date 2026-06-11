#pragma once

#include <string>
#include <cstdint>
#include "SmartArray.h"

/**
 * @brief Represents a single task in the todo manager.
 */
struct Task {
    unsigned id;          ///< Unique auto-assigned ID
    std::string title;
    std::string description;
    int priority;         ///< 0=wishlist, 1=low, 2=medium, 3=high
    int status;           ///< 0=open, 1=in-progress, 2=done, 3=wontfix
    int64_t createdAt;    ///< Unix epoch seconds
    int64_t dueDate;      ///< Unix epoch seconds, or -1 if none
    SmartArray<unsigned> deps; ///< Depencies / subtasks (IDs of tasks this task is blocked by)

    /** @brief Returns a human-readable label for the status value. */
    std::string statusLabel() const;

    /** @brief Returns a human-readable label for the priority value. */
    std::string priorityLabel() const;

    /** @brief Returns the due date formatted as DD/MM/YY, or "none" if unset. */
    std::string dueDateLabel() const;

    /** @brief Returns the creation date formatted as DD/MM/YY. */
    std::string createdAtLabel() const;
};
