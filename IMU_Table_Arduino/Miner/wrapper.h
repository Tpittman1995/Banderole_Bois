#ifndef WRAPPER_H
#define WRAPPER_H
/*********************************************************
*                      FILE INCLUDES                     *
**********************************************************/
#include "subposition.h"
#include "traxmailbox.h"

/*********************************************************
*                    LIBRARY INCLUDES                    *
**********************************************************/
#include <iostream>
#include <list>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <unistd.h>

class Wrapper
{
public:
/*********************************************************
*                      CONSTRUCTORS                      *
**********************************************************/
    Wrapper();

/*********************************************************
*                         SETTERS                        *
**********************************************************/
    void setDesiredPositionX(float newDesiredPositionX){desiredPositionX = newDesiredPositionX;}
    void setDesiredPositionY(float newDesiredPositionY){desiredPositionY = newDesiredPositionY;}
    void setDesiredPositionZ(float newDesiredPositionZ){desiredPositionZ = newDesiredPositionZ;}
    void setArduinoPositionX(float newArduinoPositionX){arduinoPositionX = newArduinoPositionX;}
    void setArduinoPositionY(float newArduinoPositionY){arduinoPositionY = newArduinoPositionY;}
    void setArduinoPositionZ(float newArduinoPositionZ){arduinoPositionZ = newArduinoPositionZ;}
    void setTraxHeading(float newTraxHeading){traxHeading = newTraxHeading;}
    void setTraxPitch(float newTraxPitch){traxPitch = newTraxPitch;}
    void setTraxRoll(float newTraxRoll){traxRoll = newTraxRoll;}
    void setStep(float newStep){step = newStep;}

/*********************************************************
*                         GETTERS                        *
**********************************************************/
    float getDesiredPositionX(){return desiredPositionX;}
    float getDesiredPositionY(){return desiredPositionY;}
    float getDesiredPositionZ(){return desiredPositionZ;}
    float getArduinoPositionX(){return arduinoPositionX;}
    float getArduinoPositionY(){return arduinoPositionY;}
    float getArduinoPositionZ(){return arduinoPositionZ;}
    float getTraxHeading(){return traxHeading;}
    float getTraxPitch(){return traxPitch;}
    float getTraxRoll(){return traxRoll;}
    float getStep(){return step;}

/*********************************************************
*                 WRAPPER SPECIFIC METHODS               *
**********************************************************/
    void populateAllSubPositions();
    void printStats();
    bool checkFinalStep();
    void updateAllExternalVariables();
    void updateDesiredSubPosition();
    bool comparePointValues();
    bool calibrationInit();
    void calibrationLoop();
    void delayBy(int amount);
    void printMessage(std::string s);

/*********************************************************
*                   TRAX USE METHODS                     *
**********************************************************/
    bool initTraxSettings();
    bool startTraxCalibration();
    bool takeTraxPoint();
    bool getTraxCalScore();
    bool saveTraxCal();
    bool abortTraxCal();
    bool setDefaultTraxSettings();
    void getHeadingFromTrax();
    void getPitchFromTrax();
    void getRollFromTrax();
    void getAllTraxHPR();
    bool testTraxMethods();
    void printAllTraxData();

/*********************************************************
*                   LOCAL MAILBOX METHODS                *
**********************************************************/
    void getArduinoX();
    void getArduinoY();
    void getArduinoZ();
    void getAllArduinoXYZ();
    void formulateFirstTX();

private:
    //Used For Desired Sub Position
    float desiredPositionX, desiredPositionY, desiredPositionZ;

    //Used For Current Arduino Positioning
    float arduinoPositionX, arduinoPositionY, arduinoPositionZ;

    //Used For Current Trax Positioning
    float traxHeading, traxPitch, traxRoll;

    //Current Step Variable
    float step;

    //Data Structure To Hold All Sub Positions
    std::list <SubPosition *> allSubPositions;

    //Private TraxMailbox Object Named tm
    TraxMailbox tm;

    //Private Mailbox Object Named lm
    //LocalMailbox lm;

};

#endif // WRAPPER_H
