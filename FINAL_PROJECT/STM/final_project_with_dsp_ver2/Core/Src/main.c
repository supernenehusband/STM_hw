/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "app_bluenrg_ms.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l475e_iot01_accelero.h"
#include "bluenrg_def.h"
#include "gatt_db.h"
#include "bluenrg_conf.h"
#include "bluenrg_gatt_aci.h"

#include <stdio.h>
#include "arm_math.h"


//#include "bluenrg_gap_aci.h"
////#include "link_layer.h"s
//#include "hci_const.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint32_t g_sample_rate; // BLE 寫入後會改變它
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ACC_SERVICE_UUID 0xAA
#define ACC_CHAR_UUID 0x01
#define SAMPLEFREQ_CHAR_UUID 0x02

#define x_threshold 750
#define y_threshold 750
#define z_threshold 969

#define horizontal_cooldown 350
#define vertical_cooldown 3000

#define g 1000

#define true 1;
#define false 0;




/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;

I2C_HandleTypeDef hi2c2;

QSPI_HandleTypeDef hqspi;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

osThreadId defaultTaskHandle;
osThreadId myTaskBLEHandle;

uint32_t myTaskBLEBuffer[512];
osStaticThreadDef_t myTaskBLEControlBlock;
osThreadId myTaskACCHandle;
uint32_t myTaskACCBuffer[512];
osStaticThreadDef_t myTaskACCControlBlock;


/* USER CODE BEGIN PV */

osThreadId myTaskShakeHandle;
uint32_t myTaskShakeBuffer[512];
osStaticThreadDef_t myTaskShakeControlBlock;




uint16_t AccServiceHandle, AccCharHandle, SampleFreqCharHandle;
int16_t pDataXYZ[3];

int8_t state = 0;


osMessageQDef(CommandQueue, 8, uint32_t);  // holds pointers
osMessageQId CommandQueueHandle;


#define BLOCK_SIZE 1
#define NUM_TAPS 5
// FIR coefficients: simple low-pass filter (moving average)
float32_t firCoeffs[NUM_TAPS] = {
    0.2f, 0.2f, 0.2f, 0.2f, 0.2f
};
//float32_t firCoeffs[NUM_TAPS] = {
//    -0.0018225232, -0.0027371416, -0.0046716008, -0.0076184935, -0.0115314465,
//    -0.0163239892, -0.0218712004, -0.0279990343, -0.0344866470, -0.0410715196,
//    -0.0474555236, -0.0533131213, -0.0583097116, -0.0621234862, -0.0644630880,
//    -0.0650860965, -0.0638142520, -0.0605477021, -0.0552754138, -0.0480797570,
//    -0.0391343940, -0.0287079029, -0.0171598410, -0.0049350158,  0.0075142664,
//     0.0196265448,  0.0308437035,  0.0406243802,  0.0484686921,  0.0539379475,
//     0.0566783683,  0.0564430203
//};




// FIR filter instances for X, Y, Z
arm_fir_instance_f32 Sx, Sy, Sz;

// State buffers (must be size NUM_TAPS + BLOCK_SIZE - 1)
float32_t firStateX[NUM_TAPS + BLOCK_SIZE - 1];
float32_t firStateY[NUM_TAPS + BLOCK_SIZE - 1];
float32_t firStateZ[NUM_TAPS + BLOCK_SIZE - 1];

// Input/output buffers
float32_t inputX, inputY, inputZ;
float32_t outputX, outputY, outputZ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DFSDM1_Init(void);
static void MX_I2C2_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
void StartDefaultTask(void const *argument);
void StartTaskBLE(void const *argument);
void StartTaskACC(void const *argument);
void StartTaskShake(void const *argument);

