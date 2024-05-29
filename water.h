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
#include <list>
#include <vector>

using namespace std;

class PlantList{
public:
    PlantList(int cubeNum);
    ~PlantList();

    void commandLine();
    void help();
    void scheduler();

    void openCube(int cube);
    void closeCube(int cube);

    void createPreset();
    void deletePreset();
    void clearPresets();

    void createPlant();
    void createPlantFromPreset();
    void deletePlant();
    void clearPlants();

    void displayPlants();
    void displayPresets();

    void swapPlants();
    void movePlant();

    void changeName();
    void changeWaterAmount();
    void changeFrequency();
    void changeMoisture();
    void changeAdultWaterAmt();
    
    struct Plant{
        int cubeNo;
        string name;
        float waterAmt;           //water per feeding (ml)
        float frequency;          //days
        float idealMoisture;    //ideal moisture level at feeding
        int hoursTillFeed; 
        int feedingsTillAdult;
        float adultWaterAmt;
    };

private:

    void storeState();

    list<string> presetNames;
    vector<Plant> plants;
    int numCubes;
};

#endif