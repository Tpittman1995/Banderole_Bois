#include <stdio.h>
#include "trax.h"
#include "serial.h"

serial::Serial serPort("/dev/ttyUSB0", 38400, serial::Timeout::simpleTimeout(80)); // Open serial port connected to TRAX - /ttyACM0

///**
//  * Puts the TRAX into compas mode, sets filter taps to 0, set to no autosampling,
//  * set mag and accell coeff set to 0, set number of cal points to 18 then save
//  * setting.
//  *
// */
//int initCal() {
//    Command SetFunctMode = kSetFunctionalMode;     // fame ID for setting TRAX to compas mode
//    Command setFIR = ksetFIRFilters;               // frame ID for setting FIR filters to 0
//    Command setConfig = kSetConfig;                 // frame ID to set configurations
//    Command save = kSave;                           // frame ID to save settings to non volitile mem
//    Command saveDone = kSaveDone;                   // frame ID to confirm save success
//    Command setDataComp = kSetDataComponents;       // frame ID to set which data we want displayed

//    success = 0;                                    // success variale for error handling

//    Command readResp;                       // read populates with frame ID of message just read
//    uint8_t payload_read[4096];             // read function populates with response payload
//    uint8_t functModePayload[1] = {0x0};    // payload of setFunctionalMode
//    uint8_t firPayload[7] = {0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0};     // payload of setFIRFilters
//    uint8_t autoPayload[2] = {0xD, 0x0};    // payload for turning off autosampling
//    uint8_t magCoeffPayload[5] = {0x12, 0x0, 0x0, 0x0, 0x0};    // saving to first coefff set
//    uint8_t accelCoeffPayload[5] ={0x13, 0x0, 0x0, 0x0, 0x0};   // saving to first coeff set
//    uint8_t calPointPayload[5] = {0xC, 0x0, 0x0, 0x0, 0x12};    // set cal points to 18

//    // Set TRAX into compas mode
//    write_command(SetFunctMode, functModePayload, 1);
//    // Set FIR filters to 0
//    write_command(setFIR, firPayload, 7);
//    // Set autosampling to false
//    write_command(setConfig, autoPayload, 2);
//    // Set mag coeff set to 0
//    write_command(setConfig, magCoeffPayload, 5);
//    // Set accel coeff set to 0
//    write_command(setConfig, accelCoeffPayload, 5);
//    // Set number of calibration points to 18
//    write_command(setConfig, calPointPayload, 5);
//}

///**
//  * Prompts TRAX to begin calibratoin procedure by sending the start cal command
// */
//void startCal() {
//    Command beginCal = kStartCal;   // command to start cal
//    // payload to start cal in mag and accel mode
//    uint8_t startCalPayload[4] = {0x0, 0x0, 0x0, 0x6E};

//    // write start cal
//    write_command(beginCal, startCalPayload, 4);
//}

///**
//  * Commands TRAX to stop calibration
// */
//void abortCal() {
//    Command stopCal = kStopCal; // command to abort cal

//    // send command to abort cal
//    write_command(stopCal, NULL, 0);
//}

///**
//  * Commands TRAX to take a calibration point and check to ensure
//  * that point was successfully taken
// */
//int takePoint() {
//    Command takeCalPoint = kTakeUserCalSample;  // command to take point
//    Command readResp;                       // read populates with frame ID of message just read
//    uint8_t payloadRead[4096];             // read function populates with response payload

//    // write command to take calibration point
//    write_command(takeCalPoint, NULL, 0);
//    // Read
//    //read_command(readResp, payloadRead, 0x1000, 0x12);
//}