/* USER CODE BEGIN PFP */
// void Add_Acc_Service(void)
//{
//	uint8_t serviceUUID = ACC_SERVICE_UUID;
//	uint8_t accCharUUID = ACC_CHAR_UUID;
//	uint8_t freqCharUUID = SAMPLEFREQ_CHAR_UUID;
//     // Add a service
//     aci_gatt_add_serv(UUID_TYPE_16, &serviceUUID, PRIMARY_SERVICE, 7, &AccServiceHandle);
//
//     // Add Characteristic A: 3-axis acceleration values
//     aci_gatt_add_char(AccServiceHandle,
//                       UUID_TYPE_16, &accCharUUID,
//                       6, // 3 * int16_t = 6 bytes
//                       CHAR_PROP_NOTIFY | CHAR_PROP_READ,
//                       ATTR_PERMISSION_NONE,
//                       GATT_NOTIFY_ATTRIBUTE_WRITE,
//                       16, 1,
//                       &AccCharHandle);
//
//     // Add Characteristic B: Sampling frequency
//     aci_gatt_add_char(AccServiceHandle,
//                       UUID_TYPE_16, &freqCharUUID,
//                       4, // uint32_t: 4 bytes
//                       CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP,
//                       ATTR_PERMISSION_NONE,
//                       GATT_NOTIFY_ATTRIBUTE_WRITE,
//                       16, 1,
//                       &SampleFreqCharHandle);
// }
void Add_Acc_Service(void)
{
//	tBleStatus ret;
  uint8_t uuid128_base[16] = {
      0xd5, 0xa5, 0x02, 0x00, 0x36, 0xac, 0xe1, 0x11,
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  // UUID for service: 0000AA00-0000-0001-11e1-ac360002a5d5
  uuid128_base[12] = 0x00;
  uuid128_base[13] = 0xAA;
  aci_gatt_add_serv(UUID_TYPE_128, uuid128_base, PRIMARY_SERVICE, 7, &AccServiceHandle);\
//  if (ret != BLE_STATUS_SUCCESS) goto fail;

  // UUID for ACC characteristic: 00000100-0000-0001-11e1-ac360002a5d5
  uuid128_base[12] = 0x00;
  uuid128_base[13] = 0x01;
  aci_gatt_add_char(
      AccServiceHandle,
      UUID_TYPE_128, uuid128_base,
      6, // 3 * int16_t = 6 bytes
      CHAR_PROP_NOTIFY | CHAR_PROP_READ,
      ATTR_PERMISSION_NONE,
      GATT_NOTIFY_ATTRIBUTE_WRITE,
      16, 1,
      &AccCharHandle);

  //    // UUID for sampling rate characteristic: 00000101-0000-0001-11e1-ac360002a5d5
  //    uuid128_base[13] = 0x02;  // change to 0x02 for 0x0101
  //    aci_gatt_add_char(
  //        AccServiceHandle,
  //        UUID_TYPE_128, uuid128_base,
  //        4, // uint32_t: 4 bytes
  //        CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP,
  //        ATTR_PERMISSION_NONE,
  //        GATT_NOTIFY_ATTRIBUTE_WRITE,
  //        16, 1,
  //        &SampleFreqCharHandle);
  uuid128_base[12] = 0x01;
  uuid128_base[13] = 0x01;
  aci_gatt_add_char(
      AccServiceHandle,
      UUID_TYPE_128, uuid128_base,
      4,
      CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP,
      ATTR_PERMISSION_NONE,
      GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP,
      16, 1,
      &SampleFreqCharHandle);

//  uuid128_base[12] = 0x00;
//  uuid128_base[13] = 0x00;
//  printf("Add service success.\n");


//  char *device_name = "NTURun";
//  ret = aci_gap_set_discoverable(ADV_IND, 0x0020, 0x0040, PUBLIC_ADDR, NO_WHITE_LIST_USE,sizeof(device_name)-1 ,device_name, sizeof(uuid128_base)-1, uuid128_base, 0, 0);
//  if (ret != BLE_STATUS_SUCCESS) printf("Error while set discoverable.\n");

//  return BLE_STATUS_SUCCESS;
//// fail:
//   printf("Error while adding LED service.\n");
//   return BLE_STATUS_ERROR ;

}

void Update_Accel_Char(int16_t *pDataXYZ)
{
  uint8_t notification[6];
  notification[0] = pDataXYZ[0] & 0xFF;
  notification[1] = (pDataXYZ[0] >> 8) & 0xFF;
  notification[2] = pDataXYZ[1] & 0xFF;
  notification[3] = (pDataXYZ[1] >> 8) & 0xFF;
  notification[4] = pDataXYZ[2] & 0xFF;
  notification[5] = (pDataXYZ[2] >> 8) & 0xFF;

  aci_gatt_update_char_value(AccServiceHandle, AccCharHandle, 0, 6, notification);
}

void Send_Action(char action[], uint8_t len)
{
  //	return;
  aci_gatt_update_char_value(AccServiceHandle, AccCharHandle, 0, len, action);
}

void Attribute_Modified_CB(uint16_t handle, uint8_t data_length, uint8_t *att_data)
{
  if (handle == SampleFreqCharHandle + 1) // +1 to reach the value handle
  {
    g_sample_rate = (uint32_t)(att_data[0] | (att_data[1] << 8) | (att_data[2] << 16) | (att_data[3] << 24));

    if (g_sample_rate == 0)
      g_sample_rate = 1; // avoid divide-by-zero later
    printf("[BLE] Sample rate updated to: %lu\n", g_sample_rate);
  }
}

//
//void Start_Advertising() {
//    tBleStatus ret;
//    const char local_name[] = "BLEDev";
//
//    ret = aci_gap_set_discoverable(
//        ADV_IND, 0x0020, 0x0040,
//        PUBLIC_ADDR, NO_WHITE_LIST_USE,
//        sizeof(local_name) - 1, (uint8_t*)local_name,
//        0, NULL, 0x0006, 0x000C
//    );
//
//    if (ret != BLE_STATUS_SUCCESS) {
//        printf("Failed to start advertising: 0x%02X\n", ret);
//    } else {
//        printf("Advertising started.\n");
//    }
//}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DFSDM1_Init();
  // MX_I2C2_Init();
  MX_QUADSPI_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_BlueNRG_MS_Init();
  /* USER CODE BEGIN 2 */
  BSP_ACCELERO_Init();
  Add_Acc_Service();

//  state = 0;
//  tBleStatus ret;
//  ret = Add_Acc_Service();
//
//  if (ret == BLE_STATUS_SUCCESS){
//	  printf("main success\n");
//  }
//  Start_Advertising();
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  /*osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);*/

  /* definition and creation of myTaskBLE */
  osThreadStaticDef(myTaskBLE, StartTaskBLE, osPriorityNormal, 0, 512, myTaskBLEBuffer, &myTaskBLEControlBlock);
  myTaskBLEHandle = osThreadCreate(osThread(myTaskBLE), NULL);

  /* definition and creation of myTaskACC */
  osThreadStaticDef(myTaskACC, StartTaskACC, osPriorityNormal, 0, 512, myTaskACCBuffer, &myTaskACCControlBlock);
  myTaskACCHandle = osThreadCreate(osThread(myTaskACC), NULL);


  CommandQueueHandle = osMessageCreate(osMessageQ(CommandQueue), NULL);


//  osThreadStaticDef(myTaskShake, StartTaskShake, osPriorityNormal, 0, 512, myTaskShakeBuffer, &myTaskShakeControlBlock);
//   myTaskShakeHandle = osThreadCreate(osThread(myTaskShake), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
   */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
   */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief DFSDM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DFSDM1_Init(void)
{

  /* USER CODE BEGIN DFSDM1_Init 0 */

  /* USER CODE END DFSDM1_Init 0 */

  /* USER CODE BEGIN DFSDM1_Init 1 */

  /* USER CODE END DFSDM1_Init 1 */
  hdfsdm1_channel1.Instance = DFSDM1_Channel1;
  hdfsdm1_channel1.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel1.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel1.Init.OutputClock.Divider = 2;
  hdfsdm1_channel1.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel1.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel1.Init.Input.Pins = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel1.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel1.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel1.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel1.Init.Awd.Oversampling = 1;
  hdfsdm1_channel1.Init.Offset = 0;
  hdfsdm1_channel1.Init.RightBitShift = 0x00;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DFSDM1_Init 2 */

  /* USER CODE END DFSDM1_Init 2 */
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00000E14;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
   */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
   */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */
}

/**
 * @brief QUADSPI Initialization Function
 * @param None
 * @retval None
 */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 2;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 23;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
}

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.battery_charging_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, M24SR64_Y_RF_DISABLE_Pin | M24SR64_Y_GPO_Pin | ISM43362_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ARD_D10_Pin | SPBTLE_RF_RST_Pin | ARD_D9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ARD_D8_Pin | ISM43362_BOOT0_Pin | ISM43362_WAKEUP_Pin | SPSGRF_915_SDN_Pin | ARD_D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, USB_OTG_FS_PWR_EN_Pin | PMOD_RESET_Pin | STSAFE_A100_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPBTLE_RF_SPI3_CSN_GPIO_Port, SPBTLE_RF_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, VL53L0X_XSHUT_Pin | LED3_WIFI__LED4_BLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPSGRF_915_SPI3_CSN_GPIO_Port, SPSGRF_915_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : M24SR64_Y_RF_DISABLE_Pin M24SR64_Y_GPO_Pin ISM43362_RST_Pin ISM43362_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = M24SR64_Y_RF_DISABLE_Pin | M24SR64_Y_GPO_Pin | ISM43362_RST_Pin | ISM43362_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_OVRCR_EXTI3_Pin SPSGRF_915_GPIO3_EXTI5_Pin SPBTLE_RF_IRQ_EXTI6_Pin ISM43362_DRDY_EXTI1_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_OVRCR_EXTI3_Pin | SPSGRF_915_GPIO3_EXTI5_Pin | SPBTLE_RF_IRQ_EXTI6_Pin | ISM43362_DRDY_EXTI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_A5_Pin ARD_A4_Pin ARD_A3_Pin ARD_A2_Pin
                           ARD_A1_Pin ARD_A0_Pin */
  GPIO_InitStruct.Pin = ARD_A5_Pin | ARD_A4_Pin | ARD_A3_Pin | ARD_A2_Pin | ARD_A1_Pin | ARD_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D1_Pin ARD_D0_Pin */
  GPIO_InitStruct.Pin = ARD_D1_Pin | ARD_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D10_Pin SPBTLE_RF_RST_Pin ARD_D9_Pin */
  GPIO_InitStruct.Pin = ARD_D10_Pin | SPBTLE_RF_RST_Pin | ARD_D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D4_Pin */
  GPIO_InitStruct.Pin = ARD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(ARD_D4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D7_Pin */
  GPIO_InitStruct.Pin = ARD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D7_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D13_Pin ARD_D12_Pin ARD_D11_Pin */
  GPIO_InitStruct.Pin = ARD_D13_Pin | ARD_D12_Pin | ARD_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D3_Pin */
  GPIO_InitStruct.Pin = ARD_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D6_Pin */
  GPIO_InitStruct.Pin = ARD_D6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ARD_D6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D8_Pin ISM43362_BOOT0_Pin ISM43362_WAKEUP_Pin SPSGRF_915_SDN_Pin
                           ARD_D5_Pin SPSGRF_915_SPI3_CSN_Pin */
  GPIO_InitStruct.Pin = ARD_D8_Pin | ISM43362_BOOT0_Pin | ISM43362_WAKEUP_Pin | SPSGRF_915_SDN_Pin | ARD_D5_Pin | SPSGRF_915_SPI3_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LPS22HB_INT_DRDY_EXTI0_Pin LSM6DSL_INT1_EXTI11_Pin ARD_D2_Pin HTS221_DRDY_EXTI15_Pin
                           PMOD_IRQ_EXTI12_Pin */
  GPIO_InitStruct.Pin = LPS22HB_INT_DRDY_EXTI0_Pin | LSM6DSL_INT1_EXTI11_Pin | ARD_D2_Pin | HTS221_DRDY_EXTI15_Pin | PMOD_IRQ_EXTI12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_PWR_EN_Pin SPBTLE_RF_SPI3_CSN_Pin PMOD_RESET_Pin STSAFE_A100_RESET_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_PWR_EN_Pin | SPBTLE_RF_SPI3_CSN_Pin | PMOD_RESET_Pin | STSAFE_A100_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : VL53L0X_XSHUT_Pin LED3_WIFI__LED4_BLE_Pin */
  GPIO_InitStruct.Pin = VL53L0X_XSHUT_Pin | LED3_WIFI__LED4_BLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : VL53L0X_GPIO1_EXTI7_Pin LSM3MDL_DRDY_EXTI8_Pin */
  GPIO_InitStruct.Pin = VL53L0X_GPIO1_EXTI7_Pin | LSM3MDL_DRDY_EXTI8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PMOD_SPI2_SCK_Pin */
  GPIO_InitStruct.Pin = PMOD_SPI2_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PMOD_SPI2_SCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PMOD_UART2_CTS_Pin PMOD_UART2_RTS_Pin PMOD_UART2_TX_Pin PMOD_UART2_RX_Pin */
  GPIO_InitStruct.Pin = PMOD_UART2_CTS_Pin | PMOD_UART2_RTS_Pin | PMOD_UART2_TX_Pin | PMOD_UART2_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D15_Pin ARD_D14_Pin */
  GPIO_InitStruct.Pin = ARD_D15_Pin | ARD_D14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  //	MX_BlueNRG_MS_Process();
  int16_t pDataXYZ[3]; // <-- Declare the array here
  for (;;)
  {
    osDelay(100);
    BSP_ACCELERO_AccGetXYZ(pDataXYZ);
    printf("x: %d\n", pDataXYZ[0]);
    printf("y: %d\n", pDataXYZ[1]);

    printf("z: %d\n", pDataXYZ[2]);

    //		printf("x: %d, y: %d, z: %d\n", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTaskBLE */
/**
 * @brief Function implementing the myTaskBLE thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskBLE */

char *command;
uint8_t command_len;
void StartTaskBLE(void const *argument)
{
  /* USER CODE BEGIN StartTaskBLE */
//	  tBleStatus ret;
//
//	    	const char local_name[] = "BLEDev";
//
//	    	ret = aci_gap_set_discoverable(
//	    	    ADV_IND,              // Advertising type
//	    	    0x0020,               // Min interval (20ms)
//	    	    0x0040,               // Max interval (40ms)
//	    	    PUBLIC_ADDR,          // Own address type
//	    	    NO_WHITE_LIST_USE,    // Advertising filter policy
//	    	    sizeof(local_name) - 1,   // Local name length (no null terminator)
//	    	    local_name,           // Local name string
//	    	    0, NULL,              // No service UUIDs
//	    	    0x0006,               // Min connection interval (7.5ms)
//	    	    0x000C                // Max connection interval (15ms)
//	    	);
//
//	    	if (ret != BLE_STATUS_SUCCESS) {
//	    	    printf("Failed to start advertising: 0x%02X\n", ret);
//	    	} else {
//	    	    printf("Advertising started.\n");
//	    	}
  /* Infinite loop */
	command = "";
	command_len = 0;
  for (;;)
  {
    MX_BlueNRG_MS_Process();
//    printf("BLE Task for loop \n");
//    osDelay(10); /// small delay
    osEvent evt = osMessageGet(CommandQueueHandle, 10);  // wait up to 10ms
    if (evt.status == osEventMessage) {
        const char* cmd = (const char*)evt.value.v;
        Send_Action(cmd, strlen(cmd));
    }
//    if (command_len != 0){
//    	Send_Action(command, command_len);
//    	command = "";
//    	command_len = 0;
//    }
  }
  /*for(;;)
  {
    MX_BlueNRG_MS_Process();
    int16_t pDataXYZ[3];  // <-- Declare the array here
        for(;;)
        {
          osDelay(100);
        BSP_ACCELERO_AccGetXYZ(pDataXYZ);
        printf("x: %d\n", pDataXYZ[0]);
        printf("y: %d\n", pDataXYZ[1]);

        printf("z: %d\n", pDataXYZ[2]);

    //		printf("x: %d, y: %d, z: %d\n", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);

        }
  }*/
  /* USER CODE END StartTaskBLE */
}

/* USER CODE BEGIN Header_StartTaskACC */
/**
 * @brief Function implementing the myTaskACC thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskACC */

// Format: {b0, b1, b2, a1, a2}



void StartTaskACC(void const *argument)
{
  /* USER CODE BEGIN StartTaskACC */
  const TickType_t ten_seconds = pdMS_TO_TICKS(10000);
  const TickType_t three_seconds = pdMS_TO_TICKS(3000);
  uint32_t shake_count = 0;
  uint8_t shake_state = 0;
  uint32_t start_time, cd_start_time;
  uint8_t cooldown = false;

  const float32_t biquadBPF_Coeffs[5] = {
       0.0675f,  0.0000f, -0.0675f,  -1.8407f,  0.8650f
  };
  float32_t bpfState[4]; // 4 floats per biquad stage
  arm_biquad_casd_df1_inst_f32 bpf;
  arm_biquad_cascade_df1_init_f32(&bpf, 1, biquadBPF_Coeffs, bpfState);



//  arm_fir_init_f32(&Sx, NUM_TAPS, firCoeffs, firStateX, BLOCK_SIZE);
  arm_fir_init_f32(&Sx, NUM_TAPS, firCoeffs, firStateX, BLOCK_SIZE);
  arm_fir_init_f32(&Sy, NUM_TAPS, firCoeffs, firStateY, BLOCK_SIZE);
  arm_fir_init_f32(&Sz, NUM_TAPS, firCoeffs, firStateZ, BLOCK_SIZE);

  float32_t noisy_input, filtered_output;
  float32_t angle = 0.0f;
  float32_t step = 2 * PI / 50;  // ~1Hz sine wave if loop runs at 10Hz
  int counter = 0;

  int16_t x_max = 0;
  int16_t x_min = 0;

  int16_t z_max = 0, z_min = 0;

  int8_t x_state= 0,z_state =0;
  for (;;)
  {
	osDelay(10);
//	float32_t sine = 100.0f * arm_sin_f32(angle);
//	float32_t noise = ((rand() % 200) - 100) * 0.5f;  // ±50 noise
//	noisy_input = sine + noise;
//
//	// FIR filter
//	arm_fir_f32(&Sx, &noisy_input, &filtered_output, BLOCK_SIZE);
//
//	// Print input vs filtered
//	printf("(%6.1f,%6.1f)", counter++, noisy_input, filtered_output);
//
//	angle += step;
//	if (angle > 2 * PI) angle -= 2 * PI;
//
//
//
//	continue;

//    BSP_ACCELERO_AccGetXYZ(pDataXYZ); // Read accelerometer
//                                      //	    printf("I'm here\n");
//
//    int16_t x = pDataXYZ[0];
//    int16_t y = pDataXYZ[1];
//    int16_t z = pDataXYZ[2];

//    char msg[50];  // Create a buffer to hold the formatted string
//    sprintf(msg, "x=%d",x);
//    Send_Action(msg, strlen(msg));
////    Send_Action("fuck", 4);
////    printf(msg);
////    printf("\n");
////    osDelay(10);
//    continue;



    BSP_ACCELERO_AccGetXYZ(pDataXYZ);

	float32_t xout, yout, zout;
//	float32_t zout;




	float32_t xin = (float32_t)pDataXYZ[0];
	float32_t yin = (float32_t)pDataXYZ[1];
	float32_t zin = (float32_t)pDataXYZ[2];

//	printf("pre x=%.2f y=%.2f z=%.2f\r\n", xin,yin,zin);


	arm_fir_f32(&Sx, &xin, &xout, BLOCK_SIZE);
	arm_fir_f32(&Sy, &yin, &yout, BLOCK_SIZE);
	arm_fir_f32(&Sz, &zin, &zout, BLOCK_SIZE);

//	printf("Filtered x=%.2f y=%.2f z=%.2f\r\n", xout, yout, zout);
//
//
//
////	continue;

	//TODO: 決定是否使用dsp

	//不使用DSP:
//	int16_t x = pDataXYZ[0];
//	int16_t y = pDataXYZ[1];
//	int16_t z = pDataXYZ[2];
//	z -= g;

	//使用DSP:

	int16_t x = (int16_t)xout;
	int16_t y = (int16_t)yout;
	int16_t z = (int16_t)zout;
	z -= g;



	//END OF TODO


//	char msg[50];  // Create a buffer to hold the formatted string
//	sprintf(msg, "x=%d",x);
//	Send_Action(msg, strlen(msg));
//	continue;

//	if (x > x_max){
//		x_max = x;
//		printf("x max is now %d\n", x_max);
//
//	}
//	if (x < x_min){
//			x_min = x;
//			printf("x min is now %d\n", x_min);
//	}

//	printf("converted x=%d, y=%d, z=%d\n", x,y,z);
//	continue;

    switch (shake_state){
    case 0:
    	shake_count = 0;
    	if (x > 500){
    		start_time = HAL_GetTick();
    		shake_state = 1;
    		shake_count++;
    	}
    	if (x < -500){
    		start_time = HAL_GetTick();
    		shake_state = 2;
    		shake_count++;
    	}
//    	start_time = sHAL_GetTick();
//    	printf("x=%d\n", x);
    	break;
    case 1:
    	if (x < -500){
//			start_time = HAL_GetTick();
			shake_state = 2;
			shake_count++;
			if (shake_count > 10){
				printf("shake!\n");
				Send_Action("shake", 5);
				shake_state = 0;
			}
		}else if (HAL_GetTick() - start_time > 3000){
			printf("no shake but = %d\n", shake_count);
			shake_state = 0;
		}
		break;
    case 2:
      	if (x > 500){
//  			start_time = HAL_GetTick();
  			shake_state = 1;
  			shake_count++;
  			if (shake_count > 10){
  				printf("shake!\n");
				Send_Action("shake", 5);
				shake_state = 0;
			}
  		}else if (HAL_GetTick() - start_time > 3000){
			printf("no shake but = %d\n", shake_count);
  			shake_state = 0;
  		}
  		break;
    }
//    continue;
//    if (cooldown){
//    	if (HAL_GetTick() - cd_start_time > 350){
//    		cooldown = false;
//    	}else{
//    		continue;
//    	}
//    }
//    printf("x_state = %d\n", x_state);
    switch (x_state){
    case 0: //no move
    	if (x_max > x_threshold){
    		char msgr[20];
    		sprintf(msgr, "r%d",x_max);
			Send_Action(msgr,strlen(msgr));
		}
    	if (x_min < -x_threshold){
    		char msgl[20];
			sprintf(msgl, "l%d",-x_min);
			Send_Action(msgl,strlen(msgl));
//			Send_Action("left",4);
		}
    	x_max = 0;
    	x_min = 0;
    	if (x > 300){
    		x_max = x;
    		x_state = 1;
//    		printf("x state swtiched to %d \n", x_state);
    	}
    	if (x < -300){
    		x_min = x;
    		x_state = -1;
//    		printf("x state swtiched to %d \n", x_state);
    	}
    	break;
    case 1:
    	if (x > x_max){
    		x_max = x;
    	}
    	if (x < 300){
    		x_state = 2;
//    		printf("x state swtiched to %d \n", x_state);
    		if (x_max > x_threshold){
        		printf("right: %d\n",x_max);
//        		Send_Action("right",5);
//        		command = "right";
////        		command_len = 5;
//        		const char* cmd = "right";
//        		osMessagePut(CommandQueueHandle, (uint32_t)cmd, 0);
    		}

    		x_min = x;
    	}
    	break;
    case 2:
    	if (x < x_min){
    		x_min = x;
    	}else if (x > -300){
    		x_state = 3;
//    		printf("x state swtiched to %d \n", x_state);
    	}
    	break;
    case 3:
    	x_min = 0;
    	if (x < 100 || x > -100){
    		x_state = 0;


//    		printf("x state swtiched to %d \n", x_state);
    	}
    	break;
   case -1:
       	if (x < x_min){
       		x_min = x;
       	}
       	if (x > -300){
       		x_state = -2;
//       		printf("x state swtiched to %d \n", x_state);
       		if (x_min < -x_threshold){
       			printf("left: %d\n",x_min);
//       			command = "left";
//				command_len = 4;
//       			Send_Action("left",4);
//				const char* cmd = "left";
//				osMessagePut(CommandQueueHandle, (uint32_t)cmd, 0);
			}
//
//       		Send_Action("left",4);
       		x_max = x;
       	}
       	break;
   case -2:
       	if (x > x_max){
       		x_max = x;
       	}else if (x < 300){
       		x_state = -3;
//       		printf("x state swtiched to %d \n", x_state);
       	}
       	break;
   case -3:
	   	x_max = 0;
       	if (x < 100 || x > -100){
       		x_state = 0;

//       		printf("x state swtiched to %d \n", x_state);
       	}
       	break;
    }



    switch (z_state){
        case 0:
            if (z_max > 900){
            	char msgu[20];
            	sprintf(msgu, "u%d",z_max);
				Send_Action(msgu,strlen(msgu));
//                Send_Action("up", 2);
            }
            if (z_min < -1200){
            	char msgd[20];
				sprintf(msgd, "d%d",-z_min);
				Send_Action(msgd,strlen(msgd));
//                Send_Action("down", 4);
            }
            z_max = 0;
            z_min = 0;
            if (z > 300){
                z_max = z;
                z_state = 1;
            }
            if (z < -300){
                z_min = z;
                z_state = -1;
            }
            break;
        case 1:
            if (z > z_max){
                z_max = z;
            }
            if (z < 300){
                z_state = 2;
                if (z_max > z_threshold){
                    printf("up: %d\n", z_max);
                }
                z_min = z;
            }
            break;
        case 2:
            if (z < z_min){
                z_min = z;
            } else if (z > -300){
                z_state = 3;
            }
            break;
        case 3:
            z_min = 0;
            if (z < 100 || z > -100){
                z_state = 0;
            }
            break;
        case -1:
            if (z < z_min){
                z_min = z;
            }
            if (z > -300){
                z_state = -2;
                if (z_min < -z_threshold){
                    printf("down: %d\n", z_min);
                }
                z_max = z;
            }
            break;
        case -2:
            if (z > z_max){
                z_max = z;
            } else if (z < 300){
                z_state = -3;
            }
            break;
        case -3:
            z_max = 0;
            if (z < 100 || z > -100){
                z_state = 0;
            }
            break;
        }
    continue;

    if (x > x_threshold)
    {
//      printf("x = %d \n", x);
      printf("right\n");
      Send_Action("right", 5);
      cooldown = true;
      cd_start_time = HAL_GetTick();
//      osDelay(350);
    }
    else if (x < -x_threshold)
    {
//      printf("x = %d \n", x);
      printf("left\n");
      Send_Action("left", 4);
      cooldown = true;
      cd_start_time = HAL_GetTick();
//      osDelay(350);
    }
    if (z - g > z_threshold)
    {
//      printf("z = %d\n", z);

      printf("up\n");
      Send_Action("up", 2);
      cooldown = true;
      cd_start_time = HAL_GetTick();
//      osDelay(350);
    }
    else if (z - g < -z_threshold)
    {
//      printf("z = %d\n", z);

      printf("down\n");
      Send_Action("down", 4);
      cooldown = true;
      cd_start_time = HAL_GetTick();
//      osDelay(350);
    }

    continue;
  }
  /* USER CODE END StartTaskACC */
}

void StartTaskShake(void const *argument)
{
//	osDelay(10000000)
			int16_t x, prev_x;
    float velocity = 0.0f;
    int prev_sign = 0;
    int curr_sign = 0;
    int shake_count = 0;

    const uint32_t time_window_ms = 3000; // 3 seconds
    const uint32_t sampling_interval_ms = 20;
    const int required_shakes = 6; // Adjust as needed

    uint32_t start_time = HAL_GetTick();

    for (;;)
    {
//    	printf("shake for loop\n");
    	osDelay(100000);
    	continue;
        BSP_ACCELERO_AccGetXYZ(pDataXYZ);
        x = pDataXYZ[0];

        // Approximate velocity integration
        velocity += ((float)x / 1000.0f) * ((float)sampling_interval_ms / 1000.0f); // x in mg, convert to g

        // Determine current sign of velocity
        if (velocity > 0.1f)
            curr_sign = 1;
        else if (velocity < -0.1f)
            curr_sign = -1;
        else
            curr_sign = 0;

        // Check for sign change (i.e., a shake)
        if (curr_sign != 0 && curr_sign != prev_sign)
        {
            shake_count++;
            printf("Shake detected! Total: %d\n", shake_count);
            prev_sign = curr_sign;
        }

        // Time window check
        uint32_t now = HAL_GetTick();
        if (now - start_time >= time_window_ms)
        {
            if (shake_count >= required_shakes)
            {
                printf("SHAKE TRIGGERED! Sending BLE message...\n");

                // Example BLE send function
//                const char *msg = "shake";
                Send_Action("shake", 5); // Replace with your actual BLE send function

            }

            // Reset for next window
            start_time = now;
            shake_count = 0;
            velocity = 0.0f;
            prev_sign = 0;
        }

        osDelay(sampling_interval_ms);
    }
}



//void StartTaskShake(void const *argument)
//{
//  /* USER CODE BEGIN StartTaskACC */
////  const TickType_t ten_seconds = pdMS_TO_TICKS(10000);
////  const TickType_t three_seconds = pdMS_TO_TICKS(3000);
//
//
//  int16_t prev_x, prev_y, prev_z;
//  int16_t x,y,z;
////  TickType_t start_tick = xTaskGetTickCount();
//  BSP_ACCELERO_AccGetXYZ(pDataXYZ); // Read accelerometer
//  prev_x = pDataXYZ[0];
//  prev_y = pDataXYZ[1];
//  prev_z = pDataXYZ[2];
//  uint16_t shake_count = 0;
//  uint8_t state = 0;
//  uint8_t periods = 0;
//  for (;;)
//  {
//
//	  x = pDataXYZ[0];
//	  y = pDataXYZ[1];
//	  z = pDataXYZ[2];
//	  switch (state){
//	  case 0:
//		  if ((x * x) > 50*50)
//		  {
//			shake_count = 1;
//			state = 1;
//			if (x-prev_x < 0){
//				state = 2;
//			}
//			periods = 0;
//		  }
//		  break;
//	  case 1:
////		  periods++;
//		  if (x < 0)
//		  {
//			printf("little left, x = %d\n", x);
//			shake_count++;
//			state = 2;
//			if (shake_count > 114514){
//				printf("shake!\n");
//				state = 0;
//				shake_count = 0;
//				periods = 0;
//			}
//
//		  }
//		  if (periods >40){
//			printf("no shake, shakes = %d\n", shake_count);
//			state = 0;
//			shake_count = 0;
//			periods = 0;
//		  }
//		  break;
//	  case 2:
////		  periods++;
//		  if (x > 0)
//		  {
//			printf("little right, x = %d\n", x);
//			shake_count++;
//			state = 1;
//			if (shake_count > 114514){
//				printf("shake!\n");
//				state = 0;
//				shake_count = 0;
//				periods = 0;
//			}
//
//		  }
//		  if (periods >40){
//			printf("no shake, shakes = %d\n", shake_count);
//			state = 0;
//			shake_count = 0;
//			periods = 0;
//		  }
//		  break;
//	  }
//	  prev_x = x;
//	  prev_y = y;
//	  prev_z = z;
//	  osDelay(20);
//	  continue;
//  }
//  /* USER CODE END StartTaskACC */
//}
/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
