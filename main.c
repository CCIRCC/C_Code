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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "valuepack_hal.h"
#include "stepper_motor.h"
#include "servo.h"
#include "geared_motor.h"
#include "water_pump.h"
#include "57stepper_motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MOTOR_FULL_SPEED 100  // 固定全速占空比
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
StepperMotor my_motor;
 RxPack rxpack;
 extern unsigned char vp_rxbuff[VALUEPACK_BUFFER_SIZE];
// static uint8_t last_motor_cmd = 0xFF; // 初始化为不可能的值
 
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	_57_Motor_Init();
	_57_Motor_Stop_Channel1();
	Motor_Init(&htim1);
	Servo_Init(&htim2);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1, (uint8_t*)vp_rxbuff, VALUEPACK_BUFFER_SIZE);
	Stepper_Init(&my_motor, 
                ENABLE_GPIO_Port, ENABLE_Pin,
                STEP_GPIO_Port, STEP_Pin,
                DIR_GPIO_Port, DIR_Pin);
  Stepper_Enable(&my_motor);
	water_pump_init();

			 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){
//		    Stepper_Run(&my_motor, MOTOR_FORWARD, 3000);
//        HAL_Delay(1000);
//        
//        // 反转200步
//        Stepper_Run(&my_motor, MOTOR_REVERSE, 3000);
//        HAL_Delay(1000);
//		_57_Motor_SetDirection(MOTOR_DIR_FORWARD);
//    HAL_Delay(2000);
////        
//    // 反向运行2秒
//    _57_Motor_SetDirection(MOTOR_DIR_REVERSE);
//    HAL_Delay(2000);
//		
//		 Set_Servo_Angle(&htim2,90);
//					HAL_Delay(1000);
//          Set_Servo_Angle(&htim2,180);
//          HAL_Delay(1000);	
//		
//    HAL_Delay(2000);
//    Motor_Stop();
//    HAL_Delay(1000);
//    Motor_Backward(1000); // 80%速度反转
//    HAL_Delay(2000);
//		Stepper_Run(&my_motor, MOTOR_FORWARD, 3000);
//		HAL_Delay(1000);
//		for(int i=0;i<100000;i++)
//		{}
//    _57_Motor_Init();
//    _57_Motor_SetDirection(MOTOR_DIR_FORWARD);
		if(readValuePack(&rxpack))
    {
				setLED(rxpack.bools[0]);
				if (rxpack.bytes[0]==1){
					Stepper_Run(&my_motor, MOTOR_FORWARD, 200);
					HAL_Delay(1000);			
				}
				if (rxpack.bytes[1]==1){	
					Set_Servo_Angle(&htim2,90);
					HAL_Delay(1000);
          Set_Servo_Angle(&htim2,180);
          HAL_Delay(1000);			
				}
				if (rxpack.bytes[2]==1){	
					Motor_Forward(1000); // 50%速度正转	
				}
				if (rxpack.bytes[2]!=1){	
					Motor_Stop(); // 50%速度正转	
				}
				if (rxpack.bytes[3]==1){	
					set_water_pump_intensity(700);
				}
				if (rxpack.bytes[3]!=1){	
					set_water_pump_intensity(0);
				}
				
				if (rxpack.bytes[4]==1){	
						_57_Motor_SetDirection(MOTOR_DIR_FORWARD);
             HAL_Delay(2000);
//        
    // 反向运行2秒
             _57_Motor_SetDirection(MOTOR_DIR_REVERSE);
           HAL_Delay(2000);
					_57_Motor_Stop_Channel1();
				}
//				  /* 57步进电机控制（使用bytes[4]） */
//        uint8_t current_cmd = rxpack.bytes[4];
//				// 仅在指令变化时执行操作
//        if(current_cmd != last_motor_cmd) {
//            switch(current_cmd) {
//                case 1:  // 正向运行
//                    _57_Motor_SetDirection(MOTOR_DIR_FORWARD);
//                    _57_Motor_SetSpeed(MOTOR_CH1, MOTOR_FULL_SPEED);
//                    break;
//                    
//                case 2:  // 反向运行
//                    _57_Motor_SetDirection(MOTOR_DIR_REVERSE);
//                    _57_Motor_SetSpeed(MOTOR_CH1, MOTOR_FULL_SPEED);
//                    break;
//                    
//                default:    // 停止及其他无效指令
//                    _57_Motor_Stop_Channel1();
//                    current_cmd = 0x00; // 同步状态
//                    break;
//            }
//            last_motor_cmd = current_cmd;
//        }
//				
				
			}
		
			
				
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
