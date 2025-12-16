#ifndef JOYSTICK_H
#define JOYSTICK_H 
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include<stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>



typedef enum {
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER
} JoystickDirection;

void Joystick_init(void);
void Joystick_cleanup(void);
JoystickDirection Joystick_getDirection(void);
bool Joystick_isPressed(void);


#endif
