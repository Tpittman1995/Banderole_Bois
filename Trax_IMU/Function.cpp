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
