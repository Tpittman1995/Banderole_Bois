#include "trax.h"
#include "Serial.h"


int main(void) {
    Command test = kGetData;
    Command getModInfoResp = kGetDataResp;
    uint8_t payload[4096];

    std::cout << "Hello" << std::endl;
    write_command(test, NULL, 0);
    int w = 1;
        std::cout << "I am trying to read" << std::endl;
        read_command(getModInfoResp, payload, 0x1000);

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

