#include "utils.h"
using namespace std;

string currentTime()
{
    time_t now = time(0);
    char *dt = ctime(&now);
    std::string s(dt);
    s.pop_back(); // remove '\n'
    return s;
}
