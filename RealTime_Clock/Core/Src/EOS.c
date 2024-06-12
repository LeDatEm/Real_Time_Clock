
#include "main.h"

void EOS_init(uint32_t delay_ms){
	uint32_t delay_ticks = delay_ms * (16000000 / 1000);
	SysTick->LOAD = delay_ticks;
	SysTick->VAL = 0;
	SysTick->CTRL = 7;
}
