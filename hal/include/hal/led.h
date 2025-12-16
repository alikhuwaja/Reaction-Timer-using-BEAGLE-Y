#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include<stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void LED_init(void); //ensures both of the LEDs are off
void LED_cleanup(void); //tuens ofd both LEDs ad the end of the program and cleans up resources
void LED_setGreen(bool on); //controls the Green LED on or off
void LED_setRed(bool on); //controls the Red LED on or off
void LED_offAll(void);// turns off both LEDs
void LED_flashGreen(int times, int total_ms); //controls the green LED flash times
void LED_flashRed(int times, int total_ms); //controls the red LED flash times


