#ifndef MAILBOX_H
#define MAILBOX_H

#include "Arduino.h"
#include "Message_Structure.h"

/* The timeout threshold for communications in milliseconds ///////////////////////////////////////

    If the amount of time specified above (in ms) is reached without a proper message being
    received, the mailbox state will divert into Loss of Communications.
*/
#define _LOC_TIMEOUT_MS 20

class Mailbox
{
public:

    typedef enum class Com_Code_T
    {
        eOK     =   0xA0,

        //Acknowledge
        eAck            =   0xAE,
        eNack           =   0xAF,

        //Loss of Com phases
        eLOC_1          =   0xA1,
        eLOC_2          =   0xA2,
        eLOC_3          =   0xA3,

        //Recovery phases
        eRecovery       =   0xA8,
        eRecovery_LOC   =   0xA9,

        //Startup phase
        eStart          =   0xAA
    };

    typedef enum class MailboxState_T
    {
        eNormal         =   0x10,

        //Loss of Com states
        eLOC_1          =   0x11,
        eLOC_2          =   0x12,
        eLOC_3          =   0x13,

        //Recovery states
        eRecovery       =   0x18,
        eRecovery_LOC   =   0x19
    };

    Mailbox();
    ~Mailbox();

    //Public Functions
    bool    RX_Ready() const    { return bRX_Ready; }
    bool    TX_Ready() const    { return bTX_Ready; }

    MailboxState_T  MailboxState() const    { return stMailboxState; }

    RX_Message  RX() const  { return mRX; }

    TX_Message  TX() const  { return mTX; }
    
    void    Set_Recovery()              { stNext_MailboxState = MailboxState_T::eRecovery; }
    void    Set_RX_Event()              { bRX_Event = true; }
    void    Set_RX_Ready()              { bRX_Ready = true; }
    void    Set_RX(RX_Message & oRX)    { mRX = oRX; }
    void    Set_TX(TX_Message & oTX)    { mTX = oTX; }
    void    Set_TX_Ready()              { bTX_Ready = true; }

    bool Process_RX();
    bool Process_TX();
    void runFrame_USB();
    void RX_USB();
    void TX_USB();

private:

    typedef struct Letter_T
    {
        //Message header
        uint8_t cMessageType;
        uint8_t nMessageLength;

        //Data
        char * cData;
        uint16_t nCRC;

        //Stop byte
        const uint8_t nStopByte = 0xE7;
    };

    //Private Functions
    bool Check_RX_Buf_Ready() const { return bRX_Buf_Ready; }

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

    bool Check_RX_Ready()
    {
        if(bRX_Ready)
        {
            bRX_Ready = false;
            return true;
        }
        else
            return false;
    }

    bool Check_TX_Buf_Ready() const { return bTX_Buf_Ready;}

    bool Check_TX_Ready()
    {
        if(bTX_Ready)
        {
            bTX_Ready = false;
            return true;
        }
        else
            return false;
        
    }

    bool checkCRC(char cData[], uint16_t nCRC);
    int RX_USB(Letter_T & lLetter);
    uint16_t computeCRC(char cData[]);

    MailboxState_T updateStateMachine();

    void Set_RX_Buf_Ready(bool bStatus) { bRX_Buf_Ready = bStatus; }
    void Set_TX_Buf_Ready(bool bStatus) { bTX_Buf_Ready = bStatus; }

    void induce_LOC();
    void TX_USB(Letter_T & lLetter);

    //Private Members
    bool bRX_Ready, bTX_Ready, bRX_Buf_Ready, bTX_Buf_Ready;        //Flags for message timing
    bool bRX_Event;
    char cRX_Buf[_RX_MESSAGE_LENGTH], cTX_Buf[_TX_MESSAGE_LENGTH];  //Char buffers for serial messaging
    uint8_t nTX_Message_Length;                                     //Dynamic message size depending on the mailbox status

    Com_Code_T cMailboxStatus, cMasterStatus;
    MailboxState_T stMailboxState, stNext_MailboxState;
    RX_Message mRX;     //RX Message structure
    TX_Message mTX;     //TX Message structure
};

#endif