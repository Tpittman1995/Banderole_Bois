#ifndef WRAPPER_H
#define WRAPPER_H
#include <stdlib.h>
#include <string>
class Wrapper
{
public:
    //CONSTRUCTOR
    Wrapper();

    //SETTERS
    void setPositionX(float newPositionX){positionX = newPositionX;}
    void setPositionY(float newPositionY){positionY = newPositionY;}
    void setPositionZ(float newPositionZ){positionZ = newPositionZ;}
    void setVelocityX(float newVelocityX){velocityX = newVelocityX;}
    void setVelocityY(float newVelocityY){velocityY = newVelocityY;}
    void setVelocityZ(float newVelocityZ){velocityZ = newVelocityZ;}
    void setTorqueX(float newTorqueX){torqueX = newTorqueX;}
    void setTorqueY(float newTorqueY){torqueY = newTorqueY;}
    void setTorqueZ(float newTorqueZ){torqueZ = newTorqueZ;}
    void setCurrentX(float newCurrentX){currentX = newCurrentX;}
    void setCurrentY(float newCurrentY){currentY = newCurrentY;}
    void setCurrentZ(float newCurrentZ){currentZ = newCurrentZ;}
    void setVoltageX(float newVoltageX){voltageX = newVoltageX;}
    void setVoltageY(float newVoltageY){voltageY = newVoltageY;}
    void setVoltageZ(float newVoltageZ){voltageZ = newVoltageZ;}
    void setTempX(float newTempX){tempX = newTempX;}
    void setTempY(float newTempY){tempY = newTempY;}
    void setTempZ(float newTempZ){tempZ = newTempZ;}
    void setDiffDegreesX(float newDiffX){diffDegreesX = newDiffX;}
    void setDiffDegreesY(float newDiffY){diffDegreesY = newDiffY;}
    void setDiffDegreesZ(float newDiffZ){diffDegreesZ = newDiffZ;}
    void setDiffStepsX(float newDiffX){diffStepsX = newDiffX;}
    void setDiffStepsY(float newDiffY){diffStepsY = newDiffY;}
    void setDiffStepsZ(float newDiffZ){diffStepsZ = newDiffZ;}
    void setCurrentMode(std::string newMode){currentMode = newMode;}
    void setCurrentInstruction(int newInstruction){currentInstruction = newInstruction;}
    //GETTERS
    float getPositionX(){return positionX;}
    float getPositionY(){return positionY;}
    float getPositionZ(){return positionZ;}
    float getVelocityX(){return velocityX;}
    float getVelocityY(){return velocityY;}
    float getVelocityZ(){return velocityZ;}
    float getTorqueX(){return torqueX;}
    float getTorqueY(){return torqueY;}
    float getTorqueZ(){return torqueZ;}
    float getCurrentX(){return currentX;}
    float getCurrentY(){return currentY;}
    float getCurrentZ(){return currentZ;}
    float getVoltageX(){return voltageX;}
    float getVoltageY(){return voltageY;}
    float getVoltageZ(){return voltageZ;}
    float getTempX(){return tempX;}
    float getTempY(){return tempY;}
    float getTempZ(){return tempZ;}
    float getDiffDegreesX(){return diffDegreesX;}
    float getDiffDegreesY(){return diffDegreesY;}
    float getDiffDegreesZ(){return diffDegreesZ;}
    float getDiffStepsX(){return diffStepsX;}
    float getDiffStepsY(){return diffStepsY;}
    float getDiffStepsZ(){return diffStepsZ;}
    std::string getCurrentMode(){return currentMode;}
    int getCurrentInstruction(){return currentInstruction;}

    void printAll();

    //

    //Start Process
    
    //Recall


private:
    float positionX, positionY, positionZ;
    float velocityX, velocityY, velocityZ;
    float torqueX, torqueY, torqueZ;
    float currentX, currentY, currentZ;
    float voltageX, voltageY, voltageZ;
    float tempX, tempY, tempZ;
    float diffDegreesX, diffDegreesY, diffDegreesZ;
    float diffStepsX, diffStepsY, diffStepsZ;
    std::string currentMode;
    int currentInstruction;
    //Store The Procedure?
    //Trax MailBox Object
    //Local MailBox Object

};

#endif // WRAPPER_H
