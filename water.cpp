/*
 *  Water Automation Header File
 *  Kinan Rabbat and Co.
 *  04/01/2024
 *
 *  Implementation for water automation
 */

/*
to do: 

add moisture sensing to scheduler, change water feed amount 
add pump control to scheduler
open and close functions, add to scheduler


*/


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
#include <chrono>
#include <cstdlib>


using namespace std;

const PlantList::Plant emptyPlant = {0, "EMPTY", 0, 0, 0, 0, 0, 0};

/*
 * purpose:   initialize an empty plant list, initialize presets list
 * arguments: integer specifing the number of cubes which is 
 * returns:   none
 * effects:   fills plant vector with empty plants
 */
PlantList::PlantList(int cubeNum){
    
    //open presets.txt
    string filename = "presets.txt";
    ifstream infile;
    string line;
    infile.open(filename);
     if (!infile.is_open()) {
        cerr << "Error opening presets.txt." << endl;
        exit(EXIT_FAILURE);
    }

    //read each line from the file, extract name of preset and add to presets list
    while (getline(infile, line)) {
        istringstream iss(line);
        string word;
        
        //read the second word (plant name)
        if (iss >> word) {
            if (iss >> word) {
                //add plant preset name to the preset name list
                presetNames.push_back(word);
            }
        }
    }

    //close presets.txt
    infile.close();

    numCubes = cubeNum;

    //initialize plants vector
    for(int i = 0; i < cubeNum; i++){
        Plant currPlant = emptyPlant;
        currPlant.cubeNo = i + 1;
        plants.push_back(currPlant);
    }

    //open plantState.txt
    filename = "plantState.txt";
    ifstream plantFile;
    string plantLine;
    plantFile.open(filename);
     if (!plantFile.is_open()) {
        cerr << "Error opening plantState.txt." << endl;
        exit(EXIT_FAILURE);
    }

    //read each line from the file, add 
    while (getline(plantFile, plantLine)) {
        istringstream iss(plantLine);
        Plant currPlant;
        string word;

        //get plant stats from each line
        if(iss >> word){
            currPlant.cubeNo = stoi(word);
            iss >> word;
            currPlant.name = word;
            iss >> word;
            currPlant.waterAmt = stof(word);
            iss >> word;
            currPlant.frequency = stof(word);
            iss >> word;
            currPlant.idealMoisture = stof(word);
            iss >> word;
            currPlant.feedingsTillAdult = stof(word);
            iss >> word;
            currPlant.adultWaterAmt = stof(word);
            currPlant.hoursTillFeed = 0;

            //add plant to plants vector
            plants.at(currPlant.cubeNo-1) = currPlant;
        }        
    }

    //close plantState.txt
    plantFile.close();
}

/*
 * purpose:   clean the plants list
 * arguments: none
 * returns:   none
 * effects:   none
 */
PlantList::~PlantList() {
}

/*
 * purpose:   prints a list of all possible commands and asks a user for a 
 *            command
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::commandLine(){
    //what command would you like to do? Type "help" for a lsit of commands
    string command;

    cout << "What would you like to do? Type 'help' for a list of commands." << endl << "-> ";
    getline(cin, command);

    if(!command.empty()){
        if(command == "help"){
            help();
        } else if(command == "create preset"){
            createPreset();
        } else if(command == "delete preset"){
            deletePreset();
        } else if(command == "clear presets"){
            clearPresets();
        } else if(command == "create plant"){
            createPlant();
        } else if(command == "create plant with preset"){
            createPlantFromPreset();
        } else if(command == "delete plant"){
            deletePlant();
        } else if(command == "clear plants"){
            clearPlants();
        } else if(command == "show plants"){
            displayPlants();
        } else if(command == "show presets"){
            displayPresets();
        } else if(command == "swap"){
            swapPlants();
        } else if(command == "move"){
            movePlant();
        } else if(command == "change name"){
            changeName();
        } else if(command == "change water amount"){
            changeWaterAmount();
        } else if(command == "change frequency"){
            changeFrequency();
        } else if(command == "change moisture"){
            changeMoisture();
        } else if(command == "change adult water amount"){
            changeAdultWaterAmt();
        } else { 
            cout << endl << "Unrecognized command. Please make sure all letters are lowercase." << endl;
        }
        cout << endl;
    }
}

/*
 * purpose:   prints all possible functions
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::help(){
    cout << "Command Name   " << endl;
    cout << "create preset" << endl;
    cout << "delete preset" << endl;
    cout << "clear presets" << endl;
    cout << "create plant" << endl;
    cout << "create plant with preset" << endl;
    cout << "delete plant" << endl;
    cout << "clear plant" << endl;
    cout << "show plants" << endl;
    cout << "show presets" << endl;
    cout << "swap" << endl;
    cout << "move" << endl;
    cout << "change name" << endl;
    cout << "change water amount" << endl;
    cout << "change water frequency" << endl;
    cout << "change moisture" << endl;
    cout << "change adult water amount" << endl;
    cout << "Please make sure that all commands are lowercase!" << endl;
}

/*
 * purpose:   feeds plants when they need to be fed when called by timer in main
 * arguments: none
 * returns:   none
 * effects:   plants vector, motors controlling water flow
 */
