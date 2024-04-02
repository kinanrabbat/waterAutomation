/*
 *  Water Automation Header File
 *  Kinan Rabbat and Co.
 *  04/01/2024
 *
 *  Implementation for water automation
 */

#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include "water.h"

using namespace std;

/*
 * purpose:   initialize an empty plant list
 * arguments: none
 * returns:   none
 * effects:   none
 */
PlantList::PlantList(){
    plants = nullptr;
    numCubes = 0;
}

/*
 * purpose:   initialize an empty plant list
 * arguments: none
 * returns:   none
 * effects:   none
 */
PlantList::PlantList(int cubeNum){
    numCubes = cubeNum;
    plants = new Plant[cubeNum];
}

/*
 * purpose:   clean the plants list
 * arguments: none
 * returns:   none
 * effects:   none
 */
PlantList::~PlantList() {
    delete[] plants;
    plants = nullptr;
}

/*
 * purpose:   prints a list of all possible commands and asks a user for a 
 *            command
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::commandLine(){

}


/*
 * purpose:   asks user to input plant parameters and stores plant preset 
 *            in presets.txt
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::createPreset() {
    
}

/*
 * purpose:   removes preset from preset.txt by deleting corresponding line in
              presets.txt
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::deletePreset() {
    
}

/*
 * purpose:   asks user to input plant parameters, creates plant in plant list, 
 *            stores plant in plantState.txt 
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::createPlant() {
    Plant newPlant;
    cout << "Enter plant name: ";
    cin >> newPlant.name;
    cout << "Enter cube number: ";
    cin >> newPlant.cubeNo;
    cout << "Enter water amount per feeding (ml): ";
    cin >> newPlant.waterAmt;
    cout << "Enter feeding frequency (hours): ";
    cin >> newPlant.frequency;
    cout << "Enter ideal moisture level: ";
    cin >> newPlant.idealMoisture;
    cout << "Enter hours since last feed: ";
    cin >> newPlant.hoursSinceFeed;
    cout << "Is it a night feeding? (1 for yes, 0 for no): ";
    cin >> newPlant.nightFeeding;
    cout << "Enter feeding list (up to 7 days): ";
    for (int i = 0; i < 7; ++i) {
        cin >> newPlant.feedingList[i];
    }

    Plant* newPlants = new Plant[numCubes + 1];
    for (int i = 0; i < numCubes; ++i) {
        newPlants[i] = plants[i];
    }
    newPlants[numCubes] = newPlant;
    delete[] plants;
    plants = newPlants;
    numCubes++;
}

/*
 * purpose:   asks user to choose a preset, creates plant in plant list, 
 *            stores plant in plantState.txt 
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::createPlantFromPreset() {

}

/*
 * purpose:   removes plant from plant list and removes its corresponding line
 *            from plantState.txt
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::deletePlant() {
    int cubeNumber;
    cout << "Enter cube number of plant to delete: ";
    cin >> cubeNumber;

    
}

/*
 * purpose:   none
 * arguments: prints plantState.txt
 * returns:   none
 * effects:   none
 */
void PlantList::displayPlants() {
 
}

/*
 * purpose:   none
 * arguments: swaps the cubes two plants are at
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::swapPlants() {
    int cube1, cube2;
    cout << "Enter cube number of first plant to swap: ";
    cin >> cube1;
    cout << "Enter cube number of second plant to swap: ";
    cin >> cube2;

    
}

/*
 * purpose:   none
 * arguments: moves plant to different cube
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::movePlant() {
    int cubeNumber, newCube;
    cout << "Enter cube number of plant to move: ";
    cin >> cubeNumber;
    cout << "Enter new cube number: ";
    cin >> newCube;

    //Plant x moved to cube y successfully
    //Cube y not found 
    //Cube y already has a plant

}

/*
 * purpose:   none
 * arguments: changes number of cubes, copies plant data to new plant list
 * returns:   none
 * effects:   plant list
 */
void PlantList::changeNumCubes() {
    int newNumCubes;
    cout << "Enter new number of cubes: ";
    cin >> newNumCubes;

    //Number of cubes changed to x
    //Changing cube number deletes existing plants, please delete plants first 

    // if (newNumCubes < numCubes) {
    //     Plant* newPlants = new Plant[newNumCubes];
    //     for (int i = 0; i < newNumCubes; ++i) {
    //         newPlants[i] = plants[i];
    //     }
    //     delete[] plants;
    //     plants = newPlants;
    //     numCubes = newNumCubes;
    //     cout << "Number of cubes changed to " << newNumCubes << ".\n";
    // } else {
    //     cout << "Cannot increase number of cubes.\n";
    // }
}

/*
 * purpose:   changes name of a plant at a cube
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeName() {
    int cubeNumber;
    cout << "Enter cube number of plant to change name: ";
    cin >> cubeNumber;

}

/*
 * purpose:   changes the amount to feed a plant each week manually
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeFeedAmount() {
    int cubeNumber;
    cout << "Enter cube number of plant to change feed amount: ";
    cin >> cubeNumber;

}

/*
 * purpose:   changes the frequency to feed a plant each week manually
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeFeedFrequency() {
    int cubeNumber;
    cout << "Enter cube number of plant to change feed frequency: ";
    cin >> cubeNumber;

}

/*
 * purpose:   changes feeding list of a plant
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeFeedingList() {
    int cubeNumber;
    cout << "Enter cube number of plant to change feeding list: ";
    cin >> cubeNumber;

}

/*
 * purpose:   changes ideal moisture level of a plant
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeMoisture() {
    int cubeNumber;
    cout << "Enter cube number of plant to change ideal moisture level: ";
    cin >> cubeNumber;

}

/*
 * purpose:   stores a plant as a preset to a line in presets.txt
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::storePreset(){

}

/*
 * purpose:   stores current state of plant list in plantState.txt
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::storeState(){

}
