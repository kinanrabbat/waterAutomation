
#include "water.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "water.h"
#include <thread>


using namespace std;


void timer(PlantList &plants) {
    while (true) {
        this_thread::sleep_for(chrono::hours(12));
        plants.scheduler();
    }
}

int main(){
    cout << endl;
    PlantList plants = PlantList(3);
    thread timerThread(timer, ref(plants));
    while(1)
        plants.commandLine();

    return 0;
}
