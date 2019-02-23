#include "Message_Structure.h"

// RX_Message -- Public Functions /////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    Constructor -- Default
*/
RX_Message::RX_Message()
{
    nSequenceNum = 0;
    fPosition_x_cmd = fPosition_y_cmd = fPosition_z_cmd = 0;
    fVelocity_x_cmd = fVelocity_y_cmd = fVelocity_z_cmd = 0;
    fTorque_x_cmd = fTorque_y_cmd = fTorque_z_cmd = 0;
}

/*-------------------------------------------------------------------------------------------------
    Constructor -- Accepts RX_Message_Structure_Normal_T structure
*/
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

/*-------------------------------------------------------------------------------------------------
    Constructor -- Accepts RX_Message_Structure_T structure
*/
RX_Message::RX_Message(RX_Message_Structure_Recovery_T & stMessage)
{
    nSequenceNum = stMessage.nSequenceNum;
    stStatus = stMessage.stStatus;
    nHashCompare = stMessage.nHashCompare;
}

/*-------------------------------------------------------------------------------------------------
    Destructor -- Default
*/
RX_Message::~RX_Message()
{

}

// TX_Message -- Public Functions  ////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    Constructor -- Default
*/
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

/*-------------------------------------------------------------------------------------------------
    Destructor -- Default
*/
TX_Message::~TX_Message()
{

}

// TX_Message -- Private Functions ////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    encode_MessageStructure --  2 Overloads

    1.
        Encodes the provided TX_Message_Stucture_Normal_T with the information stored within the TX
        message.

        INPUT (TX_Message_Structure_Normal_T &) --  stTX
            Empty message structure which is populated with the TX message data.

        OUTPUT (void)

    2.
        Encodes the provided TX_Message_Stucture_Normal_T with the information stored within the TX
        message.

        INPUT (TX_Message_Structure_Normal_T &) --  stTX
            Empty message structure which is populated with the TX message data.

        OUTPUT (void)
*/

//Overload 1:
void TX_Message::encode_MessageStructure(TX_Message_Structure_Normal_T & stTX)
{
    stTX.fPosition_x_cmd = fPosition_x_cmd;
    stTX.fPosition_y_cmd = fPosition_y_cmd;
    stTX.fPosition_z_cmd = fPosition_z_cmd;
    stTX.fVelocity_x_cmd = fVelocity_x_cmd;
    stTX.fVelocity_y_cmd = fVelocity_y_cmd;
    stTX.fVelocity_z_cmd = fVelocity_z_cmd;
    stTX.fTorque_x_cmd = fTorque_x_cmd;
    stTX.fTorque_y_cmd = fTorque_y_cmd;
    stTX.fTorque_z_cmd = fTorque_z_cmd;
    stTX.fPosition_x = fPosition_x;
    stTX.fPosition_y = fPosition_y;
    stTX.fPosition_z = fPosition_z;
    stTX.fVelocity_x = fVelocity_x;
    stTX.fVelocity_y = fVelocity_y;
    stTX.fVelocity_z = fVelocity_z;
    stTX.fTorque_x = fTorque_x;
    stTX.fTorque_y = fTorque_y;
    stTX.fTorque_z = fTorque_z;
    stTX.fTemp_x = fTemp_x;
    stTX.fTemp_y = fTemp_y;
    stTX.fTemp_z = fTemp_z;
    stTX.fVoltage_x = fVoltage_x;
    stTX.fVoltage_y = fVoltage_y;
    stTX.fVoltage_z = fVoltage_z;
    stTX.fCurrent_x = fCurrent_x;
    stTX.fCurrent_y = fCurrent_y;
    stTX.fCurrent_z = fCurrent_z;
    stTX.fDiffDegrees_x = fDiffDegrees_x;
    stTX.fDiffDegrees_y = fDiffDegrees_y;
    stTX.fDiffDegrees_z = fDiffDegrees_z;
    stTX.fDiffSteps_x = fDiffSteps_x;
    stTX.fDiffSteps_y = fDiffSteps_y;
    stTX.fDiffSteps_z = fDiffSteps_z;
}

//Overload 2:
void TX_Message::encode_MessageStructure(TX_Message_Structure_Recovery_T & stTX)
{
    stTX.fPosition_x = fPosition_x;
    stTX.fPosition_y = fPosition_y;
    stTX.fPosition_z = fPosition_z;
    stTX.fVelocity_x = fVelocity_x;
    stTX.fVelocity_y = fVelocity_y;
    stTX.fVelocity_z = fVelocity_z;
    stTX.fTorque_x = fTorque_x;
    stTX.fTorque_y = fTorque_y;
    stTX.fTorque_z = fTorque_z;
    stTX.fTemp_x = fTemp_x;
    stTX.fTemp_y = fTemp_y;
    stTX.fTemp_z = fTemp_z;
    stTX.fVoltage_x = fVoltage_x;
    stTX.fVoltage_y = fVoltage_y;
    stTX.fVoltage_z = fVoltage_z;
    stTX.fCurrent_x = fCurrent_x;
    stTX.fCurrent_y = fCurrent_y;
    stTX.fCurrent_z = fCurrent_z;
    stTX.fDiffDegrees_x = fDiffDegrees_x;
    stTX.fDiffDegrees_y = fDiffDegrees_y;
    stTX.fDiffDegrees_z = fDiffDegrees_z;
    stTX.fDiffSteps_x = fDiffSteps_x;
    stTX.fDiffSteps_y = fDiffSteps_y;
    stTX.fDiffSteps_z = fDiffSteps_z;
    stTX.nHashCOmpare = nHashCompare;
}