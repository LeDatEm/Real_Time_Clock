#include "main.h"

extern TIM_HandleTypeDef htim2;


int read_encoder(void){
	int read = __HAL_TIM_GET_COUNTER(&htim2);
	if(read <=0){
		read = 0;
	}
	return read;
}
int read_switch(void) {

	GPIO_PinState switch_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
	  if (switch_state == GPIO_PIN_RESET) {
		return 0;
	} else {
		return 1;
	}
}
