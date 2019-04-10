#include "wrapper.h"

Wrapper::Wrapper(){}

/*********************************************************
*                 WRAPPER SPECIFIC METHODS               *
**********************************************************/

//Method Used To Print Data To The Screen
void Wrapper::printStats(){
    std::cout << "Current Step: " << step << std::endl;
    std::cout << "Desired X: " << desiredPositionX << " Desired Y: " << desiredPositionY << " Desired Z: " << desiredPositionZ << std::endl;
    std::cout << "Arduino X: " << arduinoPositionX << " Arduino Y: " << arduinoPositionY << " Arduino Z: " << arduinoPositionZ << std::endl;
    std::cout << "Trax Heading: " << traxHeading << " Trax Pitch: " << traxPitch << " Trax Roll: " << traxRoll << std::endl;
    std::cout << "*******************************************************" << std::endl;
}

//Method Used To Populate The Private Data Structure allSubPositions
void Wrapper::populateAllSubPositions(){
    //Create All SubPosition Steps For The Data Structure
    //NOTE: CURRENTLY HAS MADE UP DATA
    SubPosition * step1 = new SubPosition(1.0,21.0,36.0,14.0);
    SubPosition * step2 = new SubPosition(2.0,26.0,34.0,43.0);
    SubPosition * step3 = new SubPosition(3.0,23.0,13.0,304.0);
    SubPosition * step4 = new SubPosition(4.0,12.0,93.0,44.0);
    SubPosition * step5 = new SubPosition(5.0,72.0,43.0,47.0);
    SubPosition * step6 = new SubPosition(6.0,90.0,23.0,42.0);
    SubPosition * step7 = new SubPosition(7.0,17.0,39.0,40.0);
    SubPosition * step8 = new SubPosition(8.0,23.0,36.0,64.0);
    SubPosition * step9 = new SubPosition(9.0,29.0,35.0,204.0);
    SubPosition * step10 = new SubPosition(10.0,42.0,345.0,34.0);
    SubPosition * step11 = new SubPosition(11.0,92.0,33.0,44.0);
    SubPosition * step12 = new SubPosition(12.0,72.0,203.0,54.0);
    SubPosition * step13 = new SubPosition(13.0,23.0,333.0,94.0);
    SubPosition * step14 = new SubPosition(14.0,26.0,300.0,104.0);
    SubPosition * step15 = new SubPosition(15.0,28.0,73.0,154.0);
    SubPosition * step16 = new SubPosition(16.0,22.0,13.0,274.0);
    SubPosition * step17 = new SubPosition(17.0,92.0,32.0,304.0);
    SubPosition * step18 = new SubPosition(18.0,200.0,32.0,45.0);

    //Will Populate The Data Structure allSubPositions [Step1,Step2,......Step18]<-Back
    allSubPositions.push_front(step1);
    allSubPositions.push_front(step2);
    allSubPositions.push_front(step3);
    allSubPositions.push_front(step4);
    allSubPositions.push_front(step5);
    allSubPositions.push_front(step6);
    allSubPositions.push_front(step7);
    allSubPositions.push_front(step8);
    allSubPositions.push_front(step9);
    allSubPositions.push_front(step10);
    allSubPositions.push_front(step11);
    allSubPositions.push_front(step12);
    allSubPositions.push_front(step13);
    allSubPositions.push_front(step14);
    allSubPositions.push_front(step15);
    allSubPositions.push_front(step16);
    allSubPositions.push_front(step17);
    allSubPositions.push_front(step18);
}

//Method Used To Check If We Are At The Final Step
bool Wrapper::checkFinalStep(){
    if(step == 18){
        return true;
    }
    return false;
}

//Method Used To Update All Trax Variables And Arduino Variables
void Wrapper::updateAllExternalVariables(){
    //Update All Arduino Variables To Most Recent
    //getArduinoX();
    //getArduinoY();
    //getArduinoZ();

    //Update All Trax Variables To Most Recent
    getTraxHeading();
    getTraxPitch();
    getTraxRoll();
}