void PlantList::scheduler(){
    for(Plant currPlant : plants){
        if(currPlant.name != "EMPTY"){

            //if it's time to feed the plant
            if(currPlant.hoursTillFeed <= 0){
                //plant gets fed

                //if it is still a child plant
                if(currPlant.feedingsTillAdult > 0){
                    currPlant.feedingsTillAdult--;
                }
                int scalingFactor = 1;
                
                int currCubeNo = currPlant.cubeNo;
                int pauseTime = currPlant.waterAmt * 1;
                
                string command = "python cubes.py " + to_string(pauseTime) + " " + to_string(currCubeNo);
                system(command.c_str());
                
                currPlant.hoursTillFeed = currPlant.frequency * 24;

            } else {
                currPlant.hoursTillFeed -= 12;
            }

            plants.at(currPlant.cubeNo-1) = currPlant;
        }
    }
}



/*
 * purpose:   asks user to input plant parameters and stores plant preset 
 *            in presets.txt
 * arguments: none
 * returns:   none
 * effects:   none
 * tested:    yes
 */
void PlantList::createPreset() {
    string filename = "presets.txt";
    Plant newPlant;
    //setting preset cube number to 0
    newPlant.cubeNo = 0;
    cout << "Enter plant name: " << endl;
    
    //loop until a unique name is chosen
    bool uniqueName = 0;
    while(uniqueName == 0){
        cin >> newPlant.name;
        uniqueName = 1;
        for(string plantName : presetNames){
            if(newPlant.name == plantName){
                uniqueName = 0;
                cout << "Plant name already used. Please choose a different name: " << endl;
            }
        }
    }

    presetNames.push_back(newPlant.name);

    cout << "Enter water amount per feeding (ml): " << endl;
    cin >> newPlant.waterAmt;
    cout << "Enter feeding frequency (days): " << endl;
    cin >> newPlant.frequency;
    cout << "Enter ideal moisture level: " << endl;
    cin >> newPlant.idealMoisture;
    newPlant.hoursTillFeed = 0;
    cout << "How many feedings until the plant is an adult? " << endl;
    cin >> newPlant.feedingsTillAdult;
    cout << "How much should it be fed as an adult? " << endl;
    cin >> newPlant.adultWaterAmt;

    //open preset file
    ofstream outfile(filename, ios::app);  

    if (outfile.is_open()) {

        //write plant preset to preset file
        outfile << newPlant.name << setw(20)
                << newPlant.waterAmt << setw(20) << newPlant.frequency << setw(20) << newPlant.idealMoisture
                << setw(20) << newPlant.feedingsTillAdult << setw(20) << newPlant.adultWaterAmt << endl;

        //close preset file
        outfile.close();

        cout << "Preset created!" << endl;
    } else {
        cerr << "Unable to open file." << std::endl;
    }
}

