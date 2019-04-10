#ifndef LOCALMAILBOX_H
#define LOCALMAILBOX_H

#include <string.h>
#include "Mailbox.h"
#include "serial.h"

//#include "src/Cereal/serial.h"

class LocalMailbox : MailBox {
public:
	LocalMailbox();
    void SendCereal(); //Testing
    void ReadCereal();
    float getPositionX();
    float getPositionY();
    float getPositionZ();

private:
	//These Are The Ones To Be Overloaded
	void RX_Specific(Letter_T & lLetter);   //Overload of pure virtual RX function within Mailbox
    void TX_Specific(Letter_T & lLetter);   //Overload of pure virtual TX function within Mailbox
};
#endif
