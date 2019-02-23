#include "Mailbox.h"

//  Mailbox -- Public Functions ///////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    Constructor -- Default
*/
Mailbox::Mailbox()
{
    bRX_Ready = false;
    bTX_Ready = false;
    bRX_Buf_Ready = false;
    bTX_Buf_Ready = false;

    stMailboxState = stNext_MailboxState = MailboxState_T::eStart;
    nRX_Message_Length = _RX_MESSAGE_LENGTH;
    nTX_Message_Length = _TX_MESSAGE_LENGTH;
}

/*-------------------------------------------------------------------------------------------------
    Destructor -- Default
*/
Mailbox::~Mailbox()
{

}

/*-------------------------------------------------------------------------------------------------
    runFrame_USB

    Runs a single process frame which executes the mailbox's primary functions through the Arduino's
    USB serial port.

    INPUT   --  NONE

    Output (void)
*/
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

bool Mailbox::checkCRC(Letter_T & lLetter)
{
    //Create a string with the length of the letter minus the stop byte
    int nStr_Len = sizeof(lLetter) + strlen(lLetter.cData) - 1;
    char cData[nStr_Len];

    //Copy over the letter header
    cData[0] = lLetter.cMessageType;
    cData[1] = lLetter.nMessageLength;

    //Copy over the message
    memcpy(&cData[2], lLetter.cData, strlen(lLetter.cData));
    
    //Copy over the CRC
    cData[2+nStr_Len] = HI_16(lLetter.nCRC);
    cData[3+nStr_Len] = LO_16(lLetter.nCRC);

    if(!computeCRC(cData, *((uint16_t *)cData), nStr_Len);
        return true;
    else
        return false;
    
}

/*-------------------------------------------------------------------------------------------------
    RX_USB

    Secondary subprocess for receiving a command message from the master device through the Arduino's 
    USB serial port.

    INPUT (Letter_T &)  --  lLetter
        An empty letter structure ready to be populated. Passed by reference.

    OUTPUT (int)
        Returns the number of characters that was read in through the serial event.
*/
int Mailbox::RX_USB(Letter_T & lLetter)
{
    //Else continue to fill up the buffer
    int i = 0;
    char cData[_RX_MESSAGE_LENGTH_NORMAL];

    //Grab letter headers
    while(Serial.available() && (i<2))
    {
        cData[i] = Serial.read();
        i++;
    }

    //If the header was read in correctly
    if(i == 2)
    {
        lLetter.cMessageType = (uint8_t)cData[0];
        lLetter.nMessageLength = (uint8_t)cData[1];
    }
    else
        return 0;

    //Read in cData and the tail of the letter
    while(Serial.available() && (i < lLetter.nMessageLength) && cData[i] != lLetter.nStopByte)
    {
        cData[i] = Serial.read();
        i++;
    }

    memcpy(lLetter.cData, &cData[2], nRX_Message_Length);   //Copy cData to the letter's cData
    lLetter.nCRC = (*((uint16_t *)(&cData[i-3])) & 0xEFFF); //Load in the 15 bit CRC

    return i;
}

//TODO
uint16_t Mailbox::computeCRC(Letter_T & lLetter)
{
    //Create a string with the length of the letter minus the stop bytes
    int nStr_Len = sizeof(lLetter) + strlen(lLetter.cData) - 1;
    char cData[nStr_Len];

    //Copy over the letter header
    cData[0] = lLetter.cMessageType;
    cData[1] = lLetter.nMessageLength;

    //Copy over the message
    memcpy(&cData[2], lLetter.cData, strlen(lLetter.cData));

    //The remaining 2 bytes will remain 0, as we concatenate 15 0 bits to the end of the string
    //Compute CRC with the first 16 bits of cData as the first numerator
    CRC_Bitfield_T nBitfield = computeCRC(cData, *((uint16_t *)(cData)), nSTr_Len);

    return nBitfield.nNum;
}

CRC_Bitfield_T Mailbox::computeCRC(char * cStr, uint16_t nPassDown, int nLen)
{
    int nPos = 0;
    uint16_t nProduct, nNumerator = nPassDown, nDenominator;

    //Iterate through string
    while(nPos <= nLen)
    {
        if(!(nNumerator & 0x8000))  //If the first bit of the passed down value is 0
            nDenominator - 0;           //Set the second value to 0
        else                        //Else
            nDenominator = 0xAAAA;      //Set it to the divisor

        //Calculate product for the current 16 bit window
        nNumerator = (nPassdown << 1);                                      //Shift the passdown to the right by 1
        nNumerator += ((uint16_t *)(cStr+(nPos/8)) & (0x80 >> (nPos % 8))); //Add the next bit in the string
        nNumerator ^ nDenominator;                                          //Xor the numerator and denominator   
    
        nPos++;
    }

    //Create the CRC bitfield and load the product into it
    CRC_Bitfield_T nCRC;
    CRC.nNum = nNumerator & 0x7FFF;

    return nCRC;
}

MailboxState_T Mailbox::updateStateMachine()
{
    switch(stNext_MailboxState)
    {
    case MailboxState_T::eStart:

        //Once a TX message is ready, shift to normal state
        if(bTX_Ready)
            return MailboxState_T::eNormal;

        //Otherwise remain in startup
        return MailboxState_T::eStart;

        break;

    case MailboxState_T::eNormal:

        //If loss of coms is induced, shift to LOC_1
        if(bLOC_Induced)
            return MailboxState_T::eLOC_1;

        //If the master device is still in LOC, shift to LOC_1
        if(is_LOC(stMasterState))
            return MailboxState_T::eLOC_1;

        //Otherwise continue normal operations
        return MailboxState_T::eNormal;

        break;

    case MailboxState_T::eLOC_1:

        //If loss of coms is induced, shift to LOC_2
        if(bLOC_Induced)
            return MailboxState_T::eLOC_2;
        
        if(is_LOC(stMasterState))                           //If the master device is still in LOC
            return MailboxState_T::eLOC_2;                      //Shift to LOC_2
        else if(stMasterState == MailboxState_T::eNormal)   //If the master device is back to normal operations
            return MailboxState_T::eNormal;                     //Shift to normal state as well

        //Otherwise remain in LOC_1
        return MailboxState_T::eLOC_1

        break;

    case MailboxState_T::eLOC_2:

        //If loss of coms is induced, shift to LOC_3
        if(bLOC_Induced)
            return MailboxState_T::eLOC_3;

        if(is_LOC(stMasterState))                           //If the master device is still in LOC
            return MailboxState_T::eLOC_3;                      //Shift to LOC_3
        else if(stMasterState == MailboxState_T::eNormal)   //If the master device is back to normal operations
            return MailboxState_T::eNormal;                     //Shift to normal state as well

        //Otherwise remain in LOC_2
        return MailboxState_T::eLOC_2

        break;

    case MailboxState_T::eLOC_3:

        //If loss of coms is induced, shift to LOC Recovery
        if(bLOC_Induced)
            return MailboxState_T::eRecovery_LOC;

        if(is_LOC(stMasterState))                           //If the master device is still in LOC
            return MailboxState_T::eRecovery_LOC;               //Shift to Recovery_LOC
        else if(stMasterState == MailboxState_T::eNormal)   //If the master device is back to normal operations
            return MailboxState_T::eNormal;                     //Shift to normal state as well

        //Otherwise remain in LOC_3
        return MailboxState_T::eLOC_3

        break;

    case MailboxState_T::eRecovery_LOC:

        //Remain in Recovery_LOC until the main process in loop() sets the recovery flag
        return MailboxState_T::eRecovery_LOC;

        break;

    case MailboxState_T::eRecovery:

        if(stMasterState == MailboxState_T::eNormal)    //If the master device is back to normal operations
            return MailboxState_T::eNormal;                 //Shift to normal state as well
        else                                            //Else
            return MailboxState_T::eRecovery;               //Remain in recovery

        break;
    
    default:

        //Should not be here
        //Set state to eRecovery
        return MailboxState_T::eRecovery;
    }
}

/*-------------------------------------------------------------------------------------------------
    Process_RX

    Subprocess which processes the command message that was most recently received. Will not execute 
    unless one condition is met:
        
        1.  bRX_Buf_ready is set, which means cRX_Buf is populated with a full message ready for
            processing

    INPUT   --  NONE

    OUTPUT (void)
*/
void Mailbox::Process_RX()
{
    //Check to see if the RX buffer is ready for processing and there is no untouched message
    if(!RX_Buf_Ready())
        return;
    
    //Begin parsing the buffered message based on the master's mailbox state, as that will
    //determine the type of message that was received
    switch(stMasterState)
    {
    case MailboxState_T::eNormal:

        //Create message structure
        RX_Message_Structure_Normal_T * stRX_msg = (RX_Message_Structure_Normal_T *)cRX_Buf;

        //Load into RX Message
        mRX = RX_Message(stRX_msg);

        if(mRX.SequenceNum() != mTX.SequenceNum())
            induce_LOC();

        break;

    case MailboxState_T::eLOC_1:

        //Continue down  

    case MailboxState_T::eLOC2:

        //Contnue down

    case MailboxState_T::eLOC3:

        //Induce LOC and continue down
        induce_LOC();

    case MailboxState_T::eRecovery_LOC:

    case MailboxState_T::eRecovery:

        //Create message structure
        RX_Message_Structure_Recovery_T * stRXmsg = (RX_Message_Structure_Recovery_T *)cRX_Buf;

        //Load into RX message
        mRX = RX_Message(stRX_msg);

        break;

    default:
        
        //Do nothing but return before clearing the RX_Buf_Ready flag
        return;
    }


    Set_RX_Buf_Ready(false);    //Clear bRX_Buf_Ready as the buffer's message has now been processed
    Set_RX_Ready();             //Set bRX_Ready to signal loop() that a new message has been 
                                //  processed
}

/*-------------------------------------------------------------------------------------------------
    Process_TX

    Subprocess which processes the TX_Message received from loop(). Will not execute unless
    one condition is met:
        
        1.  bTX_Ready is set, which means that loop() has updated the mailbox with a new TX
            message.

    INPUT   --  NONE

    OUTPUT (void)
*/
void Mailbox::Process_TX()
{
    //Check to see if a TX message is ready to be processed
    if(!TX_Ready())
        return;

    //Populate the message into cTX_Buf according to the current mailbox state
    switch(stMailboxState)
    {
    case MailboxState_T::eNormal:

        //Create message structure
        TX_Message_Structure_Normal_T * stTX_msg;
        mTX.encode_MessageStructure(*stTX_msg);

        //Copy to the TX buffer
        memcpy(cTX_Buf, stTX_msg, _TX_MESSAGE_LENGTH_NORMAL);

        break;

    case MailboxState_T::eLOC_1:

        //Continue down

    case MailboxState_T::eLOC2:

        //Continue down

    case MailboxState_T::eLOC3:

        //Continue down

    case MailboxState_T::eRecovery_LOC:

        //Continue down

    case MailboxState_T::eRecovery:

        //Copy hash compare from RX message
        mTX.Set_HashCompare(mRX.HashCompare());

        //Create message structure
        TX_Message_Structure_Recovery_T * stTXmsg;
        mTX.encode_MessageStructure(*stTX_msg);

        //Copy to the TX buffer
        memcpy(cTX_Buf, stTX_msg, _TX_MESSAGE_LENGTH_RECOVERY);

        break;

    default:
        
        //Do nothing but return before clearing the TX_Buf_Ready flag
        return;
        break;
    }

    Set_TX_Buf_Ready(true); //Set bTX_Buf_Ready to pend the TX subprocess
    Clear_TX_Ready();       //Clear bTX_Ready as the message has now been processed
}

/*-------------------------------------------------------------------------------------------------
    RX_USB

    Primary subprocess for receiving a message from the master device over the Arduino's USB port.
    Will not execute unless one condition is met:

        1.  bRX_Event is set, which means that a serial event has been recorded.

    INPUT   --  NONE

    OUPUT (void)
*/
void Mailbox::RX_USB()
{
    //Check for nonexistant serial event
    if(!Check_RX_Event())
        return;
    
    unsigned long tCurrentCall;
    static unsigned long tPrevCall = 0;
    Letter_T lLetter;

    if(Check_Timeout()) //If a timeout has occured
    {
        //Update the timer
        tCurrentCall = millis();
        Update_TimeoutCounter(tCurrentCall - tPrevCall);
        tPrevCall = tCurrentCall;
        
        int nRX_Length = RX_USB(lLetter); //Record the length of the string that was read in

        if(nRX_Length && lLetter.nMessageLength == nRX_Length) //If the message reports its correct length
        {
            if(checkCRC(lLetter)) //If the CRC returns no errors
            {
                stMasterState = (MailboxState_T)lLetter.cMessageType   //Set the master's mailbox status to the message code
                
                //Set the expected RX message length
                if((stMasterState == MailboxState_T::eNormal) || (stMasterState == MailboxState_T::eStart))
                    nRX_Message_Length = _RX_MESSAGE_LENGTH_NORMAL;
                else
                    nRX_Message_Length = _RX_MESSAGE_LENGTH_RECOVERY;                
                
                memcpy(cRX_Buf, lLetter.cData, nRX_Message_Length);     //Copy the message to the RX buffer
                Set_RX_Buf_Ready(true);                                 //Set bRX_Buf_ready to pend Process_RX()
                
                Reset_TimeoutCounter();
                
                return;
            }        
        }
    }
    
    induce_LOC(); //FLag Loss of Coms
}

/*-------------------------------------------------------------------------------------------------
    TX_USB

    Primary subprocess for transmitting a response message to the master device over the Arduino's
    USB port. Will not execute unless one condition is met:

        1.  bTX_Buf_Ready is set, which means that cTX_Buf has been populated with a processed TX
            message
    
    INPUT   --  NONE

    OUPUT (void)
*/
void Mailbox::TX_USB()
{
    //Check for a processed message within the TX buffer
    if(!TX_Buf_Ready())
        return;

    //Clear bTX_Buf_Ready to show that the message is being sent
    Set_TX_Buf_Ready(false);
    
    //Create and send a letter
    Letter_T lLetter;

    lLetter.cMessageType = cMailboxStatus;
    lLetter.nMessageLength = nTX_Message_Length + sizeof(lLetter) - sizeof(lLetter.cData);
    lLetter.cData = cTX_Buf;
    lLetter.nCRC = computeCRC(lLetter) & 0x7FFF;

    TX_USB(lLetter);
}

/*-------------------------------------------------------------------------------------------------
    TX_USB

    Secondary subprocess for transmitting a response message to the master device over the Arduino's
    USB port.

    INPUT (Letter_T)  --  lLetter
        A populated letter structure that needs to be transmitted. Passed by reference.

    OUPUT (void)
*/
void Mailbox::TX_USB(Letter_T & lLetter)
{
    Serial.write(lLetter.cMessageType);
    Serial.write(lLetter.nMessageLength);
    Serial.write(lLetter.cData);
    Serial.write(lLetter.nCRC);
    Serial.write(lLetter.nStopByte);
}