/**
* Writes a command to the PNI TRAX using PNI Binary protocol.
*
* @param cmd The command to write to the sensor.
* @param payload The payload of the command to write.
* @param payload_len The length of data in bytes pointed to by the payload.
*
* @param Return zero upon success and -1 upon error.
*/
int write_command(const Command cmd, const uint8_t *payload, const uint16_t payload_len)
{
    //printf("%x\n", payload[1]);
    /*
    * Construct the PNI Binary packet protocol.
    */
    int ret = 0;
    uint16_t packet_len = payload_len + 5;
    uint8_t *packet = new uint8_t[packet_len];
    uint8_t *packet1 = &packet[1];
    uint8_t *packet2 = &packet[2];
    uint8_t *packet3 = &packet[3];
    uint8_t *packet4 = &packet[4];
    /*
    * Write the byte length into the first two bytes of the packet, and then
    * write the command FRAME_ID byte.
    */
    const uint16_t packet_len_ne = htons(packet_len);
    memcpy(packet, &packet_len_ne, 2);
    packet[2] = static_cast<uint8_t>(cmd);

    /*
    * Follow the packet data with the payload for the command.
    */
    memcpy(&packet[3], payload, payload_len);

    /*
    * Calculate the CRC-16 for the packet and append it to the end.
    */
    const uint16_t crc = htons(crc16(packet, packet_len - 2));
    memcpy(&packet[packet_len - 2], &crc, 2);

    /*
    * Write the data down the serial port.
    */

    // Trying to put data into array? - BS
    //uint8_t write_data[] = {*packet1, *packet2, *packet3, *packet4};
    // is this only sending first packet - packet[0] = 00? - BS

    serPort.write(packet, packet_len);      // Could add error checking?

    uint8_t packetContents[20];
    for (int i = 0; i < 20; i++)
    {
        packetContents[i] = packet[i];
        printf("%x\n", packetContents[i]);
    }
    delete[] packet;

    return ret;
}

/**
* Reads a command from the sensor with a PNI Binary format.
*
* @param[out] resp The location to store the resulting response.
* @param[out] payload The location to store the payload of the command.
* @param max_payload_length The maximum data that can be written to payload.
*
* @return Zero upon success and -1 upon error.
*/
int read_command(Command &resp, uint8_t *payload, const uint16_t max_payload_length, const size_t responseSize)
{
    /*
    * Read the packet header (packet length stored as uint16_t)
    */
    uint8_t data[4096];
//    if (serial_port.Read(data, 2) != 2)
//    {
//        return -1;
//    }
    size_t test = serPort.read(data, responseSize);

    uint16_t packet_len;
    memcpy(&packet_len, &data, 2);
    packet_len = ntohs(packet_len);

    /*
    * Packets are limited to 4096 bytes in length.
    */
    if (packet_len > 4096)
    {
        return -1;
    }

    /*
    * Read the remainder of the packet.
    */
//    if (serial_port.Read(&data[2], packet_len - 2) != packet_len - 2)
//    {
//        return -1;
//    }

    /*
    * Calculate and check the crc16 on the packet.
    */
    uint16_t crc;
    memcpy(&crc, &(data[packet_len - 2]), 2);
    crc = ntohs(crc);

    if (crc != crc16(data, packet_len - 2))
    {
        return -1;
    }

    /*
    * Ensure that the data will fit within the provided buffer.
    */
    if (packet_len - 5 > max_payload_length)
    {
        return -1;
    }

    /*
    * Copy the fields into the user-provided buffers.
    */
    memcpy(&resp, &data[2], 1);
    memcpy(payload, &data[3], packet_len - 5);

    // print what was just read / check if everything got put into the propper variables
    printf("%x \n", resp);
    for(int i = 0; i < packet_len - 5; i++) {
        printf("%x \n", payload[i]);
    }
    printf("%x \n", crc);

    return 0;
}

/**
* CRC16 CCITT Calculator.
*
* @note Code was taken from:
* https://github.com/gtrafimenkov/pycrc16/blob/master/src/_crc16module.c
*
* @param data A pointer to the buffer to be summed.
* @param length The length of data in bytes pointed to by data.
*
* @return The final checksum value.
*/
static const uint16_t crc_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

uint16_t crc16(uint8_t *data, int length)
{
    uint16_t crc = 0x0000;
    for (int i = 0; i < length; ++i)
    {
        crc = ((crc << 8) & 0xFF00) ^ (crc_table[((crc >> 8) & 0xFF)
            ^ data[i]]);
    }

    return crc & 0xFFFF;
}

/**
* Copies a float from network endianness to host endianness.
*
* @param data A float stored in network endianness.
*
* @return A float stored in host endianness.
*/
float ntohf(float data)
{
    float result;

    /*
    * Check to see if host system is big-endian. If so, memcpy is functional.
    */
    if (htonl(47) == 47)
    {
        memcpy(&result, &data, 4);
    }
    else
    {
        char *result_ptr = reinterpret_cast<char *>(&result);
        char *data_ptr = reinterpret_cast<char *>(&data);
        result_ptr[0] = data_ptr[3];
        result_ptr[1] = data_ptr[2];
        result_ptr[2] = data_ptr[1];
        result_ptr[3] = data_ptr[0];
    }

    return result;
}
