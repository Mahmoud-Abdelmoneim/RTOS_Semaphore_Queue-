/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD_interface.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
	uint32_t ID;
	uint8_t name[20];
}student_t;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t counter=0;

QueueHandle_t  xQueueHandle1;
QueueHandle_t  xQueueHandle2;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Task_1(void *argument);
void Task_2(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 1, &myBinarySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  xQueueHandle1 = xQueueCreate(10, sizeof(uint32_t));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(Task_1, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(Task_2, NULL, &myTask02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Task_1 */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Task_1 */
void Task_1(void *argument)
{
  /* USER CODE BEGIN Task_1 */
	BaseType_t RetVal=pdTRUE;
	uint8_t count=0;
  /* Infinite loop */
  for(;;)
  {

	  RetVal = xQueueSendToFront(xQueueHandle1,(void *)&count,1000);
	  if(RetVal == pdTRUE)
	  {
	      //osSemaphoreAcquire(myBinarySem01Handle, 4000);
		  //LCD_Send_Command(_LCD_CLEAR);
		  LCD_Send_String_Pos("sending succeeded",1,1);
		  osSemaphoreRelease(myBinarySem01Handle);
	  }
	  else{
		  LCD_Send_Command(_LCD_CLEAR);
		  HAL_Delay(100);
		  LCD_Send_String("send failed ");
		  HAL_Delay(100);
	  }
   	  count++;
	  osDelay(1500);

  }
  /* USER CODE END Task_1 */
}

/* USER CODE BEGIN Header_Task_2 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_2 */
void Task_2(void *argument)
{
  /* USER CODE BEGIN Task_2 */
	BaseType_t RetVal=pdTRUE;
	uint8_t count=0;
  /* Infinite loop */

  for(;;)
  {
	  RetVal= xQueueReceive(xQueueHandle1, &count, portMAX_DELAY);
	  if(pdTRUE == RetVal)
	  {
		  osSemaphoreAcquire(myBinarySem01Handle, 4000);
		  LCD_Send_String_Pos("message Rece=",2,1);
		  LCD_Send_Number_Pos(count, 2, 14);
		  osSemaphoreRelease(myBinarySem01Handle);
	  }
	  else{
		  LCD_Send_Command(_LCD_CLEAR);
		  HAL_Delay(100);
		  LCD_Send_String("queue not Rec ");
		  HAL_Delay(100);
	  }
	    //osDelay(1000);
  }
  /* USER CODE END Task_2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

