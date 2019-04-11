#include "trax.h"
#include "serial.h"
#include "traxmailbox.h"


int main(void) {

    // ******** Test procedure for TraxMailbox ************ //
    int success = 8;
    TraxMailbox trax;

//    std::cout << "Set default" << std::endl;
    success = 9;
    success = trax.initCal();
    std::cout << "init success = " << success << std::endl;

    trax.startCal();
    std::cout << "start success" << std::endl;


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

    //trax.abortCal();

    // ******************* Test of manual calibratino ***************** //
    //manualCal();

    return 0;
}
