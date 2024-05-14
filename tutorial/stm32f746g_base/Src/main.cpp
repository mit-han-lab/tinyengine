/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   main.cpp
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#include "main.h"
//#include "camera.h"
#include "lcd.h"


#include <stdio.h>
//#include <errno.h>
//#include <sys/unistd.h>
//#include "stm32746g_discovery.h"
#include "stm32f7xx_hal.h"

#include "string.h"
#include "testing_data/golden_data.h"
#include "testing_data/images.h"
extern "C" {
//#include "genNN.h"
//#include "tinyengine_function.h"
}
// #define TESTTENSOR
#define UseCamera 0  // 1 for using Arducam; 0 for not using Arducam.
#define NoCamera_Person 0  // 1 for person; 0 for non-person. (Only applicable when UseCamera == 0) 

#include "stm32746g_discovery.h"

//extern "C" {
//#include "GAP9MHSA/inc/undefined.h"
//}

//#include "ViT_00/vit.h"
#include "ViT_00/profile.h"

#include "model_data_uncompressed.h"
#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/version.h"
#include "schema_generated.h"

//#include "mnist_schema_generated.h"

#define TENSOR_ARENA_SIZE 262144
struct tflite_model {
    const tflite::Model* model;
    tflite::ErrorReporter* error_reporter;
    tflite::MicroInterpreter* interpreter;
    TfLiteTensor* input;
    TfLiteTensor* output;
    int inference_count;
    uint8_t tensor_arena[TENSOR_ARENA_SIZE];
};
struct tflite_model tf;
void ViewModel(struct tflite_model *tf);

signed char* getInput();
signed char* getOutput();
static signed char buffer[93992];

static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void MX_GPIO_Init(void);

#define RES_W 80
#define RES_H 80
#define OUTPUT_CH 2

void SystemClock_Config(void);
void StartDefaultTask(void const *argument);

signed char out_int[OUTPUT_CH];

float labels[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

void invoke_new_weights_givenimg(signed char *out_int8) {
//	cluster_fork(out_int8);


	signed char *output = (signed char *)getOutput();
	for (int i = 0; i < OUTPUT_CH; i++)
		out_int8[i] = output[i];
}


void ViewModel(struct tflite_model *tf)
{
    TfLiteTensor *input = tf->interpreter->input(0);
    TfLiteTensor *output = tf->interpreter->output(0);
    char buf[200] = {0,};
    sprintf(buf, "Model input:\n");
    printLog(buf);

    sprintf(buf, "dims->size: %d\n", input->dims->size);
    printLog(buf);

    sprintf(buf, "dims->data[0]: %d\n", input->dims->data[0]);
    printLog(buf);

    sprintf("dims->data[1]: %d\n", input->dims->data[1]);
    printLog(buf);

    sprintf("dims->data[2]: %d\n", input->dims->data[2]);
    printLog(buf);

    sprintf("dims->data[3]: %d\n", input->dims->data[3]);
    printLog(buf);

    sprintf("input->type: %d\n\n", input->type);
    printLog(buf);

    sprintf("Model output:\n");
    printLog(buf);

    sprintf("dims->size: %d\n", output->dims->size);
    printLog(buf);

    sprintf("dims->data[0]: %d\n", output->dims->data[0]);
    printLog(buf);

    sprintf("dims->data[1]: %d\n\n", output->dims->data[1]);
    printLog(buf);
}

void RunInference(struct tflite_model *tf, float *data, size_t data_size, uint8_t debug)
{
    // // Obtain pointers to the model's input and output tensors
    TfLiteTensor *input = tf->interpreter->input(0);
    TfLiteTensor *output = tf->interpreter->output(0);

    /* Copy data to the input buffer. So much wasted RAM! */
    for (size_t i = 0; i < data_size; i++) {
        input->data.f[i] = data[i];
    }

    if (debug) printLog(("Running inference...\n"));

    uint32_t ints = disableInts();
    glb_inference_time_ms = 0;
    // Run the model on this input and make sure it succeeds.
    TfLiteStatus invoke_status = tf->interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
    	printLog("Invoke failed\n\r");
//        tf->error_reporter->Report("Invoke failed\n");
    }
    restoreInts(ints);

	flatbuffers::FlatBufferBuilder fbb;
    auto out_vect = fbb.CreateVector((float*) output->data.f, 10);
    auto output_f = MnistProt::CreateInferenceOutput(fbb, out_vect, 0, glb_inference_time_ms);

    MnistProt::CommandsBuilder builder(fbb);
    builder.add_cmd(MnistProt::Command_CMD_INFERENCE_OUTPUT);
    builder.add_ouput(output_f);
    auto resp = builder.Finish();
    fbb.Finish(resp);

    uint8_t *buf = fbb.GetBufferPointer();
    int buf_size = fbb.GetSize();

    printLog(buf);

//    HAL_UART_Transmit(&huart7, (uint8_t *)buf, buf_size, 10);

//    char buf[200];
    if (debug) {
//        sprintf(buf, ("Done in %f msec...\n\r", glb_inference_time_ms));
//        printLog(buf);
//        for (size_t i = 0; i < 10; i++) {
//            TRACE(("Out[%d]: %f\n", i, output->data.f[i]));
//        }
    }
}
void tensorflow_lite(){
	tf.model = tflite::GetModel(jupyter_notebook_mnist_tflite);
	if (tf.model->version() != TFLITE_SCHEMA_VERSION) {
		printLog("TFLITE_SCHEMA_VERSION \n\r");
	}

	static tflite::ops::micro::AllOpsResolver resolver;
	tf.interpreter = new tflite::MicroInterpreter(tf.model, resolver, tf.tensor_arena, TENSOR_ARENA_SIZE, tf.error_reporter);

	// Allocate memory from the tensor_arena for the model's tensors.
	TfLiteStatus allocate_status = tf.interpreter->AllocateTensors();
	if (allocate_status != kTfLiteOk) {
		printLog("AllocateTensor() failed \n\r");
	}
	tf.inference_count = 0;

	ViewModel(&tf);

	RunInference(&tf, (float *)digit, 784, 1);
}

