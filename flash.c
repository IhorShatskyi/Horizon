#include "flash.h"

#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB
#define DEVICE_ADDRESS  0x08060000

// master functions
static void flashWrite(uint32_t, uint32_t);
static uint32_t flashRead(uint32_t);
static void flashLock(void);
static void flashUnlock(void);
static void flashEraseSector(uint32_t);

// slave functions 
static uint8_t flashReady(void);

// struct functions
const struct flashFunction Flash = {
	flashWrite,
	flashRead,
	flashLock,
	flashUnlock,
	flashEraseSector
};

// describe master functions
void flashWrite(uint32_t offset, uint32_t data) {
    
	// waiting ready
	while(!flashReady());

	// enable flash programing
	FLASH->CR |= FLASH_CR_PG;
    
	// waiting ready
	while(!flashReady());
    
	// writing  
	*(__IO uint32_t*)(DEVICE_ADDRESS + offset) = (uint32_t) data;

	// waiting ready
	while(!flashReady());
    
	// disable flash programing
	FLASH->CR &= ~FLASH_CR_PG;
}
uint32_t flashRead(uint32_t offset) {  
  return (*(__IO uint32_t*) (DEVICE_ADDRESS + offset));
}
void flashLock(void) {
    FLASH->CR |= FLASH_CR_LOCK;
}
void flashUnlock(void) {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;   
}
void flashEraseSector(uint32_t sector) {
    
    // waiting ready
    while(!flashReady());

    FLASH->CR &= ~FLASH_CR_PSIZE;
    FLASH->CR |= 2u << FLASH_CR_PSIZE_Pos;
    
    // set bit erase
    FLASH->CR |= FLASH_CR_SER;
    
    // set address page
    FLASH->CR |= sector << FLASH_CR_SNB_Pos;

    // start erase
    FLASH->CR |= FLASH_CR_STRT;
    
    // waiting ready
    while(!flashReady());
    
    // clear bit erase
    FLASH->CR &= ~FLASH_CR_SER;
}

// describe slave functions
uint8_t flashReady(void) {
    return !(FLASH->SR & FLASH_SR_BSY);
}
