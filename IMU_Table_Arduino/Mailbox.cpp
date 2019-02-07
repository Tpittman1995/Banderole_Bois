#include "Mailbox.h"

//  Mailbox -- Public Functions ///////////////////////////////////////////////////////////////////
Mailbox::Mailbox()
{
    bRX_Ready = false;
    bTX_Ready = false;
    bRX_Buf_Ready = false;
    bTX_Buf_Ready = false;

    cMailboxStatus = Com_Code_T::eStart;
    stMailboxState = stNext_MailboxState = MailboxState_T::eNormal;
    nRX_Message_Length = _RX_MESSAGE_LENGTH;
    nTX_Message_Length = _TX_MESSAGE_LENGTH;
}

Mailbox::~Mailbox()
{

}

void Mailbox::runFrame_USB()
{
    //RX Message from master
    RX_USB();

    //Process RX message
    Process_RX();

    //Update state machine
    stMailboxState = updateStateMachine();

    //Create response message
    Process_TX();

    //TX Message to master
    TX_USB();
}

//  Mailbox -- Private Functions //////////////////////////////////////////////////////////////////

bool Process_RX()
{
    //Check to see if the RX buffer is ready for processing
    if(!Check_RX_Buf_Ready())
        return false;
    
    //Begin parsing the buffered message based on the master's message code

    switch(stMasterState)
    {
    case MailboxState_T::eNormal:

        //Create message structure
        RX_Message_Structure_Normal_T * stRX_msg = (RX_Message_Structure_Normal_T *)cRX_Buf;

        mRX = RX_Message(stRX_msg);
        break;

    case MailboxState_T::eLOC_1:

        break;

    case MailboxState_T::eLOC2:

        break;

    case MailboxState_T::eLOC3:

        break;

    case MailboxState_T::eRecovery_LOC:

        break;

    case MailboxState_T::eRecovery:

        break;

    default:
        break;
    }
}

int Mailbox::RX_USB(Letter_T & lLetter)
{
    //Else continue to fill up the buffer
    int i = 0, j = 0;
    char cData[_RX_MESSAGE_LENGTH];

    //Grab letter headers
    while(Serial.available() && (i<2))
    {
        cData[i] = Serial.read();
        i++;
        j++;
    }

    if(j == 2)
    {
        lLetter.cMessageType = (Com_Code_T)(*(uint16_t *)(&cData[0]));
        lLetter.nMessageLength = (*(uint16_t *)(&cData[1]));
    }
    else
        return j;
    

    //Grab message data
    i = 0;

    while(Serial.available() && (i < lLetter.nMessageLength))
    {
        cData[i] = Serial.read();
        i++;
        j++;
    }

    memcpy(lLetter.cData, cData, i);

    //Grab the CRC and stop bit
    i = 0;

    while((cData[i] != 0xE7) && (i < 4))
    {
        cData[i] = Serial.read();
        i++;
        j++;
    }

    return j+1;
}

void Mailbox::RX_USB()
{
    //Check for unprocessed message in buffer or nonexistant serial event
    if(Check_RX_Buf_Ready() || !Check_RX_Event())
        return;
    
    unsigned long tCurrentCall;
    static unsigned long tPrevCall = 0;
    Letter_T lLetter;

    if(Check_Timeout()) //If a timeout has occured
    {
        //Update the timer and send message
        tCurrentCall = millis();
        Update_TimeoutCounter(tCurrentCall - tPrevCall);
        nPrevCall = tCurrentCall;
        
        int nRX_Length = RX_USB(lLetter);

        if(lLetter.nMessageLength == nRX_Length) //If the message reports its correct length
        {
            if(checkCRC(lLetter)) //If the CRC returns no errors
            {
                stMasterStatus = lLetter.cMessageType
                memcpy(cRX_Buf, lLetter.cData, nRX_Message_Length); //Copy the message to the RX buffer
                Set_RX_Buf_Ready(true);                             //Flag the RX buffer as updated
                return;
            }        
        }
    }
    
    induce_LOC(); //Move into Loss of Coms
}

void Mailbox::TX_USB()
{
    if(Check_TX_Buf_Ready()) //If there is a not a processed message that is ready to send
        return;
    
    //Else create and send a letter
    Letter_T lLetter;

    lLetter.cMessageType = cMailboxStatus;
    lLetter.nMessageLength = nTX_Message_Length;
    lLetter.cData = cTX_Buf;
    lLetter.nCRC = computeCRC(lLetter);

    TX_USB(lLetter);
}

void Mailbox::TX_USB(Letter_T & lLetter)
{
    Serial.write(lLetter.cMessageType);
    Serial.write(lLetter.nMessageLength);
    Serial.write(lLetter.cData, lLetter.nMessageLength);
    Serial.write(lLetter.nCRC);
    Serial.write(lLetter.nStopByte);
}

// Private Operator Overloads /////////////////////////////////////////////////////////////////////
MailboxState_T & operator= (MailboxState_T & LHS, Com_Code_T & RHS)
{
    if(RHS == Com_Code_T::eStart)
        LHS = MailboxState_T::eNormal;
    else
        LHS = (MailboxState_T)((int)RHS);
    
    return LHS;
}