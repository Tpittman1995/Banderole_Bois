#include <Arduino_FreeRTOS.h>
#include "Mailbox.h"

//  Global Definitions  /////////////////////////////////////////////////////////////

MailBox g_Mailbox;
RX_Message g_RX;
TX_Message g_TX;

//  Task Declarations //////////////////////////////////////////////////////////////

void  Task_Mailbox(void * pvParameters);
void  Task_MotorControl(void * pvParameters);

//  Startup ////////////////////////////////////////////////////////////////////////

void setup() 
{
  //Start serial
  Serial.begin(9600);
}

void loop() 
{ 
  //Record new command message if available
  if(g_Mailbox.RX_Ready())
  {
    g_RX = g_Mailbox.RX();

    //TODO
    //Update command data to the system as appropriate
  }

  //TODO
  //Once all sensor data has been updated, compile TX message

  //Update mailbox with new TX message
  g_Mailbox.Set_TX(g_TX);

  //Run Mailbox frame
  MailBox::runFrame_USB(g_Mailbox);
}

void serialEvent()
{
  //Set the mailbox's RX_Event flag
  g_Mailbox.Set_RX_Event();
}
