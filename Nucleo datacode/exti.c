#include "stm32f091xc.h"
#include "exti.h"
#include "leds.h"

void InitEXTI_IRQ(void)
{


    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	 SYSCFG->EXTICR[0] |=SYSCFG_EXTICR1_EXTI1_PA;
	

   
    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI4;
	SYSCFG->EXTICR[1] |=SYSCFG_EXTICR1_EXTI2_PA;

    
    EXTI->IMR |= EXTI_IMR_MR1 | EXTI_IMR_MR4;

   
    EXTI->FTSR |= EXTI_FTSR_TR1 | EXTI_FTSR_TR4;


    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

}


//void EXTI0_1_IRQHandler(void)
//{ 
//if(EXTI->PR & EXTI_PR_PR1)
//{

//EXTI->PR|=EXTI_PR_PR1;
//	ByteToLeds(255);
//}

//}

//void EXTI4_15_IRQHandler(void)
//{ 
//if(EXTI->PR & EXTI_PR_PR4)
//{
//	
//EXTI->PR|=EXTI_PR_PR4;
//	ByteToLeds(0);
//}

//}