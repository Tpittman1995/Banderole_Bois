#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "trax.h"
#include <math.h>

int combine(int a, int b);

int combineData(uint8_t data[]);

bool calCount(uint8_t data[]);

void splitCalScore(uint8_t data[], uint8_t * AccelScore, uint8_t *MagScore);

void splitGetData(uint8_t data[], uint8_t *heading, uint8_t *pitch, uint8_t *roll);

#endif // FUNCTIONS_H
