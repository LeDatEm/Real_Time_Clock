#include "realtime_clock.h"
#include "ky040.h"
#include "ds3231.h"
#include "LED74HC595.h"

ClockMode currentMode = MODE_MIN_SEC;

bool buzzer = false;
bool setup_alarm = false;
bool previousButtonState = false;
int modeChangeCount = 0;
int modeTimeCount = 0;
int previousSecond = 0;
int encoderValue;
static int previousEncoderValue = 0;
uint8_t ButtonCount;
uint8_t delay_counter = 0;
uint8_t alarm_hour, alarm_minute;
uint16_t second, minute, hour, dayw, day, month, year = 2024;
uint16_t new_second, new_minute, new_hour, new_day, new_month, new_year;

void blink_led(){
	int currentSecond = second;
	if (currentSecond != previousSecond) {
		previousSecond = currentSecond;
		if (currentSecond % 2 == 0) {
			GPIOA->BSRR = GPIO_PIN_3;
		} else {
			GPIOA->BSRR = GPIO_PIN_3 << 16;
		}
	}
}

void encoder_Inc_Dec(){

	if (encoderValue != previousEncoderValue && ButtonCount == 1 && currentMode == MODE_MIN_SEC) {
		if (encoderValue > previousEncoderValue) {
			modeTimeCount++;
		} else {
			modeTimeCount--;
		}

		previousEncoderValue = encoderValue;

		if (modeTimeCount >= 1) {
			modeTimeCount = 0;
			if (++new_minute > 59) {
				new_minute = 0;
			}
		} else if (modeTimeCount <= -1) {
			modeTimeCount = 0;
			if (--new_minute <= 0) {
				new_minute = 59;
			}
		}
	}
	if (encoderValue != previousEncoderValue && ButtonCount == 2 && currentMode == MODE_MIN_SEC) {
			if (encoderValue > previousEncoderValue) {
				modeTimeCount++;
			} else {
				modeTimeCount--;
			}

			previousEncoderValue = encoderValue;

			if (modeTimeCount >= 1) {
				modeTimeCount = 0;
				if (++new_second > 59) {
					new_second = 0;
				}
			} else if (modeTimeCount <= -1) {
				modeTimeCount = 0;
				if (--new_second <= 0) {
					new_second = 59;
				}
			}
		}
	if (encoderValue != previousEncoderValue && ButtonCount == 1 && currentMode == MODE_HOUR_MIN) {
		if (encoderValue > previousEncoderValue) {
			modeTimeCount++;
		} else {
			modeTimeCount--;
		}

		previousEncoderValue = encoderValue;

		if (modeTimeCount >= 1) {
			modeTimeCount = 0;
			if (++new_hour > 23) {
				new_hour = 0;
			}
		} else if (modeTimeCount <= -1) {
			modeTimeCount = 0;
			if (--new_hour <= 0) {
				new_hour = 23;
			}
		}
	}
	if (encoderValue != previousEncoderValue && ButtonCount == 2 && currentMode == MODE_HOUR_MIN) {
		if (encoderValue > previousEncoderValue) {
			modeTimeCount++;
		} else {
			modeTimeCount--;
		}

		previousEncoderValue = encoderValue;

		if (modeTimeCount >= 1) {
			modeTimeCount = 0;
			if (++new_minute > 59) {
				new_minute = 0;
			}
		} else if (modeTimeCount <= -1) {
			modeTimeCount = 0;
			if (--new_minute <= 0) {
				new_minute = 59;
			}
		}
	}
	if (encoderValue != previousEncoderValue && ButtonCount == 1 && currentMode == MODE_DAY_MON) {
			if (encoderValue > previousEncoderValue) {
				modeTimeCount++;
			} else {
				modeTimeCount--;
			}

			previousEncoderValue = encoderValue;

			if (modeTimeCount >= 1) {
				modeTimeCount = 0;
				if (++new_day > 31) {
					new_day = 0;
				}
			} else if (modeTimeCount <= -1) {
				modeTimeCount = 0;
				if (--new_day <= 0) {
					new_day = 31;
				}
			}
		}
		if (encoderValue != previousEncoderValue && ButtonCount == 2 && currentMode == MODE_DAY_MON) {
			if (encoderValue > previousEncoderValue) {
				modeTimeCount++;
			} else {
				modeTimeCount--;
			}

			previousEncoderValue = encoderValue;

			if (modeTimeCount >= 1) {
				modeTimeCount = 0;
				if (++new_month > 12) {
					new_month = 0;
				}
			} else if (modeTimeCount <= -1) {
				modeTimeCount = 0;
				if (--new_month <= 0) {
					new_month = 12;
				}
			}
		}
		if (encoderValue != previousEncoderValue && ButtonCount == 1 && currentMode == MODE_YEAR) {
			if (encoderValue > previousEncoderValue) {
				modeTimeCount++;
			} else {
				modeTimeCount--;
			}

			previousEncoderValue = encoderValue;

			if (modeTimeCount >= 1) {
				modeTimeCount = 0;
				if (++new_year > 9999) {
					new_year = 0;
				}
			} else if (modeTimeCount <= -1) {
				modeTimeCount = 0;
				if (--new_year <= 2024) {
					new_year = 2024;
				}
			}
		}
		if (encoderValue != previousEncoderValue && ButtonCount == 1 && currentMode == MODE_ALARM) {
			if (encoderValue > previousEncoderValue) {
				modeTimeCount++;
			} else {
				modeTimeCount--;
			}

			previousEncoderValue = encoderValue;

			if (modeTimeCount >= 1) {
				modeTimeCount = 0;
				if (++alarm_hour > 23) {
					alarm_hour = 0;
				}
			} else if (modeTimeCount <= -1) {
				modeTimeCount = 0;
				if (--alarm_hour <= 0) {
					alarm_hour = 23;
				}
			}
		}
		if (encoderValue != previousEncoderValue && ButtonCount == 2 && currentMode == MODE_ALARM) {
			if (encoderValue > previousEncoderValue) {
				modeTimeCount++;
			} else {
				modeTimeCount--;
			}

			previousEncoderValue = encoderValue;

			if (modeTimeCount >= 1) {
				modeTimeCount = 0;
				if (++alarm_minute > 59) {
					alarm_minute = 0;
				}
			} else if (modeTimeCount <= -1) {
				modeTimeCount = 0;
				if (--alarm_minute <= 0) {
					alarm_minute = 59;
				}
			}
		}


}
void mode(float TEMP) {

	encoderValue = read_encoder();

    second = rtcTime.seconds;
    minute = rtcTime.minutes;
    hour  = rtcTime.hour;
    dayw = rtcTime.dayofweek;
    day = rtcTime.dayofmonth;
    month = rtcTime.month;
    year = rtcTime.year + 1952;

    //button_change();

    if (encoderValue != previousEncoderValue && ButtonCount == 0) {
        if (encoderValue > previousEncoderValue) {
            modeChangeCount++;
        } else {
            modeChangeCount--;
        }

        previousEncoderValue = encoderValue;

        if (modeChangeCount >= 3) {
            modeChangeCount = 0;
            currentMode = (currentMode == MODE_ALARM) ? MODE_MIN_SEC : (ClockMode)(currentMode + 1);
            buzzer = true;
            GPIOB->BSRR = GPIO_PIN_1;
            delay_counter = 0;
        } else if (modeChangeCount <= -3) {
            modeChangeCount = 0;
            currentMode = (currentMode == MODE_MIN_SEC) ? MODE_ALARM : (ClockMode)(currentMode - 1);
            buzzer = true;
            GPIOB->BSRR = GPIO_PIN_1;
            delay_counter = 0;
        }
    }
    encoder_Inc_Dec();
    if (buzzer) {
        delay_counter++;
        if (delay_counter >= 5) {
            GPIOB->BSRR = GPIO_PIN_1 << 16;
            buzzer = false;
        }
    }

    switch(currentMode) {
        case MODE_MIN_SEC:
        	static bool previousButtonState_MS = false;
			bool currentButtonState_MS = GPIOA->IDR & GPIO_PIN_2;
			if(!currentButtonState_MS && previousButtonState_MS){
				if(++ButtonCount >= 3){
					ButtonCount = 0;
				}
				Set_Time(new_second, new_minute, hour, dayw, day, month, year);
			}
			previousButtonState_MS = currentButtonState_MS;

            if (ButtonCount == 0) {
                GPIOA->BSRR = GPIO_PIN_3;
                GPIOA->BSRR = GPIO_PIN_4 << 16;

                clear();
                setNumber(1, minute / 10);
                setNumber(2, minute % 10);
                setNumber(3, second / 10);
                setNumber(4, second % 10);
                show();
                loop();

            } else {
                setNumber(1, new_minute / 10);
                setNumber(2, new_minute % 10);
                setNumber(3, new_second / 10);
                setNumber(4, new_second % 10);
                show();
                loop();
            }
            break;
        case MODE_HOUR_MIN:
        	GPIOA->BSRR = GPIO_PIN_4 << 16;
        	blink_led();
        	static bool previousButtonState_HM = false;
			bool currentButtonState_HM = GPIOA->IDR & GPIO_PIN_2;
			if(!currentButtonState_HM && previousButtonState_HM){
				if(++ButtonCount >= 3){
					ButtonCount = 0;
				}
				Set_Time(second, new_minute, new_hour, dayw, day, month, year);
			}
			previousButtonState_HM = currentButtonState_HM;
			if (ButtonCount == 0) {
				clear();
				setNumber(1, hour / 10);
				setNumber(2, hour % 10);
				setNumber(3, minute / 10);
				setNumber(4, minute % 10);
				show();
				loop();
			} else{
	            clear();
	            setNumber(1, new_hour / 10);
	            setNumber(2, new_hour % 10);
	            setNumber(3, new_minute / 10);
	            setNumber(4, new_minute % 10);
	            show();
	            loop();
			}
            break;
        case MODE_DAY_MON:
            GPIOA->BSRR = GPIO_PIN_3;
            GPIOA->BSRR = GPIO_PIN_4 << 16;
        	static bool previousButtonState_DM = false;
			bool currentButtonState_DM = GPIOA->IDR & GPIO_PIN_2;
			if(!currentButtonState_DM && previousButtonState_DM){
				if(++ButtonCount >= 3){
					ButtonCount = 0;
				}
				Set_Time(second, minute, hour, dayw, new_day, new_month, year);
			}
			previousButtonState_DM = currentButtonState_DM;
            if (ButtonCount == 0) {
				clear();
				setNumber(1, day / 10);
				setNumber(2, day % 10);
				setNumber(3, month / 10);
				setNumber(4, month % 10);
				show();
				loop();
            } else {
                clear();
                setNumber(1, new_day / 10);
                setNumber(2, new_day % 10);
                setNumber(3, new_month / 10);
                setNumber(4, new_month % 10);
                show();
                loop();
            }

            break;
        case MODE_YEAR:
            GPIOA->BSRR = GPIO_PIN_3 << 16;
            GPIOA->BSRR = GPIO_PIN_4 << 16;
            static bool previousButtonState_Y = false;
			bool currentButtonState_Y = GPIOA->IDR & GPIO_PIN_2;
			if(!currentButtonState_Y && previousButtonState_Y){
				if(++ButtonCount >= 2){
					ButtonCount = 0;
				}
				Set_Time(second, minute, hour, dayw, day, month, new_year);
			}
			previousButtonState_Y = currentButtonState_Y;
            if (ButtonCount == 0) {
				printInt(year, true);
				show();
				loop();
            } else {
                printInt(new_year, true);
                show();
                loop();
            }
            break;
        case MODE_TEMP:
            GPIOA->BSRR = GPIO_PIN_3 << 16;
            GPIOA->BSRR = GPIO_PIN_4 << 16;
            clear();
            setNumber(1, TEMP / 10);
            setNumber(2, (int)TEMP % 10);
            setChar(3, __DEGREE);
            setChar(4, __C);
            show();
            loop();
            break;
        case MODE_ALARM:
			if(alarm_hour == rtcTime.hour && alarm_minute == rtcTime.minutes && alarm_hour != 0 && setup_alarm){
				int currentSecond = second;
					if (currentSecond != previousSecond) {
						previousSecond = currentSecond;
						if (currentSecond % 2 == 0) {
							GPIOA->BSRR = GPIO_PIN_4;
							GPIOB->BSRR = GPIO_PIN_1;
						} else {
							GPIOA->BSRR = GPIO_PIN_4 << 16;
							GPIOB->BSRR = GPIO_PIN_1 << 16;
						}
					}
				if(!(GPIOA->IDR & GPIO_PIN_2)){

					alarm_hour = 0;
					alarm_minute = 0;
					setup_alarm = false;
					GPIOB->BSRR = GPIO_PIN_1 << 16;
					ButtonCount = 3;

				}
			} else {
				GPIOA->BSRR = GPIO_PIN_4;
				static bool previousButtonState_AL = false;
				bool currentButtonState_AL = GPIOA->IDR & GPIO_PIN_2;
				if(!currentButtonState_AL && previousButtonState_AL){
					if(++ButtonCount >= 3){
						ButtonCount = 0;
						setup_alarm = true;
					}
				}
				previousButtonState_AL = currentButtonState_AL;
			}
			if (ButtonCount == 0) {

				clear();
				blink_led();
				setNumber(1, alarm_hour / 10);
				setNumber(2, alarm_hour % 10);
				setNumber(3, alarm_minute / 10);
				setNumber(4, alarm_minute % 10);
				show();
				loop();
			} else {
				clear();
				setNumber(1, alarm_hour / 10);
				setNumber(2, alarm_hour % 10);
				setNumber(3, alarm_minute / 10);
				setNumber(4, alarm_minute % 10);
				show();
				loop();
			}
            break;

        default:
            // Trường hợp mặc định, không xử lý gì cả
            break;
    }
}
