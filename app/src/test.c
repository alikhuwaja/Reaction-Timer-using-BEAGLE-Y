#include "/home/ali/ensc351/work/as1/cmake_starter/hal/include/hal/led.h"

int main(void)
{
    // Initialize LED control (set triggers to "none" and turn LEDs off)
    LED_init();

    // Flash green LED 5 times over 1 second total
    LED_flashGreen(5, 1000);

    // Short delay between patterns
    sleep(1);

    // Flash red LED 5 times over 1 second total
    LED_flashRed(5, 1000);

    // Ensure both LEDs are off before exit
    LED_offAll();

    // Clean up (turn LEDs off and restore state)
    LED_cleanup();

    return 0;
}
