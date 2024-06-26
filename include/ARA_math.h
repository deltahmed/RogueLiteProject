/**
 * @file ARA_math.h
 * @author Ahmed A., Rayane M., Abdelwaheb A.
 * @brief Maths handling header
 * @version 1.0
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OWN_MATH
#define OWN_MATH

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "ARA_error.h"

int mod(int a, int b);
int max(int a, int b);
int min(int a, int b);
int randint(int a, int b);
int is_in(int x, int a, int b);
int stick_in_range(int x,int a,int b);

#endif



