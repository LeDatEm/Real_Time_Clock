#include "main.h"


void mode(float TEMP);

typedef enum {
	MODE_MIN_SEC,
	MODE_HOUR_MIN,
    MODE_DAY_MON,
	MODE_YEAR,
	MODE_TEMP,
	MODE_ALARM
} ClockMode;
