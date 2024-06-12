#include "main.h"
#include "ds3231.h"

RTC_TIME rtcTime;

uint8_t decToBcd(int val) {
    return (uint8_t)((val / 10 * 16) + (val % 10));
}

int bcdToDec(uint8_t val) {
    return (int)((val / 16 * 10) + (val % 16));
}

void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year) {
    uint8_t set_time[7];
    set_time[0] = decToBcd(sec);
    set_time[1] = decToBcd(min);
    set_time[2] = decToBcd(hour);
    set_time[3] = decToBcd(dow);
    set_time[4] = decToBcd(dom);
    set_time[5] = decToBcd(month);
    set_time[6] = decToBcd(year);

    HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

void Get_Time(void) {
    uint8_t get_time[7];
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
    rtcTime.seconds = bcdToDec(get_time[0]);
    rtcTime.minutes = bcdToDec(get_time[1]);
    rtcTime.hour = bcdToDec(get_time[2]);
    rtcTime.dayofweek = bcdToDec(get_time[3]);
    rtcTime.dayofmonth = bcdToDec(get_time[4]);
    rtcTime.month = bcdToDec(get_time[5]);
    rtcTime.year = bcdToDec(get_time[6]);
}

float Get_Temp(void) {
    uint8_t temp[2];
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x11, 1, temp, 2, 1000);
    return ((temp[0]) + (temp[1] >> 6) / 4.0);
}

void force_temp_conv(void) {
    uint8_t status = 0;
    uint8_t control = 0;
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0F, 1, &status, 1, 100); // read status register
    if (!(status & 0x04)) {
        HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &control, 1, 100); // read control register
        HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, (uint8_t *)(control | (0x20)), 1, 100);
    }
}