//Method Used For Updating The Desired Sub Position To The Next One
void Wrapper::updateDesiredSubPosition(){
    SubPosition * nextPosition;
    nextPosition = allSubPositions.back();
    desiredPositionX = nextPosition->getPositionX();
    desiredPositionY = nextPosition->getPositionY();
    desiredPositionZ = nextPosition->getPositionZ();
    allSubPositions.pop_back();
}

//Method Used To Compare If The Arduino Coordinated Are Within 1 Degree Of The Desired Sub Position
bool Wrapper::comparePointValues(){
    if(desiredPositionX+1 > arduinoPositionX && desiredPositionX-1 < arduinoPositionX && desiredPositionY+1 > arduinoPositionY && desiredPositionY-1 < arduinoPositionY && desiredPositionZ+1 > arduinoPositionZ && desiredPositionZ-1 < arduinoPositionZ){
        return true;
    }
    return false;
}

//Method For Calibration
bool Wrapper::calibrationInit(){
    if(initTraxSettings()){
        delayBy(2);
        if(startTraxCalibration()){
            delayBy(2);
            return true;
        }
    }
    return false;
}

//The Actual Logic Structure Behind The Calibration
void Wrapper::calibrationLoop(){

    while(1){
        //Formulate TX
        //Send TX
        //Delay
        //Get RX
        updateAllExternalVariables();
        if(comparePointValues()){
            if(takeTraxPoint()){
                if(checkFinalStep()){
                    if(getTraxCalScore()){
                        if(saveTraxCal()){
                            if(setDefaultTraxSettings()){
                               std::cout << "Successfully Calibrated!" << std::endl;
                               break;
                            }
                        }
                    }
                    else{
                        if(abortTraxCal()){
                            std::cout << "Aborted Successfully" << std::endl;
                        }
                        break;
                    }
                }
                else{ //Not The 18th Step Need To Update Desired Position To Next Step
                    updateDesiredSubPosition();
                }
            }
        }
    }
}

//Method For Delay In Seconds
void Wrapper::delayBy(int amount){
    printMessage("DELAY");
    for(int i = 0; i < amount; i++){ //Sleep For 30 Seconds
        usleep(1000000); //One Second Delay
    }
}

//Method For Debug Print Statements
void Wrapper::printMessage(std::string s){
    std::cout << s << std::endl;
}

/*********************************************************
*                   TRAX USE METHODS                     *
**********************************************************/

//Method Used To Initialize Trax Settings And Handle Errors Thrown
bool Wrapper::initTraxSettings(){
    int results = tm.initCal();
    if(results == -1){
        std::cout << "Error: Init Of Calibration Failed" << std::endl;
        return false;
    }
    return true;
}

//Method Used For Starting The Trax Calibration Procedure
bool Wrapper::startTraxCalibration(){
    try{ //Try A startCal
        tm.startCal();
        return true; //Positive Return
    }
    catch(...){
        std::cout << "Error: Start Calibration Failed" << std::endl;
        return false; //Negative Return
    }
}

//Method For Taking A Point For The Trax
bool Wrapper::takeTraxPoint(){
    int results = tm.takePoint();
    for(int i = 0; i < 10; i++){ //Loop For Attempting 10 Times
        if(results == 0){ //Check If Pass
            return true; //Positive Return
        }
    }
    std::cout << "Error: Take Point Failed (Attempted 10 Times)" << std::endl;
    return false; //Negative Return
}

//Method For Getting The Trax Calibration Score (Good Or Bad Eh)
bool Wrapper::getTraxCalScore(){
    int results = tm.getCalScore();
    if(results == -1){ //Check For Fail
        std::cout << "Error: Calibration Failed" << std::endl;
        return false;
    }
    return true;
}

//Will Save The Settings To The Trax If A Good Calibration Occurs
bool Wrapper::saveTraxCal(){
    int results = tm.save();
    if(results == -1){
        std::cout << "Error: Save Failed" << std::endl;
        return false;
    }
    return true;
}

