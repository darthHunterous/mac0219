#include "PriorityString.hpp"

using namespace std;

PriorityString::PriorityString(long long _priority, string const stringvalue)
    : priority(_priority), value(stringvalue) {
}

bool PriorityString::operator< (PriorityString const& other) const {
    return priority > other.priority;
}

bool PriorityString::operator> (PriorityString const& other) const {
    return priority < other.priority;
}
