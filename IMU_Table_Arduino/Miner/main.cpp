#include "wrapper.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    Wrapper w;
    TraxMailbox tm;
    w.delayBy(10);
    tm.initCal();
    w.printMessage("Post initCal");


    w.delayBy(10);
    tm.startCal();
    w.printMessage("Post startCal");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 1");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 2");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 3");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 4");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 5");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 6");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 7");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 8");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 9");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 10");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 11");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 12");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 13");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 14");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 15");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 16");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 17");


    w.delayBy(10);
    tm.takePoint();
    w.printMessage("Take Point 18");

}
