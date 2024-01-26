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
* @param channel - the channel where samples will be generated
* @param frequencyDivider - the output frequency divider
* @param gain - the gain for the channel
*/
void dacRampDemo(uint8_t channel, uint8_t frequencyDivider, uint8_t gain)
{
	ZMODDAC1411 dacZmod(DAC_BASE_ADDR, DAC_DMA_BASE_ADDR, IIC_BASE_ADDR, DAC_FLASH_ADDR, DAC_DMA_IRQ);

    float ofdm[] = {
        0.07812, -0.02687, -0.09277, -0.07342, 0.00324, 0.07129, 0.08853, 0.06394, 0.03408, 0.02317, 0.02778, 0.03153, 0.02393, 0.00280, -0.03233, -0.07888, -0.12232, -0.13868, -0.11633, -0.07622, -0.06129, -0.09645, -0.15842, -0.19253, -0.16615, -0.10423, -0.06532, -0.07831, -0.10647, -0.08183, 0.02073, 0.14635, 0.20312, 0.14635, 0.02073, -0.08183, -0.10647, -0.07831, -0.06532, -0.10423, -0.16615, -0.19253, -0.15842, -0.09645, -0.06129, -0.07622, -0.11633, -0.13868, -0.12232, -0.07888, -0.03233, 0.00280, 0.02393, 0.03153, 0.02778, 0.02317, 0.03408, 0.06394, 0.08853, 0.07129, 0.00324, -0.07342, -0.09277, -0.02687, 0.07812, 0.13507, 0.09387, -0.00951, -0.08345, -0.06381, 0.02960, 0.11603, 0.12975, 0.07325, 0.00088, -0.04058, -0.04957, -0.05246, -0.06171, -0.06136, -0.03393, 0.00883, 0.03239, 0.01805, -0.01218, -0.01662, 0.01942, 0.06312, 0.06482, 0.00903, -0.06747, -0.10883, -0.08922, -0.02917, 0.03116, 0.06732, 0.07812, 0.06732, 0.03116, -0.02917, -0.08922, -0.10883, -0.06747, 0.00903, 0.06482, 0.06312, 0.01942, -0.01662, -0.01218, 0.01805, 0.03239, 0.00883, -0.03393, -0.06136, -0.06171, -0.05246, -0.04957, -0.04058, 0.00088, 0.07325, 0.12975, 0.11603, 0.02960, -0.06381, -0.08345, -0.00951, 0.09387, 0.13507, 0.07812, -0.02687, -0.09277, -0.07342, 0.00324, 0.07129, 0.08853, 0.06394, 0.03408, 0.02317, 0.02778, 0.03153, 0.02393, 0.00280, -0.03233, -0.07888, -0.12232, -0.13868, -0.11633, -0.07622, -0.06129, -0.09645, -0.15842, -0.19253, -0.16615, -0.10423, -0.06532, -0.07831, -0.10647, -0.08183, 0.02073, 0.14635
    };
    float lfm[] = {
        1.00000, 1.00000, 0.99999, 0.99994, 0.99982, 0.99957, 0.99910, 0.99833, 0.99716, 0.99545, 0.99307, 0.98986, 0.98564, 0.98025, 0.97346, 0.96507, 0.95486, 0.94260, 0.92803, 0.91092, 0.89101, 0.86805, 0.84179, 0.81201, 0.77846, 0.74095, 0.69929, 0.65332, 0.60293, 0.54804, 0.48862, 0.42471, 0.35641, 0.28389, 0.20740, 0.12728, 0.04397, -0.04201, -0.13001, -0.21929, -0.30902, -0.39823, -0.48588, -0.57082, -0.65183, -0.72762, -0.79685, -0.85813, -0.91011, -0.95142, -0.98079, -0.99701, -0.99903, -0.98597, -0.95717, -0.91221, -0.85099, -0.77376, -0.68110, -0.57404, -0.45399, -0.32280, -0.18275, -0.03651, 0.11286, 0.26198, 0.40721, 0.54475, 0.67069, 0.78117, 0.87250, 0.94128, 0.98456, 0.99999, 0.98591, 0.94154, 0.86707, 0.76371, 0.63379, 0.48072, 0.30902, 0.12416, -0.06749, -0.25895, -0.44276, -0.61135, -0.75734, -0.87384, -0.95486, -0.99567, -0.99307, -0.94570, -0.85428, -0.72167, -0.55296, -0.35531, -0.13779, 0.08902, 0.31350, 0.52350, 0.70711, 0.85326, 0.95250, 0.99769, 0.98456, 0.91221, 0.78337, 0.60449, 0.38558, 0.13973, -0.11754, -0.36922, -0.59790, -0.78702, -0.92207, -0.99187, -0.98958, -0.91349, -0.76750, -0.56111, -0.30902, -0.03023, 0.25325, 0.51814, 0.74174, 0.90399, 0.98935, 0.98847, 0.89941, 0.72816, 0.48862, 0.20163, -0.10661, -0.40686, -0.66952, -0.86766, -0.97986, -0.99274, -0.90281, -0.71731, -0.45399, -0.13973, 0.19201, 0.50464, 0.76244, 0.93475, 0.99979, 0.94773, 0.78239, 0.52149, 0.19509, -0.15760, -0.49273, -0.76725, -0.94455, -0.99957, -0.92267, -0.72167, -0.42151, -0.06161, 0.30902, 0.63833, 0.87857, 0.99339, 0.96372, 0.79136, 0.49955, 0.13040, -0.26084, -0.61384, -0.87250, -0.99409, -0.95671, -0.76371, -0.44417, -0.04907, 0.35641, 0.70349, 0.93150, 0.99883, 0.89101, 0.62432, 0.24412, -0.18236, -0.57757, -0.86766, -0.99654, -0.93724, -0.69817, -0.32280, 0.11754, 0.53682, 0.85099, 0.99507, 0.93710, 0.68598, 0.29104, -0.16690, -0.59159, -0.89154
    }
    
    int len = sizeof(ofdm) / sizeof(int);
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
        valRaw = dacZmod.getSignedRawFromVolt(ofdm[idx], gain);
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
	// channel 					CH1
	// Output Frequency Divider	2
	// gain						HIGH - Corresponds to HIGH input Range
	dacRampDemo(0, 2, 1);
	return 0;
}
