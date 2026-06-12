#include "Task.h"

#include <ctime>


// file-local bc static
static std::string formatDate(int64_t epoch) {
    if (epoch == -1)
        return "none";

    time_t t = static_cast<time_t>(epoch);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%d/%m/%y", std::localtime(&t));
    return buf;
}


std::string Task::statusStr() const {
    switch (status) {
        case 0:  return "open";
        case 1:  return "in-progress";
        case 2:  return "done";
        case 3:  return "wontfix";
        default: return "?";
    }
}


std::string Task::statusSymbol() const {
    switch (status) {
        case 0:  return "☐";
        case 1:  return "🌟";  // ✯
        case 2:  return "✔";
        case 3:  return "✗";
        default: return "?";
    }
}


std::string Task::priorityStr() const {
    switch (priority) {
        case 0:  return "wishlist";
        case 1:  return "low";
        case 2:  return "medium";
        case 3:  return "high";
        default: return "?";
    }
}


std::string Task::dueDateStr() const {
    return formatDate(dueDate);
}


std::string Task::createdAtStr() const {
    return formatDate(createdAt);
}
