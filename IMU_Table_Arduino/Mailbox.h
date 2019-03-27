#ifndef MAILBOX_H
#define MAILBOX_H

#include "Message_Structure.h"

#define HI_16(x)    ((x) & 0xFF00)
#define LO_16(x)    ((x) & 0x00FF)
#define HI_32(x)    ((x) & 0xFFFF0000)
#define LO_32(x)    ((x) & 0x0000FFFF)

#define MERGE_16(hi, lo)    ((((hi) & 0x00FF) << 8) + ((lo) & 0x00FF))
#define MERGE_32(hi, lo)    ((((hi) & 0x0000FFFF) << 16) + ((lo) & 0x0000FFFF))

/* The timeout threshold for communications in milliseconds ///////////////////////////////////////

    If the amount of time specified above (in ms) is reached without a proper message being
    received, the mailbox state will divert into Loss of Communications.
*/
#define _LOC_TIMEOUT_MS 100

class MailBox
{
public:

    //Public Structs
    typedef enum class MailboxState_T
    {
        eNormal         =   0x10,

        //Loss of Com states
        eLOC_1          =   0x11,
        eLOC_2          =   0x12,
        eLOC_3          =   0x13,

        //Recovery states
        eRecovery       =   0x18,
        eRecovery_LOC   =   0x19,

        //Startup phase
        eStart          =   0xAA
    };    

    //Public Functions
    MailBox();
    ~MailBox();

    bool RX_Ready() const   { return bRX_Ready; }
    bool TX_Ready() const   { return bTX_Ready; }

    MailboxState_T MailboxState() const { return stMailboxState; }

    RX_Message & RX()
    {
        //Clear RX_Ready to show that the message has been seen
        bRX_Ready = false;
        return mRX; 
    }

    TX_Message & TX() { return mTX; }

    void Reset_TimeoutCounter()     { nTimeoutCounter = 0; }
    void Set_Recovery()             { stMailboxState = MailboxState_T::eRecovery; }
    void Set_RX(RX_Message & oRX)   { mRX = oRX; }
    void Set_RX_Event()             { bRX_Event = true; }

    void Set_TX(TX_Message & oTX)    
    {
        //Set TX_Ready to true to show that a new message has been set
        bTX_Ready = true;
        mTX = oTX;
    }

    static void runFrame_USB(MailBox & mMailbox);

private:

    typedef enum class x
    {
        xgood = 1,
        xbad = 0


    };

    typedef enum y
    {
        ygood = 0;
        ybad = 1;
    }

    x xenum = xgood;
    y yenum = x::xbad;

    //Private Structs
    typedef struct Letter_T
    {
        //Message header
        uint8_t cMessageType;
        uint8_t nMessageLength;

        //Data
        char * cData;
        uint16_t nCRC : 15;

        //Stop byte
        const uint8_t nStopByte = 0xE7;
    };

    //Private Functions
    bool is_LOC(MailboxState_T stState)
    {
        if(stState == MailboxState_T::eLOC_1 ||
           stState == MailboxState_T::eLOC_2 ||
           stState == MailboxState_T::eLOC_3 ||
           stState == MailboxState_T::eRecovery_LOC)
            return true;
        else
            return false;
    }

    bool RX_Buf_Ready() const { return bRX_Buf_Ready; }

    bool Check_RX_Event()
    {
        if(bRX_Event)
        {
            bRX_Event = false;
            return true;
        }
        else
            return false;
    }

    bool Check_Timeout()
    {
        if(nTimeoutCounter >= _LOC_TIMEOUT_MS)
        {
            Reset_TimeoutCounter();
            return true;
        }
        else
            return false;
        
    }

    bool TX_Buf_Ready() const { return bTX_Buf_Ready;}

    bool checkCRC(Letter_T & lLetter);

    int RX_USB(Letter_T & lLetter);
    uint16_t computeCRC(Letter_T & lLetter);
    uint16_t computeCRC(char * cStr, int nLen);

    MailboxState_T updateStateMachine();

    void Clear_TX_Ready()                               { bTX_Ready = false; }
    void induce_LOC()                                   { bLOC_Induced = true; }
    void Set_RX_Ready()                                 { bRX_Ready = true; }
    void Set_RX_Buf_Ready(const bool bStatus)           { bRX_Buf_Ready = bStatus; }
    void Set_TX_Buf_Ready(const bool bStatus)           { bTX_Buf_Ready = bStatus; }
    void Update_TimeoutCounter(unsigned long nMillis)   { nTimeoutCounter += nMillis; }

    void Process_RX();
    void Process_TX();
    void RX_USB();
    void TX_USB();
    void TX_USB(Letter_T & lLetter);

    //Private Operator Overloads

    //Private Members
    bool bRX_Ready, bTX_Ready, bRX_Buf_Ready, bTX_Buf_Ready;                       //Flags for message timing
    bool bRX_Event, bLOC_Induced;
    char cRX_Buf[_RX_MESSAGE_LENGTH_NORMAL], cTX_Buf[_TX_MESSAGE_LENGTH_NORMAL];   //Char buffers for serial messaging
    unsigned long nTimeoutCounter;
    uint8_t nTX_Message_Length, nRX_Message_Length;                                //Dynamic message size depending on the mailbox status
    uint32_t nMasterSequenceNum;

    MailboxState_T stMailboxState, stMasterState;
    RX_Message mRX;     //RX Message structure
    TX_Message mTX;     //TX Message structure
};

#endif