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

    //Create and send acknowledgement letter
    Letter_T lLetter;
    lLetter.cMessageType = cMailboxStatus;
    lLetter.nMessageLength = 0;
    lLetter.cData = 0;
    lLetter.nCRC = 0;
    TX_USB(lLetter);

    //Process RX message
    Process_RX();

    //Update state machine
    stMailboxState = updateStateMachine();

    //Create response message
    Process_TX();

    //TX Message to master
    bool bTX_Acked = false;
    while(!bTX_Acked)
    {
        TX_USB();

        //Wait for acknowledgement letter
        
    }


}

void Mailbox::RX_USB()
{
    //Check for unprocessed message in buffer or nonexistant serial event
    if(Check_RX_Buf_Ready() || !Check_RX_Event())
        return;
    
    static unsigned long nAccumulate_ms = 0;
    Letter_T lLetter;

    if((nAccumulate_ms += millis()) < _LOC_TIMEOUT_MS) //If no timeout has occured
    {
        int nRX_Length = RX_USB(lLetter);

        if(lLetter.nMessageLength == nRX_Length) //If the message reports its correct length
        {
            if(checkCRC(lLetter.cData, lLetter.nCRC)) //If the CRC returns no errors
            {
                cMasterStatus = lLetter.cMessageType
                memcpy(cRX_Buf, lLetter.cData, nRX_Message_Length); //Copy the message to the RX buffer
                Set_RX_Buf_Ready(true);                             //Flag the RX buffer as updated
                cMailboxStatus = Com_Code_T::eAck;

                nAccumulate_ms = 0;                                 //Reset the accumulated milliseconds

                return;
            }
        }

        cMailboxStatus = Com_Code_T::eNack;
        return;
    }

    induce_LOC(); //Else continue with Loss of Coms
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
    lLetter.nCRC = computeCRC(lLetter.cData);

    TX_USB(lLetter);
}

//  Mailbox -- Private Functions //////////////////////////////////////////////////////////////////

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

void Mailbox::induce_LOC()
{
    switch(stMailboxState)
    {
    case MailboxState_T::eLOC_1:

        stNext_MailboxState = MailboxState_T::eLOC_2;
        break;

    case MailboxState_T::eLOC_2:

        stNext_MailboxState = MailboxState_T::eLOC_3;
        break;

    case MailboxState_T::eLOC_3:

        stNext_MailboxState = MailboxState_T::eRecovery_LOC;
        break;

    default:

        stNext_MailboxState = MailboxState_T::eLOC_1;
        break;
    }
}

void Mailbox::TX_USB(Letter_T & lLetter)
{
    Serial.write(lLetter.cMessageType);
    Serial.write(lLetter.nMessageLength);
    Serial.write(lLetter.cData, lLetter.nMessageLength);
    Serial.write(lLetter.nCRC);
    Serial.write(lLetter.nStopByte);
}