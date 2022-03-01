
#include <stdio.h>
#include "wm_hal.h"
#include "FreeRTOS.h"
#include "task.h"

void Error_Handler(void);
void task1_handle(void *p);
void task2_handle(void *p);
void task3_handle(void *p);
static void GPIO_Init(void);
const HeapRegion_t xHeapRegions[] = 
{ 
    { ( uint8_t * ) 0x20020000UL, 0x5000 },
    { NULL, 0 }/*终止数组 */ 
};

TaskHandle_t htask1 = NULL;
TaskHandle_t htask2 = NULL;
TaskHandle_t htask3 = NULL;
/* 使用RreeRTOS时，需要将/include/arch/xt804/csi_config.h里
*  的#define CONFIG_KERNEL_NONE 1宏定义注释掉，否则os的systic
*  会出错导致os系统异常
*/
int main(void)
{
	SystemClock_Config(CPU_CLK_160M);
	printf("enter main\r\n");
	HAL_Init();
	GPIO_Init();
	vPortDefineHeapRegions( xHeapRegions );
	xTaskCreate(task1_handle, "task1", 512, NULL, 35, &htask1);
	xTaskCreate(task2_handle, "task2", 512, NULL, 32, &htask2);
    xTaskCreate(task3_handle, "task3", 512, NULL, 30, &htask3);
	vTaskStartScheduler();

	return 0;
}

void task1_handle(void *p)
{
	for (;;)
	{
	    printf("task1_handle\r\n");
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
		vTaskDelay(100);
	}
}

void task2_handle(void *p)
{
	for (;;)
	{
	    printf("task2_handle\r\n");
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
		vTaskDelay(1000);
	}
}

void task3_handle(void *p)
{
	uint8_t flag = 1;
	for (;;)
	{   
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
			if(flag){
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
				flag = 0;
			    printf("task3_handle: PB10 LED Toggle Trigger!\r\n");
			}
		} else {
			flag = 1;
		}
		vTaskDelay(200);
	}
}

void Error_Handler(void)
{
	while (1)
	{
	}
}

void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}


static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET);
    
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
}
