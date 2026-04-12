#include "stm32f091xc.h"
#include "leds.h"

#if !defined(EXTI_DEFINED)
	#define EXTI_DEFINED
	
	void InitEXTI_IRQ(void);
	void EXTI0_1_IRQHandler(void);
		void EXTI4_15_IRQHandler(void);
#endif