/*
 * purpose:   removes preset from preset.txt by deleting and adding all lines from 
              presets except for the one we want to remove
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::deletePreset() {
    string filename = "presets.txt";
    ifstream infile(filename);
    bool presetFound = 0;
    string plantNameToRemove;
    string line;

    vector<string> outputLines;
    cout << "What is the name of the preset: ('exit' to quit)" << endl;
    cin >> plantNameToRemove; 

    if(plantNameToRemove != "exit") {
        if (!infile.is_open()) {
            cerr << "Error opening presets.txt." << endl;
        }

        //read each line from the file, add 
        while (getline(infile, line)) {
            istringstream iss(line);
            string word;
            
            //read the second word (plant name)
            if (iss >> word) {
                    if(word != plantNameToRemove){
                        outputLines.push_back(line);
                    } else {
                        presetFound = 1;
                    }
            }
        }

        infile.close();

        //write outputLines back to preset.txt
        ofstream outfile(filename);  

        if (!outfile.is_open()) {
            cerr << "Unable to open file." << std::endl;
        }

        for (string currLine : outputLines) {
            outfile << currLine << endl;
        }
            
        //close preset file
        outfile.close();

        if(presetFound){
            cout << "Preset was deleted!" << endl;
        } else {
            cout << "Could not find preset." << endl;
        }
    }
}

/*
 * purpose:   clears all presets
 * arguments: none
 * returns:   none
 * effects:   presets.txt, preset vector
 */
void PlantList::clearPresets() {
    string input;
    string filename = "presets.txt";
    cout << "Are you sure you want to clear all presets? (y/n) " << endl;
    cin >> input;

    if(input == "y") {;
        //clear presets list
        presetNames.clear();

        //clear presets.txt file
        ofstream outfile(filename, ios::trunc);  
        if(!outfile.is_open()){
            cerr << "Unable to open file." << std::endl;
        }
        outfile.close();

        cout << "Presets have been cleared!" << endl;
    }
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
    cout << "Enter a free cube number: " << endl;
    cin >> newPlant.cubeNo;
    if(newPlant.cubeNo > numCubes or newPlant.cubeNo < 1){
        cout << "That cube doesn't exist, retry and pick a different cube." << endl;
    } else if(plants.at(newPlant.cubeNo-1).name != "EMPTY"){
        cout << "That cube number is taken, retry and pick a different cube or delete this cube first" << endl;
    } else {
        cout << "Enter plant name: " << endl;
        cin >> newPlant.name;
        cout << "Enter water amount per feeding (ml): " << endl;
        cin >> newPlant.waterAmt;
        cout << "Enter feeding frequency (days): " << endl;
        cin >> newPlant.frequency;
        cout << "Enter ideal moisture level: " << endl;
        cin >> newPlant.idealMoisture;
        newPlant.hoursTillFeed = 0;
        cout << "How many feedings until the plant is an adult? " << endl;
        cin >> newPlant.feedingsTillAdult;
        cout << "How much should it be fed as an adult? " << endl;
        cin >> newPlant.adultWaterAmt;

        plants.at(newPlant.cubeNo-1) = newPlant;
        storeState();
    }
}

/*
 * purpose:   asks user to choose a preset, creates plant in plant list, 
 *            stores plant in plantState.txt 
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::createPlantFromPreset() {
    string filename = "presets.txt";
    ifstream infile(filename);

    string line;
    string preset; 
    string input;
    string word;

    cout << "What preset would you like to use: ";
    cin >> input;
   
    bool presetFound = 0;

    //read each line and search for preset
    while (getline(infile, line)) {
        istringstream iss(line);
        iss >> word;
        //read the second word (plant name)
        if(input == word){
            presetFound = 1;
            preset = line;
        }
    }

    infile.close();

    if(presetFound == 1){
        //create plant from preset
        istringstream output(preset);
        Plant newPlant;

        cout << "Enter a free cube number: " << endl;
        cin >> newPlant.cubeNo;
       
        if(newPlant.cubeNo > numCubes or newPlant.cubeNo < 1){
        cout << "That cube doesn't exist, retry and pick a different cube." << endl;
        } else if(plants.at(newPlant.cubeNo-1).name != "EMPTY"){
            cout << "That cube number is taken, retry and pick a different cube or delete this cube first" << endl;
        } else {
            output >> word;
            newPlant.name = word;
            output >> word;
            newPlant.waterAmt = stof(word);
            output >> word;
            newPlant.frequency = stof(word);
            output >> word;
            newPlant.idealMoisture = stof(word);
            newPlant.hoursTillFeed = 0;
            output >> word;
            newPlant.feedingsTillAdult = stoi(word);
            output >> word;
            newPlant.adultWaterAmt = stof(word);

            plants.at(newPlant.cubeNo-1) = newPlant;
            storeState();
            cout << "Plant was created!" << endl;
        }
    } else {
        cout << "Preset not found." << endl;
    }
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
    
    if(cubeNumber > numCubes or cubeNumber < 1){
        cout << "That cube doesn't exist" << endl;
    } else {
        if(plants.at(cubeNumber-1).name == "EMPTY"){
            cout << "That cube number doesn't have a plant." << endl;
        } else {
            Plant currPlant = emptyPlant;
            currPlant.cubeNo = plants.at(cubeNumber-1).cubeNo;
            plants.at(cubeNumber-1) = currPlant;
            storeState();
        }
    }
}

/*
 * purpose:   clears all plants
 * arguments: none
 * returns:   none
 * effects:   plantState.txt, plants vector
 */
