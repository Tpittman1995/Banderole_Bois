#pragma once
#ifndef SERIAL_H
#define SERIAL_H

#endif // SERIAL_H

// C library headers
#include <stdio.h>
#include <string.h>
#include <string>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

using std::string;

class Serial {
public:
    Serial(const char portName[] = ""); //int baudRate = 9600
    ~Serial();
    //void setBaud(int baudRate);
    void writeData(unsigned char msg[], int length);
    void readData(uint8_t *read_buf);

private:
    string portName;
    //int baudRate;
    struct termios tty;
    int serial_port;
};

