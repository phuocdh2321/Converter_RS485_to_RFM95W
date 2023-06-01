/*
 * terminor.h
 *
 *  Created on: Apr 27, 2023
 *      Author: ADMIN
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "stm32f1xx_hal.h"
#include <string>
#include <deque>


class Terminal {

	UART_HandleTypeDef* huart;

	std::string data = "aaaa";

	std::deque<std::string> listData;

	uint8_t stage;
	uint8_t flagTxDone;
public:
	Terminal(UART_HandleTypeDef* huart);
	~Terminal();

	void setListData(const std::string &data);
	void run();
	void TxCpltCallback(UART_HandleTypeDef *huart);
	HAL_StatusTypeDef emptyListData();
};



#endif /* TERMINAL_H_ */
