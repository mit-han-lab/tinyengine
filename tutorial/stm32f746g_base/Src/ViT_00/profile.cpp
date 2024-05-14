#include "profile.h"

void printLog(const char *s) {
	static int is_initialized = 0;
	if (!is_initialized) {
		UART.Instance = USART1;
		UART.Init.BaudRate = 115200;
		UART.Init.WordLength = UART_WORDLENGTH_8B;
		UART.Init.StopBits = UART_STOPBITS_1;
		UART.Init.Parity = UART_PARITY_NONE;
		UART.Init.Mode = UART_MODE_TX_RX;
		UART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UART.Init.OverSampling = UART_OVERSAMPLING_16;
		UART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		UART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&UART) != HAL_OK) {
			//Error handling
		}
		is_initialized = 1;
	}
	HAL_UART_Transmit(&UART, (uint8_t*) s, strlen(s), 10);
//	HAL_UART_Transmit(&UART, (uint8_t*) "\n\r", 2, 10);


}

void recieveChar(char *s) {
	static int is_initialized = 0;
	if (!is_initialized) {
		UART.Instance = USART1;
		UART.Init.BaudRate = 115200;
		UART.Init.WordLength = UART_WORDLENGTH_8B;
		UART.Init.StopBits = UART_STOPBITS_1;
		UART.Init.Parity = UART_PARITY_NONE;
		UART.Init.Mode = UART_MODE_TX_RX;
		UART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UART.Init.OverSampling = UART_OVERSAMPLING_16;
		UART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		UART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&UART) != HAL_OK) {
			//Error handling
		}
		is_initialized = 1;
	}
	HAL_UART_Receive(&UART, (uint8_t*) s, 1, 10);
}
