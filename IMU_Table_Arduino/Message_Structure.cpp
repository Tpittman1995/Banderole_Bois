#include "Message_Structure.h"

// RX_Message /////////////////////////////////////////////////////////////////////////////////////

RX_Message::RX_Message()
{
    nSequenceNum = 0;
    stStatus = 0;
    fPosition_x_cmd = fPosition_y_cmd = fPosition_z_cmd = 0;
    fVelocity_x_cmd = fVelocity_y_cmd = fVelocity_z_cmd = 0;
    fTorque_x_cmd = fTorque_y_cmd = fTorque_z_cmd = 0;
}

RX_Message::~RX_Message()
{

}

void RX_Message::decode_RX(char cBuff[])
{
    //Load data from the buffer into the RX message structure
    nSequenceNum = *((unsigned long *)&cBuff[0]);       //1)    Message sequence number

    stStatus = cBuff[4];                                //2)    Status flags

    fPosition_x_cmd = *((unsigned long *)&cBuff[5]);    //3)    Position commands
    fPosition_y_cmd = *((unsigned long *)&cBuff[9]);
    fPosition_z_cmd = *((unsigned long *)&cBuff[13]);

    fVelocity_x_cmd = *((unsigned long *)&cBuff[17]);   //4)    Velocity commands
    fVelocity_y_cmd = *((unsigned long *)&cBuff[21]);
    fVelocity_z_cmd = *((unsigned long *)&cBuff[25]);

    fTorque_x_cmd = *((unsigned long *)&cBuff[29]);     //5)    Torque commands
    fTorque_y_cmd = *((unsigned long *)&cBuff[33]);
    fTorque_z_cmd = *((unsigned long *)&cBuff[37]);
}

// TX_Message /////////////////////////////////////////////////////////////////////////////////////

TX_Message::TX_Message()
{

}

TX_Message::~TX_Message()
{

}

void TX_Message::encode_TX(char cBuff[]) const
{
    //Load data from the TX message structure into the buffer
    memcpy(&cBuff[0], &nSequenceNum, 4);            //1)    Message sequence number

    memcpy(&cBuff[4], &stStatus, sizeof(stStatus)); //2)    Status Flags

    memcpy(&cBuff[7], &fPosition_x_cmd, 4);         //3)    Echoed commands
    memcpy(&cBuff[11], &fPosition_y_cmd, 4);
    memcpy(&cBuff[15], &fPosition_z_cmd, 4);
    memcpy(&cBuff[19], &fVelocity_x_cmd, 4);
    memcpy(&cBuff[23], &fVelocity_y_cmd, 4);
    memcpy(&cBuff[27], &fVelocity_z_cmd, 4);
    memcpy(&cBuff[31], &fTorque_x_cmd, 4);
    memcpy(&cBuff[35], &fTorque_y_cmd, 4);
    memcpy(&cBuff[39], &fTorque_z_cmd, 4);

    memcpy(&cBuff[43], &fPosition_x, 4);         //4)    Axis readout data
    memcpy(&cBuff[47], &fPosition_y, 4);
    memcpy(&cBuff[51], &fPosition_z, 4);
    memcpy(&cBuff[55], &fVelocity_x, 4);
    memcpy(&cBuff[59], &fVelocity_y, 4);
    memcpy(&cBuff[63], &fVelocity_z, 4);
    memcpy(&cBuff[67], &fTorque_x, 4);
    memcpy(&cBuff[71], &fTorque_y, 4);
    memcpy(&cBuff[75], &fTorque_z, 4);
    memcpy(&cBuff[79], &fTemp_x, 4);
    memcpy(&cBuff[83], &fTemp_y, 4);
    memcpy(&cBuff[87], &fTemp_z, 4);
    memcpy(&cBuff[91], &fVoltage_x, 4);
    memcpy(&cBuff[95], &fVoltage_y, 4);
    memcpy(&cBuff[99], &fVoltage_z, 4);
    memcpy(&cBuff[103], &fCurrent_x, 4);
    memcpy(&cBuff[107], &fCurrent_y, 4);
    memcpy(&cBuff[111], &fCurrent_z, 4);
    memcpy(&cBuff[115], &fDiffDegrees_x, 4);
    memcpy(&cBuff[119], &fDiffDegrees_y, 4);
    memcpy(&cBuff[123], &fDiffDegrees_z, 4);
    memcpy(&cBuff[127], &fDiffSteps_x, 4);
    memcpy(&cBuff[131], &fDiffSteps_y, 4);
    memcpy(&cBuff[135], &fDiffSteps_z, 4);
}