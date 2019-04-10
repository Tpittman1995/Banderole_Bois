#include "trax.h"
#include "serial.h"
#include "traxmailbox.h"


int main(void) {

    // ****** Test procedure for TraxMailbox ************
    int success = 8;
    TraxMailbox trax;

//    std::cout << "Set default" << std::endl;
    success = 9;
    success = trax.initCal();
    std::cout << "init success = " << success << std::endl;

    trax.startCal();
    std::cout << "start success" << std::endl;

//    success = 7;
//    success = trax.setDefaultSettings();
//    std::cout << "init success = " << success << std::endl;

//    success = trax.initCal();
//    std::cout << success << std::endl;

//    //success = -1;
//    trax.startCal();

//    success = 9;
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
    usleep(5000000);
    success = trax.takePoint();
//    std::cout << success << std::endl;

//    success = 10;
//    success = trax.getPosition();
//    std::cout << "Get position = " << success << std::endl;

    //trax.abortCal();


//    success = trax.getPosition();
//    std::cout << success << std::endl;

//    std::cout << trax.getHeading() << std::endl;
//    std::cout << trax.getPitch() << std::endl;
//    std::cout << trax.getRoll() << std::endl;

    return 0;
}