void PlantList::clearPlants() {
    string input;
    string filename = "plantState.txt";
    cout << "Are you sure you want to clear all plants? (y/n) " << endl;
    cin >> input;

    if(input == "y") {;

        int count = 1;
        //clear plant list
        for(Plant currPlant : plants){
            currPlant = emptyPlant;
            currPlant.cubeNo = count;
            plants.at(count-1) = currPlant;
            count++;
        }

        storeState();

        cout << "Plants have been cleared!" << endl;
    }
}

/*
 * purpose:   none
 * arguments: prints plantState.txt
 * returns:   none
 * effects:   none
 */
void PlantList::displayPlants() {
   //open presets.txt
    string filename = "plantState.txt";
    ifstream infile;
    infile.open(filename);
    string line;

    //create titles for column
    cout << "Cube # |" << " Name | " <<
    " Water Amt |" << " Freq |" << " Ideal Moisture Level |" <<
     " Feeds Till Adult |" << " Adult Water Amt" << endl;

    //print each plant from plants vector
    for (Plant currPlant : plants) {
        cout << currPlant.cubeNo << setw(13) << currPlant.name << setw(10) <<
            currPlant.waterAmt << setw(7) << currPlant.frequency << setw(15) << 
            currPlant.idealMoisture << setw(20) << currPlant.feedingsTillAdult << 
            setw(15) << currPlant.adultWaterAmt << endl;
    }

    //close presets.txt
    infile.close();
}

/*
 * purpose:   none
 * arguments: prints presets.txt
 * returns:   none
 * effects:   none
 */
void PlantList::displayPresets() {
    //open presets.txt
    string filename = "presets.txt";
    ifstream infile;
    infile.open(filename);
    
    string line;

    //create titles for column
    cout << "Cube # |" << " Name | " <<
    " Water Amt |" << " Freq |" << " Ideal Moisture Level |" <<
     " Feeds Till Adult |" << " Adult Water Amt" << endl;

    //print each line from the file
    while (getline(infile, line)) {
        cout << line << endl;
    }

    //close presets.txt
    infile.close();
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
    if(cube1 > numCubes or cube1 < 1){
        cout << "That is not a valid cube number. " << endl;
    } else if(plants.at(cube1-1).name == "EMPTY"){
        cout << "That cube doesn't have a plant stored." << endl;
    } else {
        cout << "Enter cube number of second plant to swap: ";
        cin >> cube2;
        if(cube2 > numCubes or cube2 < 1){
            cout << "That is not a valid cube number. " << endl;
        } else if(plants.at(cube2-1).name == "EMPTY"){
            cout << "That cube doesn't have a plant stored." << endl;
        } else {
            //both cubes are valid
            Plant currPlant = plants.at(cube2-1);
            plants.at(cube2-1) = plants.at(cube1-1);
            plants.at(cube1-1) = currPlant;
            storeState();
        }
    }
}

