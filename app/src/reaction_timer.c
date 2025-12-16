#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
//#include "/home/ali/ensc351/work/as1/cmake_starter/hal/include/hal/led.h"
//#include "/home/ali/ensc351/work/as1/cmake_starter/hal/include/hal/joystick.h"
#include "hal/led.h"
#include "hal/joystick.h"


static long long getTimeInMs(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return (long long)spec.tv_sec * 1000 + spec.tv_nsec / 1000000;
}

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000000LL;
    struct timespec reqDelay = { delayInMs / 1000, (delayInMs % 1000) * NS_PER_MS };
    nanosleep(&reqDelay, NULL);
}

static void waitUntilJoystickReleasedUD(void)
{
    while (true) {
        JoystickDirection d = Joystick_getDirection();
        if (!(d == UP || d == DOWN))
            break;
        usleep(1000);
    }
}

int main(void)
{
    srand((unsigned int)time(NULL));

    LED_init();
    Joystick_init();
    LED_offAll();

    printf("Hello embedded world, from Ali!\n");
    printf("When the LEDs light up, press the joystick in that direction!\n");
    printf("(Press left or right to exit)\n");

    long long bestTimeMs = -1;
    bool quit = false;

    while (!quit) {
        printf("Get ready...\n");
        for (int i = 0; i < 4; i++) {
            LED_setGreen(true);
            sleepForMs(250);
            LED_setGreen(false);
            LED_setRed(true);
            sleepForMs(250);
            LED_setRed(false);
        }

        JoystickDirection d = Joystick_getDirection();
        if (d == UP || d == DOWN) {
            printf("Please let go of joystick.\n");
            waitUntilJoystickReleasedUD();
        }

        int delayMs = 500 + (rand() % 2501);
        sleepForMs(delayMs);

        d = Joystick_getDirection();
        if (d == UP || d == DOWN) {
            printf("Too soon!\n");
            LED_flashRed(2, 300);
            LED_offAll();
            continue;
        }

        bool chooseUp = (rand() % 2) == 0;
        if (chooseUp) {
            printf("Press UP now!\n");
            LED_setGreen(true);
        } else {
            printf("Press DOWN now!\n");
            LED_setRed(true);
        }

        long long startMs = getTimeInMs();
        long long nowMs = startMs;
        JoystickDirection pressed = NONE;

        while (true) {
            pressed = Joystick_getDirection();
            nowMs = getTimeInMs();

            if (pressed != NONE)
                break;

            if (nowMs - startMs > 5000) {
                printf("No input within 5000ms; quitting!\n");
                quit = true;
                break;
            }
            usleep(1000);
        }

        LED_offAll();
        if (quit) break;

        long long reactionMs = nowMs - startMs;

        if (pressed == LEFT || pressed == RIGHT) {
            printf("User selected to quit.\n");
            quit = true;
            break;
        }

        bool correct = (chooseUp && pressed == UP) || (!chooseUp && pressed == DOWN);

        if (correct) {
            printf("Correct!\n");
            if (bestTimeMs < 0 || reactionMs < bestTimeMs) {
                bestTimeMs = reactionMs;
                printf("New best time!\n");
            }
            printf("Your reaction time was %lldms; best so far is %lldms.\n",
                   reactionMs, bestTimeMs);
            LED_flashGreen(5, 1000);
        } else {
            printf("Incorrect.\n");
            LED_flashRed(5, 1000);
        }
    }

    LED_offAll();
    LED_cleanup();
    Joystick_cleanup();
    return 0;
}
