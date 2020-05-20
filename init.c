#include "init.h"
#include "stm32f446xx.h"
#include "define.h"
#include "flash.h"
#include "led.h"
#include "device.h"

// CLOCK
#define APB1_CLK CPU_CLK/4
#define APB2_CLK CPU_CLK/2
#define APB1_TIMER_CLK 2*APB1_CLK
#define APB2_TIMER_CLK 2*APB2_CLK

// UART
#define BR_UART1 2048000 // USB 
#define BR_UART2 2560000 // DDC 
#define BR_UART3 2560000 // DUC


// Master Functions
static void Gpio(void);
static void Systick(void);
static void Uart(void);
static void Dma(void);
static void Spi(void);
static void FlashMemory(void);
static void Rcc(void);
static void Tim(void);
static void Irq(void);
static void W5500(void);

// Master struct
const struct initFunctions Init = {
	Gpio,
	Systick,
	Uart,
	Dma,
	Spi,
	FlashMemory,
	Rcc,
	Tim,
	Irq,
	W5500
};

// Describe master functions
static void Gpio(void) {
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Port A
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Port B
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Port C
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Port D
	
	// USART1 - USB
	// PA8 - Link
	GPIOA->MODER |= 0<< GPIO_MODER_MODER8_Pos; // input mode
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_8; // Push-pul
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8; // Very high speed  
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8; // no pull-down, no pull-up
	// PA9 - TX	
	GPIOA->MODER |= 0x2 << GPIO_MODER_MODE9_Pos; // Alternate function mode
	GPIOA->OTYPER |= 0x0 << GPIO_OTYPER_OT9_Pos; // Push-pul
	GPIOA->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED9_Pos; // Very high speed
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD9_Pos; // Pull-down
	GPIOA->AFR[1] |= 0x7 << GPIO_AFRH_AFSEL9_Pos; // AF7
	// PA10 - RX	
	GPIOA->MODER |= 0x2 << GPIO_MODER_MODE10_Pos; // Alternate function mode
	GPIOA->OTYPER |= 0x0 << GPIO_OTYPER_OT10_Pos; // Push-pul
	GPIOA->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED10_Pos; // Very high speed
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD10_Pos; // Pull-down
	GPIOA->AFR[1]  |= 0x7 << GPIO_AFRH_AFSEL10_Pos; // AF7
	// PA11 - CTS	
 	GPIOA->MODER |= 0x2 << GPIO_MODER_MODER11_Pos; // Alternate function mode
 	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_11; // Push-pul
 	GPIOA->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED11_Pos; // Very high speed
 	GPIOA->AFR[1] |= 0x7 << GPIO_AFRH_AFSEL11_Pos; // AF7
 	// PA12 - RTS	
 	GPIOA->MODER |= 0x2 << GPIO_MODER_MODER12_Pos; // Alternate function mode
 	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_12; // Push-pul
 	GPIOA->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED12_Pos; // Very high speed
 	GPIOA->AFR[1] |= 0x7 << GPIO_AFRH_AFSEL12_Pos; // AF7	
	
	// USART2 - DDC
	// PA2 - TX	
	GPIOA->MODER |= 0x2 << GPIO_MODER_MODER2_Pos;//GPIO_MODER_MODE2_Pos; // Alternate function mode
	GPIOA->OTYPER |= 0x0 << GPIO_OTYPER_OT2_Pos; // Push-pul
	GPIOA->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED2_Pos; // Very high speed
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD2_Pos; // Pull-down
	GPIOA->AFR[0] |= 0x7 << GPIO_AFRL_AFSEL2_Pos; // AF7
	// PA3 - RX	
	GPIOA->MODER |= 0x2 << GPIO_MODER_MODE3_Pos; // Alternate function mode
	GPIOA->OTYPER |= 0x0 << GPIO_OTYPER_OT3_Pos; // Push-pul
	GPIOA->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED3_Pos; // Very high speed
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD3_Pos; // Pull-down
	GPIOA->AFR[0]  |= 0x7 << GPIO_AFRL_AFSEL3_Pos; // AF7
	
	// USART3 - DUC
	// PB10 - TX	
	GPIOB->MODER |= 0x2 << GPIO_MODER_MODER10_Pos;//GPIO_MODER_MODE2_Pos; // Alternate function mode
	GPIOB->OTYPER |= 0x0 << GPIO_OTYPER_OT10_Pos; // Push-pul
	GPIOB->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED10_Pos; // Very high speed
	GPIOB->PUPDR |= 0x2 << GPIO_PUPDR_PUPD10_Pos; // Pull-down
	GPIOB->AFR[1] |= 0x7 << GPIO_AFRH_AFSEL10_Pos; // AF7
	// PC5 - RX	
	GPIOC->MODER |= 0x2 << GPIO_MODER_MODE5_Pos; // Alternate function mode
	GPIOC->OTYPER |= 0x0 << GPIO_OTYPER_OT5_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED5_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2 << GPIO_PUPDR_PUPD5_Pos; // Pull-down
	GPIOC->AFR[0]  |= 0x7 << GPIO_AFRL_AFSEL5_Pos; // AF7
	
	// SPI1 - Wiznet W5500
	// MOSI
	GPIOB->MODER &= ~GPIO_MODER_MODER5;
	GPIOB->MODER |= 2U << GPIO_MODER_MODER5_Pos;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_5;
  GPIOB->OSPEEDR |= 3U << GPIO_OSPEEDR_OSPEED5_Pos;
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5; 
  GPIOB->PUPDR |= 2U << GPIO_PUPDR_PUPD5_Pos;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5;
	GPIOB->AFR[0] |= 5U << GPIO_AFRL_AFSEL5_Pos;    
	// MISO
	GPIOB->MODER |= 2U << GPIO_MODER_MODER4_Pos;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_4;
	GPIOB->OSPEEDR |= 3U << GPIO_OSPEEDR_OSPEED4_Pos;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4; 
	GPIOB->PUPDR |= 2U << GPIO_PUPDR_PUPD4_Pos;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4;
	GPIOB->AFR[0] |= 5U << GPIO_AFRL_AFSEL4_Pos;
	// SCK
	GPIOB->MODER &= ~GPIO_MODER_MODER3;
	GPIOB->MODER |= 2U << GPIO_MODER_MODER3_Pos;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_3;
	GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3;
	GPIOB->OSPEEDR |= 3U << GPIO_OSPEEDR_OSPEED3_Pos;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD3; 
	GPIOB->PUPDR |= 2U << GPIO_PUPDR_PUPD3_Pos;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3;
	GPIOB->AFR[0] |= 5U << GPIO_AFRL_AFSEL3_Pos; 
	// Reset
	GPIOB->MODER |= GPIO_MODER_MODER6_0;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_6;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_0;
	// CS
	GPIOD->MODER &= ~GPIO_MODER_MODER2;
	GPIOD->MODER |= 1U << GPIO_MODER_MODER2_Pos;
	GPIOD->OTYPER &= ~GPIO_OTYPER_OT_2;
	GPIOD->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;
	GPIOD->OSPEEDR |= 3U << GPIO_OSPEEDR_OSPEED2_Pos;
	GPIOD->PUPDR &= ~GPIO_PUPDR_PUPDR2; 
	GPIOD->PUPDR |= 1U << GPIO_PUPDR_PUPD2_Pos;  

	// Button - PC11
	GPIOC->MODER |= 0 << GPIO_MODER_MODER11_Pos; // input mode
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_11; // Push-pul
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11; // Very high speed  
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR11; // no Pull-down, no pull-up 

	// RGB LED
	//PC13 - red
	GPIOC->MODER |= 0x1U << GPIO_MODER_MODE13_Pos; // Output mode
	GPIOC->OTYPER |= 0x0U << GPIO_OTYPER_OT13_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED13_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2U << GPIO_PUPDR_PUPD13_Pos; // Pull-down
  // PC14 - blue
  GPIOC->MODER |= 0x1U << GPIO_MODER_MODE14_Pos; // Output mode
	GPIOC->OTYPER |= 0x0U << GPIO_OTYPER_OT14_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED14_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2U << GPIO_PUPDR_PUPD14_Pos; // Pull-down
  // PC15 - green
  GPIOC->MODER |= 0x1U << GPIO_MODER_MODE15_Pos; // Output mode
	GPIOC->OTYPER |= 0x0U << GPIO_OTYPER_OT15_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED15_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2U << GPIO_PUPDR_PUPD15_Pos; // Pull-down
	
	RED_OFF; GREEN_OFF; BLUE_OFF;
}
static void Systick(void){
	SysTick->LOAD = 0xFFFFFFFFU;
	SysTick->VAL = 0xFFFFFFFFU;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;  
}
static void Uart(){
	
	// USART 1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Clock	
	USART1->BRR = (uint32_t) (APB2_CLK/BR_UART1); // Baud rate
	USART1->CR1 |= USART_CR1_TE; // Transmitter enable
	USART1->CR1 |= USART_CR1_RE; // Receiver enable
	USART1->CR1 |= USART_CR1_RXNEIE; // RXNE interrupt enable
	USART1->CR3 |= USART_CR3_DMAT; // DMA enable transmitter
	USART1->CR3 |= USART_CR3_CTSE; // CTS mode enabled
	USART1->CR3 |= USART_CR3_RTSE; // RTS output enabled
	USART1->CR1 |= USART_CR1_UE; // USART enable 	
	
	// USART 2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	USART2->BRR = ( APB1_CLK / BR_UART2 );
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_RXNEIE;
	USART2->CR3 |= USART_CR3_DMAT;   
	USART2->CR1 |= USART_CR1_UE;

	// USART 3
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	USART3->BRR = ( APB1_CLK / BR_UART3 );
	USART3->CR1 |= USART_CR1_TE;
	USART3->CR1 |= USART_CR1_RE;
	USART3->CR1 |= USART_CR1_RXNEIE;
	USART3->CR3 |= USART_CR3_DMAT;   
	USART3->CR1 |= USART_CR1_UE;
}
static void Dma(void){   
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;	

	DMA1_Stream3->PAR = (int)&(USART3->DR);
	DMA1_Stream3->CR |= 1 << DMA_SxCR_MINC_Pos;
	DMA1_Stream3->CR |= 1 << DMA_SxCR_DIR_Pos;
	DMA1_Stream3->CR |= 4 << DMA_SxCR_CHSEL_Pos;
	
	DMA1_Stream6->PAR = (int)&(USART2->DR);
	DMA1_Stream6->CR |= 1 << DMA_SxCR_MINC_Pos;
	DMA1_Stream6->CR |= 1 << DMA_SxCR_DIR_Pos;
	DMA1_Stream6->CR |= 4 << DMA_SxCR_CHSEL_Pos;

	DMA2_Stream7->PAR = (int)&(USART1->DR);
	DMA2_Stream7->CR |=  1 << DMA_SxCR_MINC_Pos;
	DMA2_Stream7->CR |=  1 << DMA_SxCR_DIR_Pos ;
	DMA2_Stream7->CR |=  4 << DMA_SxCR_CHSEL_Pos;
}
static void Spi(void){
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Clock SPI
	SPI1->CR1 &= ~SPI_CR1_CPOL; //Polarity cls
	SPI1->CR1 &= ~SPI_CR1_CPHA; //Phase cls
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // Frame format - MSB first
	SPI1->CR1 |= SPI_CR1_MSTR; // Master configuration
	SPI1->CR1&= ~SPI_CR1_SSM;
	SPI1->CR1|= 1U <<SPI_CR1_SSM_Pos;
	SPI1->CR1&= ~SPI_CR1_SSI;
	SPI1->CR1|= 1U << SPI_CR1_SSI_Pos;
	SPI1->CR1 |= SPI_CR1_SPE;
}
static void FlashMemory(void){
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
	FLASH->ACR |= FLASH_ACR_PRFTEN;	// Prefetch enable
	FLASH->ACR |= FLASH_ACR_ICEN; // Instruction cache enable
	FLASH->ACR |= FLASH_ACR_DCEN;	// Data cache enable
}
static void Rcc(void){
	RCC->CR |= RCC_CR_HSEON; // HSE clock enable
	while(!(RCC->CR & RCC_CR_HSERDY)); // HSE clock ready flag
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; // Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM; // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
	RCC->PLLCFGR |= 9 << RCC_PLLCFGR_PLLM_Pos;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN; // Main PLL (PLL) multiplication factor for VCO
	RCC->PLLCFGR |= 200 << RCC_PLLCFGR_PLLN_Pos;//200-2048000
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP; // Main PLL (PLL) division factor for main system clock 
	RCC->PLLCFGR |= 0x0 << RCC_PLLCFGR_PLLP_Pos;	
	RCC->CR |= RCC_CR_PLLON; // Main PLL (PLL) enable
	while(!(RCC->CR & RCC_CR_PLLRDY)); // Main PLL (PLL) clock ready flag
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB Low speed prescaler (APB1)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2)
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	RCC->CFGR &= ~RCC_CFGR_SW; // System clock switch
	RCC->CFGR |= 0x2 << RCC_CFGR_SW_Pos;
}
static void Tim(void) {	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 10240 - 1 ;
	TIM2->ARR = (uint32_t) 0xf;
	TIM2->CR1 |= TIM_CR1_CEN;
}
static void Irq(void){
    NVIC_SetPriority(USART1_IRQn,1);
    NVIC_SetPriority(USART2_IRQn,1);
    NVIC_SetPriority(USART3_IRQn,1);

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(USART2_IRQn); 
    NVIC_EnableIRQ(USART3_IRQn);
}
static void W5500(void){

	// Load Ethernet settings from internal flash
	ethernet parameter;	
	parameter.Ip = Flash.Read(0x00);
	parameter.Mask = Flash.Read(0x04);
	
	//init
	Device.Ethernet.Set(parameter);
}
