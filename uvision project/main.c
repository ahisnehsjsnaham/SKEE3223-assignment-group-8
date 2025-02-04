#include "stm32f4xx.h"

void System_Init(void) {
RCC->AHB1ENR |= (1 << 0); // Power up port A
RCC->AHB1ENR |= (1 << 1); // Power up port B
GPIOA->MODER |= (3 << (0 * 2)); // PA0 set to analog
GPIOA->MODER &= ~(3 << (1 * 2)); // PA1 set to input
GPIOB->MODER |= (1 << (0 * 2)); // PB0 set to output
GPIOB->MODER |= (1 << (1 * 2)); // PB1 set to output
}

void ADC_Init(void) {
RCC->APB2ENR |= (1 << 8); // Enable ADC1 clock
ADC1->CR1 &= ~(3 << 24); // Set 12-bit resolution
ADC1->CR1 &= ~(1 << 8); // SCAN mode disabled (Bit8)
ADC1->SQR3 |= 0; // Select channel 0 (PA0) as first sequence
ADC1->SQR1 &= ~(15 << 20); // Only 1 conversion
ADC1->CR2 |= (1 << 1); // Enable Continuous mode
ADC1->CR2 &= ~(1 << 11); // Right alignment of data bits (bit12....bit0)
ADC1->SMPR2 |= (3 << (0 * 3)); // Set sampling time (56 cycles) for channel 0 (PA0)
ADC1->CR2 |= (1 << 0); // Enable ADC1
ADC1->CR2 |= (1 << 30); // Start ADC
}

uint16_t ADC_Read(void) {
while (!(ADC1->SR & (1 << 1))); // Wait for EOC flag
return ADC1->DR; // Read ADC value
}

int main(void) {
System_Init();
ADC_Init();

while (1) {
uint16_t light_intensity = ADC_Read();

// Corrected BSRR usage
if (light_intensity >= 2048)
GPIOB->BSRR = (1 << 0); // Set PB0 HIGH
else
GPIOB->BSRR = (1 << (0 + 16)); // Set PB0 LOW

if (GPIOA->IDR & (1 << 1))
GPIOB->BSRR = (1 << (1+16)); // Set PB1 HIGH
else
GPIOB->BSRR = (1 << 1); // Set PB1 LOW
}
}