/*
 * waterGaugeRS485.h
 *
 *  Created on: Apr 11, 2023
 *      Author: ADMIN
 */

#ifndef WATERGAUGERS485_H_
#define WATERGAUGERS485_H_

#include "stm32f1xx_hal.h"

#include <cstring>

typedef enum{
	DR1 			= 0x01U,
	DR2				= 0x02U,
	DR3				= 0x03U,
	DR4				= 0x04U,
	DR5				= 0x05U,
	DR6				= 0x06U,
	DR7				= 0x07U,
	DR8				= 0x08U,
	DR9				= 0x09U,
	DR10			= 0x0aU,
}BkpregID;

 enum {
	BEGIN_WARERGAGUE 			= 0x01U,
	READ_OK_STAGE_WATERGAUGE	= 0xfeU,
	MAX_STAGE_WATERGAUGE		= 0xffU,
};

class WaterGauge {
	UART_HandleTypeDef* huart;
	RTC_HandleTypeDef* hrtc;

	GPIO_TypeDef  *POWER_Port;
	uint16_t POWER_Pin;

	uint8_t stage;
	uint16_t valueWater;

	uint8_t pTxData[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0a};

	uint8_t pRxData[10];
	uint8_t pRxDoneFlag;

	uint32_t timePre;

public:
	WaterGauge(	UART_HandleTypeDef* huart,
			RTC_HandleTypeDef* hrtc,
			GPIO_TypeDef  *POWER_Port,
			uint16_t POWER_Pin);
	~WaterGauge();

	void run();
	void run_DMA();
	void rxCpltCallback(UART_HandleTypeDef *huart, uint16_t Size, uint8_t* flagIDLE);
	uint8_t getStage();
	void setStage(uint8_t stage);
	uint8_t getValueWater();

private:

	uint16_t checkCrcModbus(uint8_t* data, uint8_t lenght);
};




#endif /* WATERGAUGERS485_H_ */
