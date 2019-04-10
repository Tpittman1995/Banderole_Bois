#include "LocalMailbox.h"
#include <iostream>
#include <arpa/inet.h>


//Declare Serial Port
serial::Serial serPort("/dev/ttyACM0",38400,serial::Timeout::simpleTimeout(25));

//The Global Local Mailbox
LocalMailbox g_Mailbox;

//----------------------------------------------------------
//                     PUBLIC METHODS                      -
//----------------------------------------------------------
//Constructor Method
LocalMailbox::LocalMailbox(){} 

void LocalMailbox::SendCereal(){
    //1.)Going To Have To Take Data From Somewhere Else
    //2.)Make It Into A TX Message And Write That Out

    uint8_t *packers = new uint8_t[10];
    const uint16_t greenbay = htons(10);
    memcpy(packers,&greenbay,2);
    serPort.write(packers,10);
}

void LocalMailbox::ReadCereal(){
    //1.) Read Data Into Buffer
    uint8_t data[4096];
    size_t size = serPort.read(data,0xD);
    std::cout << data << std::endl;
}

//Method For Getting Position X From RX Message Structure
float LocalMailbox::getPositionX(){
    try{
        RX_Message rxTemp = g_Mailbox.Get_RX();
        float xTemp = rxTemp.Position_x_cmd();
        return xTemp;
    }
    catch(...){
        std::cout << "Couldn't Get Position X From RX" << std::endl;
        return -1.0;
    }
}

//Method For Getting Position Y From RX Message Structure
float LocalMailbox::getPositionY(){
    try{
        RX_Message rxTemp = g_Mailbox.Get_RX();
        float yTemp = rxTemp.Position_y_cmd();
        return yTemp;
    }
    catch(...){
        std::cout << "Couldn't Get Position Y From RX" << std::endl;
        return -1.0;
    }
}

//Method For Getting Position Z From RX Message Structure
float LocalMailbox::getPositionZ(){
    try{
        RX_Message rxTemp = g_Mailbox.Get_RX();
        float zTemp = rxTemp.Position_z_cmd();
        return zTemp;
    }
    catch(...){
        std::cout << "Couldn't Get Position Z From RX" << std::endl;
        return -1.0;
    }
}

//----------------------------------------------------------
//                     PRIVATE METHODS                     -
//----------------------------------------------------------

//Method For READING RX USB
void LocalMailbox::RX_Specific(Letter_T & lLetter){
    //Add nBytesReceived
    //[TD] Update Regular Length For RX_Message
    //Create Buffer
    uint8_t cData[_RX_MESSAGE_LENGTH_NORMAL];

    //Read In Actual Data, Copy Into cData Get rData Size
    size_t rData = serPort.read(cData,_RX_MESSAGE_LENGTH_NORMAL);

    //Copy cData Contents Into Letter cData and cData[1] into nMessageLength
    lLetter.cMessageType = cData[0];
    lLetter.nMessageLength = cData[1];

    //Calculate the length of the RX message within the received letter, then copy it
    int nMessageLength = lLetter.nMessageLength - sizeof(lLetter);
    strncpy(lLetter.cData, (char*)&cData[2], nMessageLength);

    //Load Into Buffer So You Can Get The CRC
    lLetter.nCRC = (MERGE_16(cData[rData-3], cData[rData-2]) & 0x7FFF);

    nBytesReceived = rData;
}

//Method For Writing TX USB SENT
void LocalMailbox::TX_Specific(Letter_T & lLetter){
    //Write Out Using Serial

    serPort.write((uint8_t *)&lLetter.cMessageType,1);
    serPort.write((uint8_t *)&lLetter.nMessageLength,1);
    serPort.write(lLetter.cData);

    uint8_t nCRC_Hi = HI_16(lLetter.nCRC);
    uint8_t nCRC_Lo = LO_16(lLetter.nCRC);

    serPort.write(&nCRC_Hi, 1);
    serPort.write(&nCRC_Lo, 1);
    serPort.write((uint8_t *)&lLetter.nStopByte,1);

    nBytesSent = sizeof(lLetter) + strlen(lLetter.cData) - sizeof(lLetter.cData);
}

