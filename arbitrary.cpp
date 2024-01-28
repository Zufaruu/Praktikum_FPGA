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
#include <vector>

#include "zmod.h"
#include "zmoddac1411.h"

#define TRANSFER_LEN	0x400
#define IIC_BASE_ADDR   0xE0005000
#define ZMOD_IRQ 		61

#define DAC_BASE_ADDR 		0x43C10000
#define DAC_DMA_BASE_ADDR 	0x40410000
#define DAC_FLASH_ADDR   	0x31
#define DAC_DMA_IRQ 		63

/*
 * Simple DAC test, using arbitrary values populated in the buffer.
* @oaram signal_name - Name of Signal we want to generate (am, ofdm)
* @param channel - the channel where samples will be generated
* @param frequencyDivider - the output frequency divider
* @param gain - the gain for the channel
*/
void dacRampDemo(std::string signal_name, uint8_t channel, uint8_t frequencyDivider, uint8_t gain)
{
	ZMODDAC1411 dacZmod(DAC_BASE_ADDR, DAC_DMA_BASE_ADDR, IIC_BASE_ADDR, DAC_FLASH_ADDR, DAC_DMA_IRQ);

	std::vector<float> signal {0, 0, 0, 0, 0, 0, 0, 0, 0};
	if (signal_name == "am"){
		signal = {
        	0.07812, -0.02687, -0.09277, -0.07342, 0.00324, 0.07129, 0.08853, 0.06394, 0.03408, 0.02317, 0.02778, 0.03153, 0.02393, 0.00280, -0.03233, -0.07888, -0.12232, -0.13868, -0.11633, -0.07622, -0.06129, -0.09645, -0.15842, -0.19253, -0.16615, -0.10423, -0.06532, -0.07831, -0.10647, -0.08183, 0.02073, 0.14635, 0.20312, 0.14635, 0.02073, -0.08183, -0.10647, -0.07831, -0.06532, -0.10423, -0.16615, -0.19253, -0.15842, -0.09645, -0.06129, -0.07622, -0.11633, -0.13868, -0.12232, -0.07888, -0.03233, 0.00280, 0.02393, 0.03153, 0.02778, 0.02317, 0.03408, 0.06394, 0.08853, 0.07129, 0.00324, -0.07342, -0.09277, -0.02687, 0.07812, 0.13507, 0.09387, -0.00951, -0.08345, -0.06381, 0.02960, 0.11603, 0.12975, 0.07325, 0.00088, -0.04058, -0.04957, -0.05246, -0.06171, -0.06136, -0.03393, 0.00883, 0.03239, 0.01805, -0.01218, -0.01662, 0.01942, 0.06312, 0.06482, 0.00903, -0.06747, -0.10883, -0.08922, -0.02917, 0.03116, 0.06732, 0.07812, 0.06732, 0.03116, -0.02917, -0.08922, -0.10883, -0.06747, 0.00903, 0.06482, 0.06312, 0.01942, -0.01662, -0.01218, 0.01805, 0.03239, 0.00883, -0.03393, -0.06136, -0.06171, -0.05246, -0.04957, -0.04058, 0.00088, 0.07325, 0.12975, 0.11603, 0.02960, -0.06381, -0.08345, -0.00951, 0.09387, 0.13507, 0.07812, -0.02687, -0.09277, -0.07342, 0.00324, 0.07129, 0.08853, 0.06394, 0.03408, 0.02317, 0.02778, 0.03153, 0.02393, 0.00280, -0.03233, -0.07888, -0.12232, -0.13868, -0.11633, -0.07622, -0.06129, -0.09645, -0.15842, -0.19253, -0.16615, -0.10423, -0.06532, -0.07831, -0.10647, -0.08183, 0.02073, 0.14635
   	 	};
	}
	else if (signal_name == "ofdm"){
		signal = {
        	0.1, -0.02687, -0.09277, -0.07342, 0.00324, 0.07129, 0.08853, 0.06394, 0.03408, 0.02317, 0.02778, 0.03153, 0.02393, 0.00280, -0.03233, -0.07888, -0.12232, -0.13868, -0.11633, -0.07622, -0.06129, -0.09645, -0.15842, -0.19253, -0.16615, -0.10423, -0.06532, -0.07831, -0.10647, -0.08183, 0.02073, 0.14635, 0.20312, 0.14635, 0.02073, -0.08183, -0.10647, -0.07831, -0.06532, -0.10423, -0.16615, -0.19253, -0.15842, -0.09645, -0.06129, -0.07622, -0.11633, -0.13868, -0.12232, -0.07888, -0.03233, 0.00280, 0.02393, 0.03153, 0.02778, 0.02317, 0.03408, 0.06394, 0.08853, 0.07129, 0.00324, -0.07342, -0.09277, -0.02687, 0.07812, 0.13507, 0.09387, -0.00951, -0.08345, -0.06381, 0.02960, 0.11603, 0.12975, 0.07325, 0.00088, -0.04058, -0.04957, -0.05246, -0.06171, -0.06136, -0.03393, 0.00883, 0.03239, 0.01805, -0.01218, -0.01662, 0.01942, 0.06312, 0.06482, 0.00903, -0.06747, -0.10883, -0.08922, -0.02917, 0.03116, 0.06732, 0.07812, 0.06732, 0.03116, -0.02917, -0.08922, -0.10883, -0.06747, 0.00903, 0.06482, 0.06312, 0.01942, -0.01662, -0.01218, 0.01805, 0.03239, 0.00883, -0.03393, -0.06136, -0.06171, -0.05246, -0.04957, -0.04058, 0.00088, 0.07325, 0.12975, 0.11603, 0.02960, -0.06381, -0.08345, -0.00951, 0.09387, 0.13507, 0.07812, -0.02687, -0.09277, -0.07342, 0.00324, 0.07129, 0.08853, 0.06394, 0.03408, 0.02317, 0.02778, 0.03153, 0.02393, 0.00280, -0.03233, -0.07888, -0.12232, -0.13868, -0.11633, -0.07622, -0.06129, -0.09645, -0.15842, -0.19253, -0.16615, -0.10423, -0.06532, -0.07831, -0.10647, -0.08183, 0.02073, 0.14635
   	 	};
	}

    int len = signal.size();
	uint32_t *buf;
	uint32_t valBuf;
	int16_t valRaw;
	size_t length = len;
	int i;

	if (length > ((1<<14) - 1))
	{
		// limit the length to maximum buffer size (1<<14 - 1)
		length = ((1<<14) - 1);
	}

	buf = dacZmod.allocChannelsBuffer(length);
	dacZmod.setOutputSampleFrequencyDivider(frequencyDivider);
	dacZmod.setGain(channel, gain);

    i = 0;
    for(int idx = 0; idx < len; idx++){
        valRaw = dacZmod.getSignedRawFromVolt(signal[idx], gain);
		valBuf = dacZmod.arrangeChannelData(channel, valRaw);
		buf[i++] = valBuf;
    }

	// send data to DAC and start the instrument
	dacZmod.setData(buf, length);
	dacZmod.start();
	dacZmod.freeChannelsBuffer(buf, length);
}

int main() {
	std::cout << "ZmodDAC1411 Demo\n";
	// signal_name				Name of Signal we want to generate (am, ofdm)
	// channel 					CH1
	// Output Frequency Divider	2
	// gain						HIGH - Corresponds to HIGH input Range
	dacRampDemo("am", 0, 2, 1);
	return 0;
}
