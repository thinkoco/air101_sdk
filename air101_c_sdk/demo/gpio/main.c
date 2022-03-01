
#include <stdio.h>
#include "wm_hal.h"

void Error_Handler(void);
static void GPIO_Init(void);

static volatile uint8_t key_flag = 0;
static volatile uint8_t cnt = 0;
int main(void)
{
    SystemClock_Config(CPU_CLK_160M);
    printf("air101 gpio main!\r\n");
    HAL_Init();
    GPIO_Init();
    
    while (1)
    {
        HAL_Delay(100);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
        if (key_flag == 1)
        {
            HAL_Delay(20);
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
            {
                if((cnt & 0x1) == 0){
                    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
                } else{
                    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10); 
                }
                cnt++;
            }
            key_flag = 0;
        }
    }
    
    return 0;
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
    
    HAL_NVIC_SetPriority(GPIOA_IRQn, 0);
    HAL_NVIC_EnableIRQ(GPIOA_IRQn);

}

void HAL_GPIO_EXTI_Callback(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    if ((GPIOx == GPIOA) && (GPIO_Pin == GPIO_PIN_0))
    {
        key_flag = 1;
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