//void invoke_new_weights_givenimg(signed char *out_int8) {
//  invoke(labels);
//  signed char *output = (signed char *)getOutput();
//  for (int i = 0; i < OUTPUT_CH; i++)
//    out_int8[i] = output[i];
//}


//void print(const char *s) {
//
//	HAL_UART_Transmit(&UART, (uint8_t*) s, strlen(s), 10);
//}


#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_10
#define BUTTON2_GPIO_Port GPIOF

uint16_t *RGBbuf;

//SPI_HandleTypeDef flash_conf;

int main(void) {
//  char buf[150];
  char showbuf[150];

  printf("test\n");

  CPU_CACHE_Enable();
  HAL_Init();
//  UART.Instance = USART1;
//  UART.Init.BaudRate = 115200;
//  UART.Init.WordLength = UART_WORDLENGTH_8B;
//  UART.Init.StopBits = UART_STOPBITS_1;
//  UART.Init.Parity = UART_PARITY_NONE;
//  UART.Init.Mode = UART_MODE_TX_RX;
//  UART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  UART.Init.OverSampling = UART_OVERSAMPLING_16;
//  UART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//  UART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//  if (HAL_UART_Init(&UART) != HAL_OK) {
//	  //Error handling
//  }

//
//  // Configure the SPI
//  flash_conf.Instance = SPI1;
//  flash_conf.Init.Mode = SPI_MODE_MASTER;
//  flash_conf.Init.Direction = SPI_DIRECTION_2LINES;
//  flash_conf.Init.DataSize = SPI_DATASIZE_8BIT;
//  flash_conf.Init.CLKPolarity = SPI_POLARITY_LOW;
//  flash_conf.Init.CLKPhase = SPI_PHASE_1EDGE;
//  flash_conf.Init.NSS = SPI_NSS_SOFT;
//  flash_conf.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
//  flash_conf.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  flash_conf.Init.TIMode = SPI_TIMODE_DISABLE;
//  flash_conf.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  flash_conf.Init.CRCPolynomial = 7;
//
//  if (HAL_SPI_Init(&flash_conf) != HAL_OK)
//  {
//	  // Initialization Error
//	  while(1);
//  }

  SystemClock_Config();

  MX_GPIO_Init();

  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  lcdsetup();

#ifdef TESTTENSOR
  {
    uint32_t start, end;
    setRGBTestImage();
    start = HAL_GetTick();
    invoke(labels);
    end = HAL_GetTick();
    uint8_t *output = (uint8_t *)getOutput();
    int i;
    for (i = 0; i < 100; i++) {
      sprintf(buf, "%d,", output[i]);
      printLog(buf);
    }
    printLog("\r\n");
    return 0;
  }
#endif

#if UseCamera
  int camErr = initCamera();

  uint32_t start, end;
  StartCapture();
  signed char *input = getInput();
  RGBbuf = (uint16_t *)&input[80 * 80 * 4];
  int t_mode = 0;

  while (1) {
    start = HAL_GetTick();
    ReadCapture();
    StartCapture();
    DecodeandProcessAndRGB(RES_W, RES_H, input, RGBbuf, 1);
    for (int i = 0; i < RES_W; i++) {
      for (int j = 0; j < RES_W; j++) {
        uint8_t red = (int32_t)input[(80 * i + j) * 3] + 128;
        uint8_t green = (int32_t)input[(80 * i + j) * 3 + 1] + 128;
        uint8_t blue = (int32_t)input[(80 * i + j) * 3 + 2] + 128;

        uint16_t b = (blue >> 3) & 0x1f;
        uint16_t g = ((green >> 2) & 0x3f) << 5;
        uint16_t r = ((red >> 3) & 0x1f) << 11;

        RGBbuf[j + RES_W * i] = (uint16_t)(r | g | b);
      }
    }
    loadRGB565LCD(10, 10, RES_W, RES_W, RGBbuf, 3);

  	invoke_new_weights_givenimg(out_int);
  	int person = 0;
  	if (out_int[0] > out_int[1]) {
  	  person = 0;
  	}
  	else {
  	  person = 1;
  	}
  	end = HAL_GetTick();
  	sprintf(showbuf, " Inference ");
  	displaystring(showbuf, 273, 10);
  	detectResponse(person, end - start, t_mode, 0, 0);
  }

#else
  uint32_t start, end;
  signed char *input = buffer;
  for (int i = 0; i < RES_W * RES_W * 3; i++) {
#if NoCamera_Person
    input[i] = person[i];  // Image of person
#else
    input[i] = no_person[i];  // Image of non-person
#endif
  }
  RGBbuf = (uint16_t *)&input[80 * 80 * 4];
  int t_mode = 0;

//  submain();

  start = HAL_GetTick();

  for (int i = 0; i < RES_W; i++) {
    for (int j = 0; j < RES_W; j++) {
      uint8_t red = (int32_t)input[(80 * i + j) * 3] + 128;
      uint8_t green = (int32_t)input[(80 * i + j) * 3 + 1] + 128;
      uint8_t blue = (int32_t)input[(80 * i + j) * 3 + 2] + 128;

      uint16_t b = (blue >> 3) & 0x1f;
      uint16_t g = ((green >> 2) & 0x3f) << 5;
      uint16_t r = ((red >> 3) & 0x1f) << 11;

      RGBbuf[j + RES_W * i] = (uint16_t)(r | g | b);
    }
  }
  loadRGB565LCD(10, 10, RES_W, RES_W, RGBbuf, 3);

//  main_vision_transformer(RGBbuf);
  tensorflow_lite();
//	invoke_new_weights_givenimg(out_int);
	int person = 0;
	if (out_int[0] > out_int[1]) {
	  person = 0;
	}
	else {
	  person = 1;
	}

	end = HAL_GetTick();
	sprintf(showbuf, " Inference ");
	printLog("Inference \n\r");
	displaystring(showbuf, 273, 10);
	detectResponse(person, end - start, t_mode, 0, 0);

#endif
}

