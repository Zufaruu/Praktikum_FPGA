/**
 * @file main.cpp
 * @author Cosmin Tanislav
 * @author Cristian Fatu
 * @date 15 Nov 2019
 * @brief File containing the ZMOD DAC1411 linux demo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <cmath>

#include "zmod.h"
#include "zmoddac1411.h"

#define TRANSFER_LEN	0x400
#define IIC_BASE_ADDR   0xE0005000
#define ZMOD_IRQ 		61

#define DAC_BASE_ADDR 		0x43C10000
#define DAC_DMA_BASE_ADDR 	0x40410000
#define DAC_FLASH_ADDR   	0x31
#define DAC_DMA_IRQ 		63

#define PI 3.14159265358979323846
/*
 * Simple DAC test, using sine values populated in the buffer.
* @param amplitude - the amplitude for the generated sine
* @param N_sample - the number of generated samples
* @param step - phase increment of the signal's sample
* @param channel - the channel where samples will be generated
* @param frequencyDivider - the output frequency divider
* @param gain - the gain for the channel
*/
void dacRampDemo(float amplitude, int N_sample, int step, uint8_t channel, uint8_t frequencyDivider, uint8_t gain)
{
	ZMODDAC1411 dacZmod(DAC_BASE_ADDR, DAC_DMA_BASE_ADDR, IIC_BASE_ADDR, DAC_FLASH_ADDR, DAC_DMA_IRQ);
	uint32_t *buf;
	float sample_value;
	uint32_t valBuf;
	int16_t valRaw;
	size_t length = N_sample;
	int address;

	if (length > ((1<<14) - 1))
	{
		// limit the length to maximum buffer size (1<<14 - 1)
		length = ((1<<14) - 1);
	}

	buf = dacZmod.allocChannelsBuffer(length);
	dacZmod.setOutputSampleFrequencyDivider(frequencyDivider);
	dacZmod.setGain(channel, gain);

    sample_value = 0;

    address = 0;
    for (int i = 0; i < N_sample; i+=step) {
        sample_value = amplitude * sin((2 * PI * i / N_sample));

        valRaw = dacZmod.getSignedRawFromVolt(sample_value , gain);
		valBuf = dacZmod.arrangeChannelData(channel, valRaw);
		buf[address++] = valBuf;
    }

	// send data to DAC and start the instrument
	dacZmod.setData(buf, length);
	dacZmod.start();
	dacZmod.freeChannelsBuffer(buf, length);
}

int main() {
	std::cout << "ZmodDAC1411 Demo\n";
	// amplitude 				2 V
	// N_sample 				64
	// step 					1
	// channel 					CH1
	// Output Frequency Divider	2
	// gain						HIGH - Corresponds to HIGH input Range
	dacRampDemo(2, 64, 1, 0, 2, 1);
	return 0;
}
