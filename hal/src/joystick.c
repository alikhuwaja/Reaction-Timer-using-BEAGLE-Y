//#include "/home/ali/ensc351/work/as1/cmake_starter/hal/include/hal/joystick.h"
#include "hal/joystick.h"
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

#define SPI_DEV "/dev/spidev0.1"
#define SPI_MODE 0
#define SPI_SPEED_HZ 1000000
#define SPI_BITS_PER_W 8
#define MCP_BITS 12
static int CH_X = 0;
static int CH_Y = 1;
#define JOY_READ_DELAY_US 50000

static int s_fd = -1;
static int s_midX = 0, s_midY = 0;
static int s_thr = -1;

static int spi_xfer(uint8_t *tx, uint8_t *rx, size_t n)
{
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = (uint32_t)n,
        .speed_hz = SPI_SPEED_HZ,
        .bits_per_word = SPI_BITS_PER_W,
        .delay_usecs = 0,
        .cs_change = 0
    };
    return ioctl(s_fd, SPI_IOC_MESSAGE(1), &tr);
}

static int mcp_read(int ch)
{
    uint8_t tx[3] = {0}, rx[3] = {0};
#if MCP_BITS == 10
    tx[0] = 0x01;
    tx[1] = 0x80 | ((ch & 0x07) << 4);
    tx[2] = 0x00;
    if (spi_xfer(tx, rx, 3) < 0) return -1;
    int val = ((rx[1] & 0x03) << 8) | rx[2];
#elif MCP_BITS == 12
    tx[0] = 0x06 | ((ch & 0x04) >> 2);
    tx[1] = (ch & 0x03) << 6;
    tx[2] = 0x00;
    if (spi_xfer(tx, rx, 3) < 0) return -1;
    int val = ((rx[1] & 0x0F) << 8) | rx[2];
#endif
    usleep(JOY_READ_DELAY_US);
    return val;
}

void Joystick_init(void)
{
    if (s_fd >= 0) return;
    s_fd = open(SPI_DEV, O_RDWR);
    if (s_fd < 0) { perror("open spidev"); exit(1); }

    uint8_t mode = SPI_MODE;
    uint8_t bpw = SPI_BITS_PER_W;
    uint32_t spd = SPI_SPEED_HZ;

    if (ioctl(s_fd, SPI_IOC_WR_MODE, &mode) < 0) perror("SPI_IOC_WR_MODE");
    if (ioctl(s_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spd) < 0) perror("SPI_IOC_WR_MAX_SPEED_HZ");
    if (ioctl(s_fd, SPI_IOC_WR_BITS_PER_WORD, &bpw) < 0) perror("SPI_IOC_WR_BITS_PER_WORD");

    long sumX = 0, sumY = 0;
    const int N = 32;
    for (int i = 0; i < N; i++) {
        int vx = mcp_read(CH_X);
        int vy = mcp_read(CH_Y);
        if (vx < 0 || vy < 0) break;
        sumX += vx;
        sumY += vy;
        usleep(2000);
    }
    s_midX = (int)(sumX / N);
    s_midY = (int)(sumY / N);
#if MCP_BITS == 10
    const int full = 1023;
#else
    const int full = 4095;
#endif
    s_thr = (int)(0.12 * full);
}

void Joystick_cleanup(void)
{
    if (s_fd >= 0) close(s_fd);
    s_fd = -1;
}

JoystickDirection Joystick_getDirection(void)
{
    if (s_fd < 0) return NONE;
    int x = mcp_read(CH_X);
    int y = mcp_read(CH_Y);
    if (x < 0 || y < 0) { usleep(JOY_READ_DELAY_US); return NONE; }

    int dx = x - s_midX;
    int dy = y - s_midY;
    JoystickDirection dir = NONE;

    if (abs(dy) >= abs(dx)) {
        if (dy > s_thr) dir = UP;
        else if (dy < -s_thr) dir = DOWN;
    } else {
        if (dx > s_thr) dir = RIGHT;
        else if (dx < -s_thr) dir = LEFT;
    }
    usleep(JOY_READ_DELAY_US);
    return dir;
}

bool Joystick_isPressed(void)
{
    return Joystick_getDirection() != NONE;
}
