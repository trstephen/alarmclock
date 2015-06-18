#include "Audio.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"

#include <stdlib.h>

static void WriteRegister(uint8_t address, uint8_t value);
static void StartAudioDMAAndRequestBuffers();
static void StopAudioDMA();

static AudioCallbackFunction *CallbackFunction;
static void *CallbackContext;
static int16_t * volatile NextBufferSamples;
static volatile int NextBufferLength;
static volatile int BufferNumber;
static volatile bool DMARunning;

	uint32_t *ptr;
	uint32_t *ptrSR;
	uint32_t *ptrDOR;
	uint32_t *ptrTIM;
void InitializeAudio(int plln, int pllr, int i2sdiv, int i2sodd) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef 	DAC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;

	// Initialize state.
	CallbackFunction = NULL;
	CallbackContext = NULL;
	NextBufferSamples = NULL;
	NextBufferLength = 0;
	BufferNumber = 0;
	DMARunning = false;
    volatile int i;

	// Turn on peripherals.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);





			TIM_TimeBaseStructInit(&TIM_InitStruct);
			TIM_InitStruct.TIM_Period	= 90;	//approx 44khz
			TIM_InitStruct.TIM_Prescaler = 20;
			TIM_TimeBaseInit(TIM6, &TIM_InitStruct);

			/* TIM2 TRGO selection */
			TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

		// Enable GPIO port A4 as an analog output
		//
			GPIO_StructInit( &GPIO_InitStructure );
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

		//
		// Configure DAC channel 1
		//
			DAC_StructInit( &DAC_InitStruct );
			DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
			DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
			DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable ;
			DAC_Init(DAC_Channel_1, &DAC_InitStruct );

			//
			// Enable DAC channel 1
			//
	//		DMAEnable();

			NVIC_InitTypeDef      NVIC_InitStructure;
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

}

void AudioOn() {
/*	WriteRegister(0x02, 0x9e);
	SPI3 ->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1
			| SPI_I2SCFGR_I2SE; // Master transmitter, Phillips mode, 16 bit values, clock polarity low, enable.*/
}

void AudioOff() {
/*	WriteRegister(0x02, 0x01);
	SPI3 ->I2SCFGR = 0;*/
}

void SetAudioVolume(int volume) {
	/*
	WriteRegister(0x20, (volume + 0x19) & 0xff);
	WriteRegister(0x21, (volume + 0x19) & 0xff);*/
}

void OutputAudioSample(int16_t sample) {
	//while (!(SPI3 ->SR & SPI_SR_TXE ))
	//	;
	//SPI3 ->DR = sample;
}

void OutputAudioSampleWithoutBlocking(int16_t sample) {
	//SPI3 ->DR = sample;
}

void PlayAudioWithCallback(AudioCallbackFunction *callback, void *context) {
	StopAudioDMA();

	CallbackFunction = callback;
	CallbackContext = context;
	BufferNumber = 0;

	if (CallbackFunction)
		CallbackFunction(CallbackContext, BufferNumber);
}

void StopAudio() {

	StopAudioDMA();

	CallbackFunction = NULL;
}

void ProvideAudioBuffer(void *samples, int numsamples) {
	while (!ProvideAudioBufferWithoutBlocking(samples, numsamples))
			__asm__ volatile ("wfi");


}

bool ProvideAudioBufferWithoutBlocking(void *samples, int numsamples) {
	int loop=0;

	if (NextBufferSamples)
		return false;

	NextBufferSamples = samples;
	NextBufferLength  = numsamples;

	 	 ptr = &DAC->DHR12R1;
	 	 ptrSR = &DAC->SR;	//chck for dma underrrun
	     ptrDOR = &DAC->DOR1;

	if (!DMARunning)
		StartAudioDMAAndRequestBuffers();

	return true;
}

static void WriteRegister(uint8_t address, uint8_t value) {
	/*
	while (I2C1 ->SR2 & I2C_SR2_BUSY )
		;

	I2C1 ->CR1 |= I2C_CR1_START; // Start the transfer sequence.
	while (!(I2C1 ->SR1 & I2C_SR1_SB ))
		; // Wait for start bit.

	I2C1 ->DR = 0x94;
	while (!(I2C1 ->SR1 & I2C_SR1_ADDR ))
		; // Wait for master transmitter mode.
	I2C1 ->SR2;

	I2C1 ->DR = address; // Transmit the address to write to.
	while (!(I2C1 ->SR1 & I2C_SR1_TXE ))
		; // Wait for byte to move to shift register.

	I2C1 ->DR = value; // Transmit the value.

	while (!(I2C1 ->SR1 & I2C_SR1_BTF ))
		; // Wait for all bytes to finish.
	I2C1 ->CR1 |= I2C_CR1_STOP; // End the transfer sequence.*/
}

static void StartAudioDMAAndRequestBuffers() {


	// Configure DMA stream.
	DMA1_Stream5 ->CR = /*(0 * DMA_SxCR_CHSEL_0 )*/0x0E000000  | // Channel 7
			(1 * DMA_SxCR_PL_0 ) | // Priority 1
			(1 * DMA_SxCR_PSIZE_0 ) | // PSIZE = 16 bit
			(1 * DMA_SxCR_MSIZE ) | // MSIZE = 16 bit
			DMA_SxCR_MINC | // Increase memory address
			(1 * DMA_SxCR_DIR_0 ) | // Memory to peripheral
			DMA_SxCR_TCIE; // Transfer complete interrupt
	DMA1_Stream5 ->NDTR = NextBufferLength >> 1;
	DMA1_Stream5 ->PAR = (uint32_t) &DAC->DHR12R1;
	DMA1_Stream5 ->M0AR = (uint32_t) NextBufferSamples;
	DMA1_Stream5 ->FCR = DMA_SxFCR_DMDIS;
	DMA1_Stream5 ->CR |= DMA_SxCR_EN;


	//TIM_Cmd(TIM6, DISABLE);

	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_DMACmd(DAC_Channel_1, ENABLE); //DMAEN1

	  TIM_Cmd(TIM6, ENABLE);

	// Update state.
	NextBufferSamples = NULL;
	BufferNumber ^= 1;
	DMARunning = true;

	// Invoke callback if it exists to queue up another buffer.
	if (CallbackFunction)
		CallbackFunction(CallbackContext, BufferNumber);
}

static void StopAudioDMA() {

	DMA1_Stream5 ->CR &= ~DMA_SxCR_EN; // Disable DMA stream.
		while (DMA1_Stream5 ->CR & DMA_SxCR_EN )
			; // Wait for DMA stream to stop.*/

	DMARunning = false;
}


void DMA1_Stream5_IRQHandler()
{
	DMA1 ->HIFCR |= DMA_HIFCR_CTCIF5; // Clear interrupt flag.

	if (NextBufferSamples) {
			StartAudioDMAAndRequestBuffers();
		}
	else
	{
			DMARunning = false;
		}

}
