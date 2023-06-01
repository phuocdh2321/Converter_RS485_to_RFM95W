/*
 * rfm95m.h
 *
 *  Created on: Feb 9, 2023
 *      Author: ADMIN
 */

#ifndef RFM95W_H_
#define RFM95W_H_

#include "stm32f1xx_hal.h"

#include <cstdio>

/* LongRangeMode */
#define LORA_MODE				(0x80U)		// LoRa Mode

#define HIGH_FREQUENCY_MODE		(0x00U)

/* LowDataRateOptimize */
#define LOW_DATA_RATE_OPTIMIZE	(0x08U)		// Enabled; mandated for when the symbol length exceeds 16ms

/* OcpOn */
#define OCP_ON					(0x20U)

/* RxPayloadCrcOn */
#define RX_PAYLOAD_CRC_ON		(0x04U)

/* Version*/
#define VERSION 				(0x12U)
#define OP_MODE_DEFAULT			(0x09U)

/* ID Mesh */
#define ID_MESH					(0x23U)
#define ID_NODE					(0x00U)
#define SYNC_WORD				(0x12U)

/* LoRa Mode Register Map */
enum {
	REG_FIFO					= 0x00U,
	REG_OP_MODE					= 0x01U,
	REG_FR_MSB					= 0x06U,
	REG_FR_MID					= 0x07U,
	REG_FR_LSB					= 0x08U,
	REG_PA_CONFIG				= 0x09U,
	REG_OCP						= 0x0bU,
	REG_LNA						= 0x0cU,
	REG_FIFO_ADDR_PTR			= 0x0dU,
	REG_FIFO_TX_BASE_ADDR		= 0x0eU,
	REG_FIFO_RX_BASE_ADDR		= 0x0fU,
	REG_FIFO_RX_CURRENT_ADDR	= 0x10U,
	REG_IRQ_FLAGS_MASK			= 0x11U,
	REG_IRQ_FLAGS				= 0x12U,
	REG_RX_NB_BYTES				= 0x13U,
	REG_PKT_SNR_VALUE			= 0x19U,
	REG_PKT_RSSI_VALUE			= 0x1aU,
	REG_RSSI_VALUE				= 0x1bU,
	REG_MODEM_CONFIG_1			= 0x1dU,
	REG_MDOEM_CONFIG_2			= 0x1eU,
	REG_SYMB_TIMEOUT_LSB		= 0x1fU,
	REG_PREAMBLE_MSB			= 0x20U,
	REG_PREAMBLE_LSB			= 0x21U,
	REG_PAYLOAD_LENGTH			= 0x22U,
	REG_MAX_PAYLOAD_LENGTH		= 0x23U,
	REG_FIFO_RX_BYTE_ADDR		= 0x25U,
	REG_MODEM_CONFIG_3			= 0x26U,
	REG_IF_FREG_2				= 0x2fU,	// 	if (125, 250MHZ) to 0x40
	REG_DETECT_OPTIMIZE			= 0x31U,	//	Should be set to 0x0 after each reset (POR on manual)
	REG_HIGH_BW_OPTIMIZE_1		= 0x36U,	//	Set to 0x02
	REG_DETECTION_THRESHOLD		= 0x37U,
	REG_SYNC_WORD				= 0x39U,
	REG_HIGH_BW_OPTIMIZE_2		= 0x3aU,	// 	Set to 0x64
	REG_DIO_MAPPING_1			= 0x40U,
	REG_VERSION					= 0x42U,
	REG_PA_DAC					= 0x4dU,
};

/* Mode */
typedef enum {
	MODE_SLEEP               	= 0x00U,
	MODE_STDBY               	= 0x01U,	// (Default)
	MODE_TX                  	= 0x03U,	// Transmit
	MODE_RXCONTINUOUS       	= 0x05U,	// Receive continuous
	MODE_RXSINGLE           	= 0x06U,	// Receive single
	MODE_CAD					= 0x07U,	// Channel activity detection
} RFM95W_Mode;

/* PaSelect */
typedef enum {
	RFO_PIN						= 0x00U,	// Output power is limited to +14 dBm (Default)
	PA_BOOST_PIN				= 0x80U, 	// Output power is limited to +20 dBm
} RFM95W_PaSelect;

/* LnaGain */
typedef enum {
	lNA_GAIN_G1					= 0x20U,	// Maximum gain (Default)
	lNA_GAIN_G2					= 0x40U,
	lNA_GAIN_G3					= 0x60U,
	lNA_GAIN_G4					= 0x80U,
	lNA_GAIN_G5					= 0xa0U,
	lNA_GAIN_G6					= 0xc0U,	// Minimum gain
} RFM95W_LnaGain;

/* LnaBoostHf */
typedef enum {
	LNA_BOOST_HF_DEFAULT		= 0x00U,	// Default LNA current (Default)
	LNA_BOOST_HF_150			= 0x03U, 	// Boost on, 150% LNA current
} RFM95W_LnaBoostHf;

