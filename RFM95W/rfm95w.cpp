/*
 * HAL.cpp
 *
 *  Created on: Feb 9, 2023
 *      Author: ADMIN
 */

#include "rfm95w.h"

LoRaClass::LoRaClass(	SPI_HandleTypeDef* hspi,
		RTC_HandleTypeDef* hrtc,
		GPIO_TypeDef  *GPIO_NSS_GPIO_Port,
		uint16_t NSS_Pin,
		GPIO_TypeDef  *GPIO_CONTROL_POWER_GPIO_Port,
		uint16_t CONTROL_POWER_Pin,
		GPIO_TypeDef  *GPIO_KEEP_POWER_GPIO_Port,
		uint16_t KEEP_POWER_Pin
){
	this->hspi = hspi;
	this->hrtc = hrtc;
	this->GPIO_NSS_GPIO_Port = GPIO_NSS_GPIO_Port;
	this->NSS_Pin = NSS_Pin;
	this->GPIO_CONTROL_POWER_GPIO_Port = GPIO_CONTROL_POWER_GPIO_Port;
	this->CONTROL_POWER_Pin = CONTROL_POWER_Pin;
	this->GPIO_KEEP_POWER_GPIO_Port = GPIO_KEEP_POWER_GPIO_Port;
	this->KEEP_POWER_Pin = KEEP_POWER_Pin;

	stage = 0;
}

LoRaClass::~LoRaClass(){

}

/**
 * @brief The function starts before running the program HAL5
 * @note Frequency (HAL from 525E6)
 * @note SpreadingFactor (11 -> 12)
 * @note SignalBandwidth (125, 250 KHz)
 * @note CodingRate (5)
 * @note lowDataRateOptimize > 16ms;
 * @retval HAL status
 */

void LoRaClass::run(uint32_t frequency,
		RFM95W_Bw bw,
		RFM95W_SpreadingFactor spreadingFactor ) {


	/* -------------- Init LoRa	 ----------------*/
	if (stage == 1) {

		/* Set Off Power LoRa	 */
		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_SET);
		HAL_Delay(10U);
		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_RESET);

		/* Set On Power LoRa	 */
		HAL_GPIO_WritePin(GPIO_KEEP_POWER_GPIO_Port, KEEP_POWER_Pin, GPIO_PIN_SET);
		HAL_Delay(10U);

		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_SET);
		HAL_Delay(10U);
		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIO_KEEP_POWER_GPIO_Port, KEEP_POWER_Pin, GPIO_PIN_RESET);

		/* Turn On RESET Pin and NSS Pin		 */
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
		//		HAL_Delay(1U);

		/* Set Lora Mode	 */
		pTxData[0] = REG_OP_MODE | 0x80;
		pTxData[1] = 0x80;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set Frequency, RegPaConfig	 */
		pTxData[0] = REG_FR_MSB | 0x80;
		frequency = ((frequency / 1000000UL) << 19) / 32;
		for (uint8_t i = 0; i < 3; i++) {
			pTxData[i + 1] = frequency >> ((2 - i) * 8);
		}
		pTxData[4] = 0xcf;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 5, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set RegOcp, RegLna	 */
		pTxData[0] = REG_OCP | 0x80;
		pTxData[1] = 0x31;
		pTxData[2] = 0x23;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 3, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set RegFifoAddrPtr	 */
		pTxData[0] = REG_FIFO_TX_BASE_ADDR | 0x80;
		pTxData[1] = 0x00;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set RegModemConfig1, RegModemConfig2	 */
		pTxData[0] = REG_MODEM_CONFIG_1 | 0x80;
		pTxData[1] = 0x04 | bw;
		pTxData[2] = spreadingFactor;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 3, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set RegModemConfig3 */
		pTxData[0] = REG_MODEM_CONFIG_3 | 0x80;
		pTxData[1] = LOW_DATA_RATE_OPTIMIZE;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set SyncWord */
		pTxData[0] = REG_SYNC_WORD | 0x80;
		pTxData[1] = SYNC_WORD;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set RegPaDac */
		pTxData[0] = REG_PA_DAC | 0x80;
		pTxData[1] = 0x87;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/*Set Standby Mode	*/
		pTxData[0] = REG_OP_MODE | 0x80;
		pTxData[1] = 0x80 | MODE_STDBY;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set Length TxData with Size = 6 */
		pTxData[0] = REG_PAYLOAD_LENGTH | 0x80;
		pTxData[1] = 0x06;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set TxDone Interprut Pin	 */
		pTxData[0] = REG_DIO_MAPPING_1 | 0x80;
		pTxData[1] = TX_PIN;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set FIFO   */
		pTxData[0] = REG_FIFO | 0x80;
		//		pTxData[1] = ID_MESH;
		//		pTxData[1] = '+';
		//		pTxData[2] = 'U';
		//		pTxData[3] = 'R';
		//		pTxData[4] = 'L';
		//		pTxData[5] = ':';
		//		pTxData[6] = ID_NODE;
		//		pTxData[7] = ',';
		//		pTxData[8] = pData;
		//		sprintf((char*) pTxData + 8, "%u", pData);
		//		pTxData[9] = checkCrcXOR(pTxData, 9U);
		pTxData[1] = 0x95;
		pTxData[2] = 0x45;
		pTxData[3] = pData;
