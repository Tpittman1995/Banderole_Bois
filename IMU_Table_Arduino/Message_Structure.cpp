#include "Message_Structure.h"

// RX_Message /////////////////////////////////////////////////////////////////////////////////////

RX_Message::RX_Message()
{
    nSequenceNum = 0;
    fPosition_x_cmd = fPosition_y_cmd = fPosition_z_cmd = 0;
    fVelocity_x_cmd = fVelocity_y_cmd = fVelocity_z_cmd = 0;
    fTorque_x_cmd = fTorque_y_cmd = fTorque_z_cmd = 0;
}

RX_Message::RX_Message(RX_Message_Structure_Normal_T & stMessage)
{
    nSequenceNum = stMessage.nSequenceNum;
    stStatus = stMessage.stStatus;
    fPosition_x_cmd = stMessage.fPosition_x_cmd;
    fPosition_y_cmd = stMessage.fPosition_y_cmd;
    fPosition_z_cmd = stMessage.fPosition_z_cmd;
    fVelocity_x_cmd = stMessage.fVelocity_x_cmd;
    fVelocity_y_cmd = stMessage.fVelocity_y_cmd;
    fVelocity_z_cmd = stMessage.fVelocity_z_cmd;
    fTorque_x_cmd = stMessage.fTorque_x_cmd;
    fTorque_y_cmd = stMessage.fTorque_y_cmd;
    fTorque_z_cmd = stMessage.fTorque_z_cmd;
}

RX_Message::RX_Message(RX_Message_Structure_Recovery_T & stMessage)
{
    
}

RX_Message::~RX_Message()
{

}

// TX_Message /////////////////////////////////////////////////////////////////////////////////////

TX_Message::TX_Message()
{
    nSequenceNum = nHashCompare = 0;
    fPosition_x_cmd = fPosition_y_cmd = fPosition_z_cmd = 0;
    fVelocity_x_cmd = fVelocity_y_cmd = fVelocity_z_cmd = 0;
    fTorque_x_cmd = fTorque_y_cmd = fTorque_z_cmd = 0;
    fPosition_x = fPosition_y = fPosition_z = 0;
    fVelocity_x = fVelocity_y = fVelocity_z = 0;
    fTorque_x = fTorque_y = fTorque_z = 0;
    fTemp_x = fTemp_y = fTemp_z = 0;
    fVoltage_x = fVoltage_y = fVoltage_z = 0;
    fCurrent_x = fCurrent_y = fCurrent_z = 0;
    fDiffDegrees_x = fDiffDegrees_y = fDiffDegrees_z = 0;
    fDiffSteps_x = fDiffSteps_y = fDiffSteps_z = 0;
}

TX_Message::~TX_Message()
{

}