/* Bw Signal bandwidth: */
typedef enum {
	BW_125_KHZ					= 0x70U,	//	(Default)
	BW_250_KHZ					= 0x80U,
	BW_500_KHZ					= 0x90U,
} RFM95W_Bw;

/* CodingRate */
typedef enum {
	CODING_RATE_5				= 0x02U,	// 4/5 (Default)
	CODING_RATE_6				= 0x04U,	// 4/6
	CODING_RATE_7				= 0x06U,	// 4/7
	CODING_RATE_8				= 0x08U,	// 4/8
}	RFM95W_Coding_Rate;

/* ImplicitHeaderModeOn */
typedef enum {
	EXPLICIT_HEADER_MODE 		= 0x00U,	// (Default)
	IMPLICIT_HEADER_MODE 		= 0x01U,
} RFM95W_ImplicitHeaderModeOn;

/* SpreadingFactor */
typedef enum {
	SPREADING_FACTOR_6 			= 0x60U,	// 64 chips / symbol
	SPREADING_FACTOR_7 			= 0x70U,	// 128 chips / symbol (Default)
	SPREADING_FACTOR_8 			= 0x80U,	// 256 chips / symbol
	SPREADING_FACTOR_9 			= 0x90U,	// 512 chips / symbol
	SPREADING_FACTOR_10 		= 0xa0U,	// 1024 chips / symbol
	SPREADING_FACTOR_11 		= 0xb0U,	// 2048 chips / symbol
	SPREADING_FACTOR_12 		= 0xc0U,	// 4096 chips / symbol
} RFM95W_SpreadingFactor;

/* TxContinuousMode */
typedef enum {
	NORMAL_MODE					= 0x00U,	// a single packet is sent (Default)
	CONTINUOUS_MODE				= 0x08U,	// send multiple packets across the FIFO
} RFM95W_TxContinuousMode;

// IRQ masks
typedef enum {
	IRQ_TX_DONE_MASK           	= 0x08U,
	IRQ_PAYLOAD_CRC_ERROR_MASK 	= 0x20U,
	IRQ_RX_DONE_MASK           	= 0x40U,
} RFM95W_IrqMasks;

/* Mode Transmit in LoRa Mesh */
enum {
	SEND_MODE					= 0x00U,
	FIND_MODE					= 0x01U,
};

/* Interupt Pin Dio0 */
enum {
	RX_PIN						= 0x00U,
	TX_PIN						= 0x40U,
};

/* Function Stage */
enum {
	TRANSMIT_STAGE_RFM95W		= 0x01U,
	TRANSMITING_STAGE_RFM95W	= 0xfeU,
	MAX_STAGE_RFM95W			= 0xffU,
} ;

// PA config
#define PA_BOOST                 0x80

#define MAX_PKT_LENGTH           255

class LoRaClass {

	SPI_HandleTypeDef* hspi;

	RTC_HandleTypeDef* hrtc;

	GPIO_TypeDef  *GPIO_NSS_GPIO_Port;
	uint16_t NSS_Pin;

	GPIO_TypeDef  *GPIO_CONTROL_POWER_GPIO_Port;
	uint16_t CONTROL_POWER_Pin;

	GPIO_TypeDef  *GPIO_KEEP_POWER_GPIO_Port;
	uint16_t KEEP_POWER_Pin;

	uint8_t stage;
	uint8_t pTxData[15];
	uint8_t pRxData[10];

	uint8_t pData;
	uint16_t volData;

	RFM95W_IrqMasks irqMode;

public:

	LoRaClass(	SPI_HandleTypeDef* hspi,
			RTC_HandleTypeDef* hrtc,
			GPIO_TypeDef  *GPIO_NSS_GPIO_Port,
			uint16_t NSS_Pin,
			GPIO_TypeDef  *GPIO_CONTROL_POWER_GPIO_Port,
			uint16_t CONTROL_POWER_Pin,
			GPIO_TypeDef  *GPIO_KEEP_POWER_GPIO_Port,
			uint16_t KEEP_POWER_Pin
	);
	~LoRaClass();

	void run(uint32_t frequency,
				RFM95W_Bw bw = BW_125_KHZ,
				RFM95W_SpreadingFactor spreadingFactor = SPREADING_FACTOR_12
		);
	void run_DMA(uint32_t frequency,
			RFM95W_Bw bw = BW_125_KHZ,
			RFM95W_SpreadingFactor spreadingFactor = SPREADING_FACTOR_7
	);

	/* DMA_Callback	 */
	void txRxCpltCallback();
	void irqDoneCallback();

	/* ------ Function	------- */
	void setStage(uint8_t func);
	void setData(uint8_t pData);
	void setVolData(uint16_t volData);

	uint8_t getStage();

	void clearFlags();

	uint8_t checkCrcXOR(uint8_t* data, uint8_t size);

};

#endif /* RFM95W_H_ */
