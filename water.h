/*
 *  Water Automation Header File
 *  Kinan Rabbat and Co.
 *  04/01/2024
 *
 *  Defines functions for water automation
 * 
 * Uses plantState.txt to store the current state of plants
 * Uses presets.txt to store presets
 * 
 */

#ifndef WATER
#define WATER

#include <iostream>

using namespace std;

class PlantList{
public:
    void commandLine();
    void createPreset();

    void createPlant();
    void createPlantFromPreset();
    void deletePlant();

    void displayPlants();

    void swapPlants();
    void movePlant();
    void changeNumCubes();

    void changeName();
    void changeFeedAmount();
    void changeFeedFreqeuency();
    void changeFeedingList();
    void changeMoisture();
    void changeFeedList();

private:

    void storePreset();
    void storeState();

    struct Plant{
        string name;
        int cubeNo;               
        int waterAmt;           //water per feeding (ml)
        int frequency;          //hours
        float idealMoisture;    //ideal moisture level at feeding
        int hoursSinceFeed; 
        int feedingList[];      //array to schedule how much to be fed each week
                                //if feed amounts need to change over time
    };

    Plant* plants;
    int numCubes;
};

#endif