//Will Abort The Calibration Procedure For The Trax
bool Wrapper::abortTraxCal(){
    try{ //Will Attempt A Abort
        tm.abortCal();
        return true;
    }
    catch(...){
        std::cout << "Error: Abort Calibration Failed" << std::endl;
        return false;
    }
}

//Will Reset The Default Settings For The Trax
bool Wrapper::setDefaultTraxSettings(){
    int results = tm.setDefaultSettings();
    if(results == -1){
        std::cout << "Error: Set Default Settings Failed" << std::endl;
        return false;
    }
    return true;
}

//Method For Setting The Heading Variable Accordingly
void Wrapper::getHeadingFromTrax(){
    try{
        float tempHeading = tm.getHeading();
        setTraxHeading(tempHeading);
    }
    catch(...){
        std::cout << "Error: Get Trax Heading Failed" << std::endl;
    }
}

//Method For Setting The Pitch Variable Accordingly
void Wrapper::getPitchFromTrax(){
    try{
        float tempPitch = tm.getPitch();
        setTraxPitch(tempPitch);
    }
    catch(...){
        std::cout << "Error: Get Trax Pitch Failed" << std::endl;
    }
}

//Method For Setting The Roll Variable Accordingly
void Wrapper::getRollFromTrax(){
    try{
        float tempRoll = tm.getRoll();
        setTraxRoll(tempRoll);
    }
    catch(...){
        std::cout << "Error: Get Trax Roll Failed" << std::endl;
    }
}

//Method For Getting All The Trax Variables
void Wrapper::getAllTraxHPR(){
    std::cout << "Entered getAllTraxHPR" << std::endl;
    if(tm.getPosition() != -1){
        std::cout << "Got Position" << std::endl;
        getTraxHeading();
        getTraxPitch();
        getTraxRoll();
    }
    else{
        std::cout << "Couldnt Update Position For Trax" << std::endl;
    }
}

void Wrapper::printAllTraxData(){
    std::cout << "TRAX DATA: " << std::endl;
    std::cout << "Heading: " << traxHeading << std::endl;
    std::cout << "Pitch: " << traxPitch << std::endl;
    std::cout << "Roll: " << traxRoll << std::endl;
}

//Method For A Quick Test On If Everything Is Behaving Correctly
bool Wrapper::testTraxMethods(){
    //DEBUG: Temp Tests For Debug With Braden
    if(initTraxSettings()){
        printMessage(("initTraxSettings Passed"));
        if(startTraxCalibration()){
            printMessage("startTraxCalibration Passed");
            if(takeTraxPoint()){
                //printMessage("takeTraxPoint Passed");
                return true;
            }
        }
    }
    printMessage("Failed");
    return false;

    //Test For Full Calibration
    /*if(initTraxSettings()){ //Initialize Trax Settings
        std::cout <<"Passed initTraxSettings" << std::endl;
        if(startTraxCalibration()){ //Start Calibration
            std::cout <<"Passed startTraxCalibration" << std::endl;
            if(takeTraxPoint()){
                getAllTraxHPR();
                printAllTraxData();
                std::cout <<"Passed getAllTraxHPR" << std::endl;
                if(abortTraxCal()){
                    std::cout <<"Passed abortTraxCal" << std::endl;
                    if(setDefaultTraxSettings()){
                        std::cout << "Great Test!" << std::endl;
                        return true;
                    }
                }
            }
        }
    }
    std::cout << "Error: Not Great Test" << std::endl;
    return false;*/
}

/*********************************************************
*                   LOCAL MAILBOX METHODS                *
**********************************************************/

//Method For Getting Arduino X Variable From LocalMailbox
void setArduinoX(){
    //[TD] Fill In From LM
    //arduinoPositionX = lm.getX();
}

//Method For Getting Arduino Y Variable From LocalMailbox
void setArduinoY(){
    //[TD] Fill In From LM
}

//Method For Getting Arduino Z Variable From LocalMailbox
void setArduinoZ(){
    //[TD] Fill In From LM
}

//Method For Getting All Arduino XYZ
void setAllArduinoXYZ(){
    setArduinoX();
    setArduinoY();
    setArduinoZ();
}
