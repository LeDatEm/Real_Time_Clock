#ifndef DS3231_H_
#define DS3231_H_

#include "main.h"

#define DS3231_ADDRESS 0xD0

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;
    uint8_t dayofweek;
    uint8_t dayofmonth;
    uint8_t month;
    uint8_t year;
} RTC_TIME;
extern RTC_TIME rtcTime;
extern I2C_HandleTypeDef hi2c1;

uint8_t decToBcd(int val);
int bcdToDec(uint8_t val);

void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);
void Get_Time(void);
float Get_Temp(void);
void force_temp_conv(void);

#endif /* DS3231_H_ */
