/*
 * uart_processing.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Dell
 */


#include "uart_processing.h"

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t in_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t buffer_count = 0;

uint8_t cmd_rec[MAX_BUFFER_SIZE];
uint8_t cmd_proc[MAX_BUFFER_SIZE];
uint8_t res_frame[MAX_BUFFER_SIZE];
uint8_t res_size = 0;
uint8_t cmd_idx = 0;

enum FrameReceive {
	IDLE,
	BUFFER_RECORD
};

enum FrameReceive cmd_parser = IDLE;

enum CmdProcessing {
	WAIT_REQ,
	DATA_TRANSMIT,
	WAIT_RES
};

enum CmdProcessing cmd_state = IDLE;

void command_parser_fsm() {
	switch (cmd_parser) {
		case IDLE:
			if (temp == '!') {
				cmd_parser = BUFFER_RECORD;
				cmd_idx = 0;
				memset(cmd_rec, 0, MAX_BUFFER_SIZE);
			}
			break;
		case BUFFER_RECORD:
			if (temp == '#') {
				cmd_parser = IDLE;
				memcpy(cmd_proc, cmd_rec, MAX_BUFFER_SIZE);
			} else {
				cmd_rec[cmd_idx++] = temp;
			}
			break;
		default:
			break;
	}
}

void uart_communiation_fsm() {
	switch (cmd_state) {
		case WAIT_REQ:
			if (strcmp((char *)cmd_proc, "RTS") == 0) {
				cmd_state = DATA_TRANSMIT;
				res_size = sprintf((char *)res_frame, "!ADC=%d#", (int) HAL_ADC_GetValue(&hadc1));
			}
			break;
		case DATA_TRANSMIT:
			HAL_UART_Transmit(&huart2, res_frame, sizeof(res_frame), 1000);
			cmd_state = WAIT_RES;
			setTimer0(3000);
			break;
		case WAIT_RES:
			if (strcmp((char *)cmd_proc, "OK") == 0) {
				cmd_state = WAIT_REQ;
				memset(cmd_proc, 0, MAX_BUFFER_SIZE);
			}
			if (getTimer0Flag()) {
				cmd_state = DATA_TRANSMIT;
			}
			break;
		default:
			break;
	}
}
