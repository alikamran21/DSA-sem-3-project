#include "Array_handler.h"
#include <iostream>
using namespace std;

int main() {
    arrayHandler handler;
    handler.addTask("Finish DSA project");
    handler.addTask("Review header files");
    handler.showTaskList();

    handler.removeTask();
    handler.showTaskList();

    handler.clearTask();
    handler.showTaskList();

    return 0;
}
