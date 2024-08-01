#include "scheduler.h"

int main() {
    Scheduler scheduler("input.txt", "log.txt");
    ull time = 1;
    while(true) {
        scheduler.run(time++);
        if (time == 30) break;
    }
    return 0;
}