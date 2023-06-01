/*
 * terminor.cpp
 *
 *  Created on: Apr 27, 2023
 *      Author: ADMIN
 */
#include "terminal.h"

Terminal::Terminal(UART_HandleTypeDef* huart){
	this->huart = huart;

	stage = 0;
	flagTxDone = HAL_OK;
}

Terminal::~Terminal(){

}

void Terminal::run() {
	if (stage == 0){
		if (listData.size() < 1) return;
		HAL_UART_Transmit_DMA(huart, (uint8_t* )listData[0].data(), listData[0].size());
		flagTxDone = HAL_BUSY;
		stage++;
	}
	if (stage == 1) {
		if (flagTxDone != HAL_OK) return;
		listData.pop_front();
		stage = 0;
	}
}

void Terminal::setListData(const std::string &data){
	listData.push_back(data);
}

void Terminal::TxCpltCallback(UART_HandleTypeDef *huart) {
	if (this->huart->Instance == huart->Instance) {
		flagTxDone = HAL_OK;
	}
}

HAL_StatusTypeDef Terminal::emptyListData(){
	if (listData.size() < 1) return HAL_OK;
	return HAL_ERROR;
}