/*
 * purpose:   none
 * arguments: moves plant to different cube
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::movePlant() {
    int oldCube, newCube;
    cout << "Enter cube number of plant to move: ";
    cin >> oldCube;
    if(oldCube > numCubes or oldCube < 1){
        cout << "That is not a valid cube number. " << endl;
    } else if(plants.at(oldCube-1).name == "EMPTY"){
        cout << "That cube doesn't have a plant stored." << endl;
    } else {
        cout << "Enter new cube number for plant : ";
        cin >> newCube;
        if(newCube > numCubes or newCube < 1){
            cout << "That is not a valid cube number. " << endl;
        } else if(plants.at(newCube-1).name != "EMPTY"){
            cout << "That cube already has a plant stored." << endl;
        } else {
            Plant oldPlant = emptyPlant;
            oldPlant.cubeNo = plants.at(oldCube-1).cubeNo;
            Plant newPlant = plants.at(oldCube-1);
            newPlant.cubeNo = newCube;
            plants.at(newCube-1) = newPlant;
            plants.at(oldCube) = oldPlant;
        }
    }
}

/*
 * purpose:   changes name of a plant at a cube
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeName() {
    int cubeNumber;
    string newName;
    cout << "Enter cube number of plant to change name: ";
    cin >> cubeNumber;

    if(cubeNumber > numCubes or cubeNumber < 1){
        cout << "That is not a valid cube number. " << endl;
    } else if(plants.at(cubeNumber-1).name == "EMPTY"){
        cout << "That cube doesn't have a plant stored." << endl;
    } else {
        cout << "What would you like to change the plant name to: " << endl;
        cin >> newName;
        plants.at(cubeNumber-1).name = newName;
        storeState();
    }
}

/*
 * purpose:   changes the amount to feed a plant each week manually
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeWaterAmount() {
    int cubeNumber;
    float waterAmt;
    cout << "Enter cube number of plant to change watering amount: ";
    cin >> cubeNumber;

    if(cubeNumber > numCubes or cubeNumber < 1){
        cout << "That is not a valid cube number. " << endl;
    } else if(plants.at(cubeNumber-1).name == "EMPTY"){
        cout << "That cube doesn't have a plant stored." << endl;
    } else {
        cout << "What would you like to change the watering amount to: " << endl;
        cin >> waterAmt;
        plants.at(cubeNumber-1).waterAmt = waterAmt;
        storeState();
    }
}

/*
 * purpose:   changes the frequency to feed a plant each week manually
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeFrequency() {
    int cubeNumber;
    float frequency;
    cout << "Enter cube number of plant to change watering frequency: ";
    cin >> cubeNumber;

    if(cubeNumber > numCubes or cubeNumber < 1){
        cout << "That is not a valid cube number. " << endl;
    } else if(plants.at(cubeNumber-1).name == "EMPTY"){
        cout << "That cube doesn't have a plant stored." << endl;
    } else {
        cout << "What would you like to change the watering frequency to: " << endl;
        cin >> frequency;
        plants.at(cubeNumber-1).frequency = frequency;
        storeState();
    }
}

/*
 * purpose:   changes ideal moisture level of a plant
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeMoisture() {
    int cubeNumber;
    float moisture;
    cout << "Enter cube number of plant to change ideal moisture level for: ";
    cin >> cubeNumber;

    if(cubeNumber > numCubes or cubeNumber < 1){
        cout << "That is not a valid cube number. " << endl;
    } else if(plants.at(cubeNumber-1).name == "EMPTY"){
        cout << "That cube doesn't have a plant stored." << endl;
    } else {
        cout << "What would you like to change the ideal moisture level to: " << endl;
        cin >> moisture;
        plants.at(cubeNumber-1).idealMoisture = moisture;
        storeState();
    }
}

/*
 * purpose:   changes adult watering amount of a plant
 * arguments: none
 * returns:   none
 * effects:   plant list, plantState.txt
 */
void PlantList::changeAdultWaterAmt() {
    int cubeNumber;
    float adult;
    cout << "Enter cube number of plant to change adult water amount: ";
    cin >> cubeNumber;

    if(cubeNumber > numCubes or cubeNumber < 1){
        cout << "That is not a valid cube number. " << endl;
    } else if(plants.at(cubeNumber-1).name == "EMPTY"){
        cout << "That cube doesn't have a plant stored." << endl;
    } else {
        cout << "What would you like to change the adult watering amount to: " << endl;
        cin >> adult;
        plants.at(cubeNumber-1).adultWaterAmt = adult;
        storeState();
    }
}

/*
 * purpose:   stores current state of plant list in plantState.txt
 * arguments: none
 * returns:   none
 * effects:   none
 */
void PlantList::storeState(){
    string filename = "plantState.txt";

    //open file to add current state
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    int count = 1;
    //output plant data
    for (Plant currPlant : plants) {
        outfile << count << setw(20) << currPlant.name << setw(20) <<
            currPlant.waterAmt << setw(20) << currPlant.frequency << setw(20) << 
            currPlant.idealMoisture << setw(20) << currPlant.feedingsTillAdult << 
            setw(20) << currPlant.adultWaterAmt << endl;
        count++;
    }

    outfile.close();
}
