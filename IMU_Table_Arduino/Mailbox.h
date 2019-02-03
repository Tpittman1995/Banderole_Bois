#ifndef MAILBOX_H
#define MAILBOX_H

#include "Message_Structure.h"

class Mailbox
{
public:

    Mailbox();
    ~Mailbox();

    //Public Functions
    bool        RX_Ready() const    { return bRX_Ready; }
    bool        TX_Ready() const    { return bTX_Ready; }

    void Process_RX();
    void Process_TX(const TX_Message & oMessage);

private:

    //Private Functions
    bool check_RX_Ready()
    {
        if(bRX_Ready)
        {
            bRX_Ready = false;
            return true;
        }
        else
            return false;
    }

    bool check_TX_Ready()
    {
        if(bTX_Ready)
        {
            bTX_Ready = false;
            return true;
        }
        else
            return false;
        
    }

    //Private Members
    bool bRX_Ready, bTX_Ready;
    RX_Message oRX;
    TX_Message oTX;
};

#endif