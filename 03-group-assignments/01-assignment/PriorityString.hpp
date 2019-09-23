#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class PriorityString {
    public:
        long long priority;
        string value;

    PriorityString(long long _priority, string const stringvalue);

    bool operator< (PriorityString const& other) const;

    bool operator> (PriorityString const& other) const;
};