//		pTxData[4] = checkCrcXOR(pTxData + 1, 3U);
		pTxData[4] = volData >> 8;
		pTxData[5] = volData;
		pTxData[6] = checkCrcXOR(pTxData + 1, 5U);

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 7, 1000U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		/* Set RegOpMode, Transmitter Mode */
		pTxData[0] = REG_OP_MODE | 0x80;
		pTxData[1] = 0x80 | MODE_TX;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

		//		stage++;
		//	}
		//
		//	if (stage == 2) {
		//		if (HAL_GetTick() - timePre < 2000U) return;
		//
		//		/* Set Off Power LoRa	 */
		//		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_SET);
		//		HAL_Delay(100U);
		//		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_RESET);

		stage = MAX_STAGE_RFM95W;
	}
}

void LoRaClass::run_DMA(uint32_t frequency,
		RFM95W_Bw bw,
		RFM95W_SpreadingFactor spreadingFactor ) {


	/* -------------- Init LoRa	 ----------------*/
	if (stage == 1) {

		/* Set Off Power LoRa	 */
		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_SET);
		HAL_Delay(100U);
		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_RESET);

		/* Set On Power LoRa	 */
		HAL_GPIO_WritePin(GPIO_KEEP_POWER_GPIO_Port, KEEP_POWER_Pin, GPIO_PIN_SET);
		HAL_Delay(100U);

		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_SET);
		HAL_Delay(100U);
		HAL_GPIO_WritePin(GPIO_CONTROL_POWER_GPIO_Port, CONTROL_POWER_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIO_KEEP_POWER_GPIO_Port, KEEP_POWER_Pin, GPIO_PIN_RESET);

		/* Turn On RESET Pin and NSS Pin		 */
		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
		//		HAL_Delay(1U);

		/* Set Lora Mode	 */
		pTxData[0] = REG_OP_MODE | 0x80;
		pTxData[1] = 0x80;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		/* Delay 30 ns		 */
		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);
		/* Delay 100 ns		 */
		stage++;
	}

	if (stage == 2) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return;

		/* Set Frequency, RegPaConfig	 */
		pTxData[0] = REG_FR_MSB | 0x80;
		frequency = ((frequency / 1000000UL) << 19) / 32;
		for (uint8_t i = 0; i < 3; i++) {
			pTxData[i + 1] = frequency >> ((2 - i) * 8);
		}
		pTxData[4] = 0xcf;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		/* Delay 30 ns		 */

		HAL_SPI_Transmit_DMA(hspi, pTxData, 5);
		/* Delay 100 ns		 */

		stage++;
	}

	if (stage == 3) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return;

		/* Set RegOcp, RegLna	 */
		pTxData[0] = REG_OCP | 0x80;
		pTxData[1] = 0x31;
		pTxData[2] = 0x23;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		/* Delay 30 ns		 */

		HAL_SPI_Transmit_DMA(hspi, pTxData, 3);
		/* Delay 100 ns		 */

		stage++;
	}

	if (stage == 4) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return;

		/* Set RegFifoAddrPtr	 */
		pTxData[0] = REG_FIFO_TX_BASE_ADDR | 0x80;
		pTxData[1] = 0x00;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		/* Delay 30 ns		 */

		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);
		/* Delay 100 ns		 */

		stage++;
	}

	if (stage == 5) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/* Set RegModemConfig1, RegModemConfig2	 */
		pTxData[0] = REG_MODEM_CONFIG_1 | 0x80;
		pTxData[1] = 0x04 | bw;
		pTxData[2] = spreadingFactor;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit_DMA(hspi, pTxData, 3);
		stage++;
	}

	if (stage == 6) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/* Set RegModemConfig3 */
		pTxData[0] = REG_MODEM_CONFIG_3 | 0x80;
		pTxData[1] = LOW_DATA_RATE_OPTIMIZE;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);

		stage++;
	}

	if (stage == 7) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/* Set SyncWord */
		pTxData[0] = REG_SYNC_WORD | 0x80;
		pTxData[1] = SYNC_WORD;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);
		stage++;
	}

	if (stage == 8) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/* Set RegPaDac */
		pTxData[0] = REG_PA_DAC | 0x80;
		pTxData[1] = 0x87;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);
		stage++;
	}

	if (stage == 9) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/*Set Standby Mode	*/
		pTxData[0] = REG_OP_MODE | 0x80;
		pTxData[1] = 0x80 | MODE_STDBY;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);
		HAL_Delay(100U);
		/* Delay 100 ns		 */
		stage++;
	}

	if (stage == 10) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/* Set Length TxData with Size = 2 */
		pTxData[0] = REG_PAYLOAD_LENGTH | 0x80;
		pTxData[1] = 0x03;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);
		stage++;
	}

	if (stage == 11) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/* Set TxDone Interprut Pin	 */
		pTxData[0] = REG_DIO_MAPPING_1 | 0x80;
		pTxData[1] = TX_PIN;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);
		stage++;
	}

	if (stage == 12) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;

		/* Set FIFO   */
		pTxData[0] = REG_FIFO | 0x80;
		pTxData[1] = ID_MESH;
		pTxData[2] = ID_NODE;
		pTxData[3] = pData;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit_DMA(hspi, pTxData, 4);
		stage++;
	}

	if (stage == 13) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return;

		/* Set RegOpMode, Transmitter Mode */
		pTxData[0] = REG_OP_MODE | 0x80;
		pTxData[1] = 0x80 | MODE_TX;

		HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit_DMA(hspi, pTxData, 2);

		stage++;
	}

	if (stage == 14) {
		if (HAL_GPIO_ReadPin(GPIO_NSS_GPIO_Port, NSS_Pin) != GPIO_PIN_SET) return ;
		/* Set Off Power LoRa	 */

		stage = MAX_STAGE_RFM95W;
	}
}

void LoRaClass::txRxCpltCallback(){
	HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
}

void LoRaClass::irqDoneCallback() {
	/* TxDone	 */
	stage = MAX_STAGE_RFM95W;
}

void LoRaClass::setStage(uint8_t func) {
	stage = func;
}

void LoRaClass::setData(uint8_t pData) {
	this->pData = pData;
}

void LoRaClass::setVolData(uint16_t volData) {
	this->volData = volData;
}

uint8_t LoRaClass::getStage() {
	return stage;
}

void LoRaClass::clearFlags() {

	/* Set RegOpMode, Transmitter Mode */
	pTxData[0] = REG_IRQ_FLAGS | 0x80;
	pTxData[1] = 0xff;

	HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, pTxData, 2, 10U);
	HAL_GPIO_WritePin(GPIO_NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);
}

uint8_t LoRaClass::checkCrcXOR(uint8_t* data, uint8_t size) {
	uint8_t XOR = 0;
	for (uint8_t i = 0; i < size ;i++)
	{
		XOR = XOR ^ (*(data + i));
	}

	return XOR;
}


