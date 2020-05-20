#ifndef LED_H
#define LED_H

#define RED_ON GPIOC->BSRR = GPIO_BSRR_BR_15
#define GREEN_ON GPIOC->BSRR = GPIO_BSRR_BR_14
#define BLUE_ON GPIOC->BSRR = GPIO_BSRR_BR_13
#define RED_OFF GPIOC->BSRR = GPIO_BSRR_BS_15
#define GREEN_OFF GPIOC->BSRR = GPIO_BSRR_BS_14
#define BLUE_OFF GPIOC->BSRR = GPIO_BSRR_BS_13
#define RED_TOOGLE GPIOC->ODR ^= GPIO_ODR_ODR_15
#define GREEN_TOOGLE GPIOC->ODR ^= GPIO_ODR_ODR_14
#define BLUE_TOOGLE GPIOC->ODR ^= GPIO_ODR_ODR_13

extern const struct ledFunctions {
	void (*const Update)(void);
} Led;

#endif
