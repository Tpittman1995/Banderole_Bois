#include "wrapper.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    Wrapper w;
    TraxMailbox tm;


    w.printMessage("Doing initCal");
    w.delayBy(5);
    tm.initCal();
    w.printMessage("**************************************************************");


    w.printMessage("Doing startCal");
    w.delayBy(5);
    tm.startCal();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 1");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 2");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 3");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 4");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 5");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 6");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 7");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 8");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 9");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 10");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 11");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 12");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 13");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 14");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 15");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 16");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 17");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");


    w.printMessage("Taking Point 18");
    w.delayBy(5);
    tm.takePoint();
    w.printMessage("**************************************************************");

}
