#define _POSIX_C_SOURCE 200809L
//#include "/home/ali/ensc351/work/as1/cmake_starter/hal/include/hal/led.h"
#include "hal/led.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

#define LED_ACT_TRIGGER   "/sys/class/leds/ACT/trigger"
#define LED_ACT_BRIGHT    "/sys/class/leds/ACT/brightness"
#define LED_PWR_TRIGGER   "/sys/class/leds/PWR/trigger"
#define LED_PWR_BRIGHT    "/sys/class/leds/PWR/brightness"


static void fail(const char* what, const char* path)
{
    fprintf(stderr, "%s (%s): %s\n", what, path, strerror(errno));
    exit(EXIT_FAILURE);
}

static void write_text(const char* path, const char* text)
{
    FILE* f = fopen(path, "w");
    if (!f) fail("fopen failed", path);
    if (fprintf(f, "%s", text) < 0) {
        fclose(f);
        fail("fprintf failed", path);
    }
    if (fclose(f) != 0) fail("fclose failed", path);
}

static void sleep_ms(long long ms)
{
    if (ms <= 0) return;
    struct timespec ts;
    ts.tv_sec  = (time_t)(ms / 1000);
    ts.tv_nsec = (long)((ms % 1000) * 1000000L);
    nanosleep(&ts, NULL);
}



void LED_init(void)
{
    write_text(LED_ACT_TRIGGER, "none");
    write_text(LED_PWR_TRIGGER, "none");
    sleep_ms(50);  
    write_text(LED_ACT_BRIGHT,  "0");
    write_text(LED_PWR_BRIGHT,  "0");
}

void LED_cleanup(void)
{
    write_text(LED_ACT_BRIGHT, "0");
    write_text(LED_PWR_BRIGHT, "0");
}

void LED_setGreen(bool on)
{
    write_text(LED_ACT_BRIGHT, on ? "1" : "0");
}

void LED_setRed(bool on)
{
    write_text(LED_PWR_BRIGHT, on ? "1" : "0");
}

void LED_offAll(void)
{
    LED_setGreen(false);
    LED_setRed(false);
}

void LED_flashGreen(int times, int total_ms)
{
    if (times <= 0 || total_ms <= 0) return;
    int half = total_ms / (times * 2);
    if (half <= 0) half = 1;

    for (int i = 0; i < times; ++i) {
        LED_setGreen(true);  sleep_ms(half);
        LED_setGreen(false); sleep_ms(half);
    }
}

void LED_flashRed(int times, int total_ms)
{
    if (times <= 0 || total_ms <= 0) return;
    int half = total_ms / (times * 2);
    if (half <= 0) half = 1;

    for (int i = 0; i < times; ++i) {
        LED_setRed(true);    sleep_ms(half);
        LED_setRed(false);   sleep_ms(half);
    }
}
