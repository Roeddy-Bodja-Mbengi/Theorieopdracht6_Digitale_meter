// Basiscode voor het starten van eender welk project op een Nucleo-F091RC met Nucleo Extension Shield V2.
//
// OPM:
//	- via 'Project -> Manage -> Select software packs' kies je bij Keil::STM32F0xx_DFP voor versie 2.0.0.
//	- via 'Options for Target -> C/C++' zet je de compiler op C11, optimizations op default en warnings op AC5-like.
// 
// Versie: 20230206

// Includes.
#include "stm32f091xc.h"
#include "stdio.h"
#include "stdbool.h"
#include "leds.h"
#include "buttons.h"
#include "usart2.h"
#include "ad.h"
#include "main.h"

// Variabelen aanmaken. 
// OPM: het keyword 'static', zorgt ervoor dat de variabele enkel binnen dit bestand gebruikt kan worden.
static uint8_t teller = 0;
static volatile uint32_t ticks = 0;
static char text[2500];
static char text1[2500];

// Entry point.
int main(void)
{
	// Initialisaties.
	SystemClock_Config();
	InitIo();
	InitButtons();
	InitLeds();
	InitUsart2(115200);
	InitAd();

	// Oneindige lus starten.
	while (1)
	{	
//		SendP1Data();
//	
		sprintf(text,"\r/FLU5\\253769484_A\r\n"
    "0-0:96.1.4(50217)\r\n"
    "0-0:96.1.1(68465168468354684635464)\r\n"
    "0-0:1.0.0(230204074733W)\r\n"
    "1-0:1.8.1(002371.541*kWh)\r\n"
    "1-0:1.8.2(002624.283*kWh)\r\n"
    "1-0:2.8.1(003559.833*kWh)\r\n"
    "1-0:2.8.2(001218.296*kWh)\r\n"
    "0-0:96.14.0(0002)\r\n"
    "1-0:1.4.0(00.078*kW)\r\n"
    "1-0:1.7.0(00.350*kW)\r\n"
    "1-0:2.7.0(00.000*kW)\r\n"
    "!4BBD\r\n");
		
		StringToUsart2(text);
	}
	
	// Terugkeren zonder fouten... (unreachable).
	return 0;
}
//void SendP1Data(void)
//{
//    snprintf(text, sizeof(text),
//    "\r\n/FLU5\\253769484_A\r\n\n"
//    "0-0:96.1.4(50217)\r\n"
//    "0-0:96.1.1(68465168468354684635464)\r\n"
//    "0-0:1.0.0(230204074733W)\r\n"
//    "1-0:1.8.1(002371.541*kWh)\r\n"
//    "1-0:1.8.2(002624.283*kWh)\r\n"
//    "1-0:2.8.1(003559.833*kWh)\r\n"
//    "1-0:2.8.2(001218.296*kWh)\r\n"
//    "0-0:96.14.0(0002)\r\n"
//    "1-0:1.4.0(00.078*kW)\r\n"
//		"1-0:1.6.0(230203210000W)(06.654*kW)\r\n"
//		"0-0:98.1.0(2)(1-0:1.6.0)(1-0:1.6.0)(230101000000W)(221210170000W)(10.891*kW)(230201000000W)(230103180000W)(08.741*kW)\r\n"
//    "1-0:1.7.0(00.350*kW)\r\n"
//    "1-0:2.7.0(00.000*kW)\r\n"
//    "1-0:21.7.0(00.143*kW)\r\n"
//    "1-0:41.7.0(00.099*kW)\r\n"
//    "1-0:61.7.0(00.106*kW)\r\n"
//    "1-0:22.7.0(00.000*kW)\r\n"
//    "1-0:42.7.0(00.000*kW)\r\n"
//    "1-0:62.7.0(00.000*kW)\r\n"
//    "1-0:32.7.0(230.4*V)\r\n"
//    "1-0:52.7.0(229.6*V)\r\n"
//    "1-0:72.7.0(230.6*V)\r\n"
//    "1-0:31.7.0(001.29*A)\r\n"
//    "1-0:51.7.0(000.52*A)\r\n"
//    "1-0:71.7.0(000.62*A)\r\n"
//    "0-0:96.3.10(1)\r\n"
//    "0-0:17.0.0(999.9*kW)\r\n"
//    "1-0:31.4.0(999*A)\r\n"
//    "0-0:96.13.0()\r\n"
//    "0-1:24.1.0(003)\r\n"
//    "0-1:96.1.1(843549843416849546849)\r\n"
//    "0-1:24.4.0(1)\r\n"
//    "0-1:24.2.3(230204074522W)(01545.064*m3)\r\n"
//    "!4BBD\r\n"
//    );

//  StringToUsart2(text);
//}

// Functie om extra IO's te initialiseren.
void InitIo(void)
{

}

// Handler die iedere 1ms afloopt. Ingesteld met SystemCoreClockUpdate() en SysTick_Config().
void SysTick_Handler(void)
{
	ticks++;
}

// Wachtfunctie via de SysTick.
void WaitForMs(uint32_t timespan)
{
	uint32_t startTime = ticks;
	
	while(ticks < startTime + timespan);
}

// Klokken instellen. Deze functie niet wijzigen, tenzij je goed weet wat je doet.
void SystemClock_Config(void)
{
	RCC->CR |= RCC_CR_HSITRIM_4;														// HSITRIM op 16 zetten, dit is standaard (ook na reset).
	RCC->CR  |= RCC_CR_HSION;																// Internal high speed oscillator enable (8MHz)
	while((RCC->CR & RCC_CR_HSIRDY) == 0);									// Wacht tot HSI zeker ingeschakeld is
	
	RCC->CFGR &= ~RCC_CFGR_SW;															// System clock op HSI zetten (SWS is status geupdatet door hardware)	
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);	// Wachten to effectief HSI in actie is getreden
	
	RCC->CR &= ~RCC_CR_PLLON;																// Eerst PLL uitschakelen
	while((RCC->CR & RCC_CR_PLLRDY) != 0);									// Wacht tot PLL zeker uitgeschakeld is
	
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_PREDIV;								// 01: HSI/PREDIV selected as PLL input clock
	RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV2;										// prediv = /2		=> 4MHz
	RCC->CFGR |= RCC_CFGR_PLLMUL12;													// PLL multiplied by 12 => 48MHz
	
	FLASH->ACR |= FLASH_ACR_LATENCY;												//  meer dan 24 MHz, dus latency op 1 (p 67)
	
	RCC->CR |= RCC_CR_PLLON;																// PLL inschakelen
	while((RCC->CR & RCC_CR_PLLRDY) == 0);									// Wacht tot PLL zeker ingeschakeld is

	RCC->CFGR |= RCC_CFGR_SW_PLL; 													// PLLCLK selecteren als SYSCLK (48MHz)
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// Wait until the PLL is switched on
		
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;												// SYSCLK niet meer delen, dus HCLK = 48MHz
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;												// HCLK niet meer delen, dus PCLK = 48MHz	
	
	SystemCoreClockUpdate();																// Nieuwe waarde van de core frequentie opslaan in SystemCoreClock variabele
	SysTick_Config(48000);																	// Interrupt genereren. Zie core_cm0.h, om na ieder 1ms een interrupt 
																													// te hebben op SysTick_Handler()
}
