#include "Function.h"

int combine(int a, int b) {
   int times = 1;
   while (times <= b)
      times *= 10;
   return a*times + b;
}

int combineData(uint8_t data[])
{
    int first = combine(data[0],data[1]);
    int second = combine(data[2], data[3]);
    int final = combine(first, second);
    return final;
}

bool calCount(uint8_t data[])
{
    int a = 0;
    a = data[3];
    if (a < 18)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void splitCalScore(uint8_t data[], uint8_t * AccelScore, uint8_t *MagScore)
{
    int a = 0;
    for(int i = 0; i < 4; i++)
    {
        MagScore[i] = data[a];
        a++;
    }
    a = 8;
    for(int i = 0; i < 4; i++)
    {
        AccelScore[i] = data[a];
        a++;
    }
}

float BitToDec(int data[], int length)
{
    float sum = 0;
    for(int i = 0; i < length; i++)
    {
        sum += (float)(pow(2, i)*data[i]);
    }

    return sum;
}

float shrinkMantissa(float mantissa)
{
    float number = 0.0;
    while(mantissa > 1)
    {
        mantissa = (mantissa/10);
    }
    number = 1 + mantissa;
    return number;
}

float createFloat(uint8_t data[])
{
    std::bitset<8> byte1(data[0]); //putting the data recieved into 8 bit arrays
    std::bitset<8> byte2(data[1]);
    std::bitset<8> byte3(data[2]);
    std::bitset<8> byte4(data[3]);

    int s = 0; //byte4[7];

    int exponentBits[8] = {byte3[7], byte4[0], byte4[1], byte4[2], byte4[3], byte4[4], byte4[5], byte4[6]};
    float exponent = BitToDec(exponentBits, 8); //used to convert from bits to a decimal value
    std::cout << exponent << std::endl;

    int mantissaBits[23] = {byte1[0],byte1[1],byte1[2],byte1[3],byte1[4],byte1[5],byte1[6],byte1[7],byte2[0],byte2[1],byte2[2],byte2[3],byte2[4],byte2[5],byte2[6],byte2[7],byte3[0],byte3[1],byte3[2],byte3[3],byte3[4],byte3[5],byte3[6]};
    float mantissa = BitToDec(mantissaBits, 23);
    std::cout << mantissa << std::endl;
    float OnePointMantissa = shrinkMantissa(mantissa); //used to get mantissa down to a value less than 1
    std::cout << OnePointMantissa << std::endl;

    float actualFloatValue = ((2*(exponent - 127))*(OnePointMantissa));

    if(s)
    {
        actualFloatValue = (actualFloatValue*(-1));
    }
    //std::cout << byte1[1] << std::endl;


    std::cout << actualFloatValue << std::endl;

    return actualFloatValue;
    //printf("%d\n", s);
    //float number = ((-1)(data[0]))
}



void splitGetData(uint8_t data[], uint8_t *heading, uint8_t *pitch, uint8_t *roll)
{
    int a = 1;
    //heading[] = {0,0,0,0}
    //uint8_t pitch[4] = {0,0,0,0};
    //uint8_t roll[4] = {0,0,0,0};
    while(a < 15)
    {
        switch (data[a]) {
        case 5:

            for(int i = 0; i < 4; i++)
            {
                a++;
                heading[i] = data[a];
            }
            a++;
            break;
        case 24:

            for(int i = 0; i < 4; i++)
            {
                a++;
                pitch[i] = data[a];

            }
            a++;
            break;

        case 25:

            for(int i = 0; i < 4; i++)
            {
                a++;
                roll[i] = data[a];

            }
            a++;
            break;
        default:
            break;
        }
    }
}
