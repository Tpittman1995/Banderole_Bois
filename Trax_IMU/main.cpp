#include "trax.h"
#include "Serial.h"


int main(void) {
    Command test_write = kPowerDown;
    Command test_read = kPowerDownDone;
    Command kgetModInfo = kGetModInfo;
    Command kgetModInfoResp = kGetModInfoResp;

    uint8_t payload_write[1] = {0xA};   // payload of command to write
    uint8_t payload_read[4096];     // read function populates w/ response payload

    std::cout << "Hello" << std::endl;
//    write_command(test_write, NULL, 0);//sizeof(payload_write)); - write powerdown
    write_command(kgetModInfo, NULL, 0);    // write getMod

    std::cout << "I am trying to read" << std::endl;
    read_command(kgetModInfoResp, payload_read, 0x1000);


//    Serial serPort("/dev/ttyACM0", 9600);
////    while(1) {
////      unsigned char msg[] = {'a', 'b'};
////      serPort.writeData(msg, 2);
////    }
//    while(1) {
//        uint8_t read_msg[4096];
//        serPort.readData(read_msg);
//    }
    return 0;
}