void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  ret = HAL_PWREx_EnableOverDrive();
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }
}
static void Error_Handler(void) {

  BSP_LED_On(LED1);
  while (1) {
  }
}

static void CPU_CACHE_Enable(void) {

  SCB_EnableICache();

  SCB_EnableDCache();
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin,
                    GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOI, ARDUINO_D7_Pin | ARDUINO_D8_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOG, ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin,
                    GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = OTG_HS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_HS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_TXD1_Pin | RMII_TXD0_Pin | RMII_TX_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_D7_Pin | ULPI_D6_Pin | ULPI_D5_Pin | ULPI_D3_Pin |
                        ULPI_D2_Pin | ULPI_D1_Pin | ULPI_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPDIF_RX0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF8_SPDIFRX;
  HAL_GPIO_Init(SPDIF_RX0_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Audio_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Audio_INT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_P_Pin | OTG_FS_N_Pin | OTG_FS_ID_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_D7_Pin | ARDUINO_D8_Pin | LCD_DISP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = uSD_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TP3_Pin | NC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_SCK_D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(ARDUINO_SCK_D13_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DCMI_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DCMI_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LCD_INT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_NXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_STP_Pin | ULPI_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_RXER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RMII_RXER_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_CLK_Pin | ULPI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_MISO_D12_Pin | ARDUINO_MOSI_PWM_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON2_GPIO_Port, &GPIO_InitStruct);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {

  while (1) {
  }
}
#endif
