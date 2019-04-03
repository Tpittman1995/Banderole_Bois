#include "trax.h"
#include "serial.h"


int main(void) {
    Command test_write = kPowerDown;
    Command test_read = kPowerDownDone;
    Command getModInfo = kGetModInfo;

    // Commands to set and get functional mode to compas
    Command SetFunctMode = kSetFunctionalMode;
    Command getFunctMode = kGetFunctionalMode;
    uint8_t functModePayload[1] = {0x0};    // payload of setFunctionalMode

    // Commands to set and get FIR filters
    Command setFIR = ksetFIRFilters;
    Command getFIR = kGetFIRFilters;
    uint8_t firPayloadSet[3] = {0x3, 0x1, 0x0};  // payload of setFIRFilters
    uint8_t firPayloadGet[2] = {0x3, 0x1};       // payload of getFIRFilters

    // Commands to set and get config - autosampling
    Command setConfig = kSetConfig;
    Command getConfig = kGetConfig;
    uint8_t autoPayloadSet[2] = {0xD, 0x0};    // payload for turning off autosampling
    uint8_t autoPayloadGet[1] = {0xD};         // payload for getting auto sampling setting

    // set and get mag and accel coeff set
    uint8_t magCoeffPayloadSet[5] = {0x12, 0x0, 0x0, 0x0, 0x0}; // saving to first coefff set
    uint8_t magCoeffPayloadGet[1] = {0x12}; // saving to first coefff set

    uint8_t accelCoeffPayloadSet[5] ={0x13, 0x0, 0x0, 0x0, 0x0};  // saving to first coeff set
    uint8_t accelCoeffPayloadGet[1] ={0x13};  // saving to first coeff set

    // set and get total cal points
    uint8_t calPointPayloadSet[5] = {0xC, 0x0, 0x0, 0x0, 0x12};  // set cal points to 18
    uint8_t calPointPayloadGet[5] = {0xC, 0x0, 0x0, 0x0, 0x12};  // set cal points to 18

    // Commands to save data to non volitile mem
    Command save = kSave;
    Command saveDone = kSaveDone;
    // Commands to set and get data components
    Command setDataComp = kSetDataComponents;
    Command getData = kGetData;

    // Structures for read to fill with data
    Command readResp;         // read populates with frame ID of message just read
    uint8_t payloadRead[4096];     // read function populates with response payload

    // set and get functional mode
    write_command(SetFunctMode, functModePayload, 1);
    //write_command(getFunctMode, NULL, 0);

    // Set and get FIR mode
    write_command(setFIR, firPayloadSet, 3);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 0, 5);
    write_command(getFIR, firPayloadGet, 2);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 4, 9);

    // set autosampling to false
    write_command(setConfig, autoPayloadSet, 2);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 0, 5);

    write_command(getConfig, autoPayloadGet, 1);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 2, 7);

    // Set and get mag and accel coeff
    write_command(setConfig, magCoeffPayloadSet, 5);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 0, 5);

    write_command(getConfig, magCoeffPayloadGet, 1);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 5, 10);

    // set and get accel coeff
    write_command(setConfig, accelCoeffPayloadSet, 5);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 0, 5);

    write_command(getConfig, accelCoeffPayloadGet, 1);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 5, 10);

    // set and get total cal points
    write_command(setConfig, calPointPayloadSet, 5);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 0, 5);

    write_command(getConfig, calPointPayloadGet, 1);
    std::cout << "I am trying to read" << std::endl;
    read_command(readResp, payloadRead, 5, 10);


    return 0;
}
