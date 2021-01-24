
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

static TIM_HandleTypeDef htim4 = {
    .Instance = TIM4
};

void MX_TIM4_Init(void){
	TIM_MasterConfigTypeDef sMasterConf;
	TIM_OC_InitTypeDef sConfOC;
	__TIM4_CLK_ENABLE();
	htim4.Init.Prescaler = 10;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 65535;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&htim4);

	sMasterConf.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConf.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConf);

	sConfOC.OCMode = TIM_OCMODE_PWM1;
	sConfOC.Pulse = 0;
	sConfOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfOC, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfOC, TIM_CHANNEL_4);

	HAL_TIM_PWM_MspInit(&htim4);

}

void GPIO_Init(void){
	__GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Infra;

	GPIO_Infra.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_Infra.Mode = GPIO_MODE_AF_PP;
	GPIO_Infra.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Infra.Pull = GPIO_NOPULL;
	GPIO_Infra.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD, &GPIO_Infra);
}

void setPWM(uint16_t pulse)
{
	TIM_OC_InitTypeDef sCOnfOC;

	sCOnfOC.OCMode = TIM_OCMODE_PWM1;
	sCOnfOC.Pulse = pulse;
	sCOnfOC.OCNPolarity = TIM_OCPOLARITY_HIGH;
	sCOnfOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim4, &sCOnfOC, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim4, TIM_CHANNEL_1);
}

void Delay_ms(volatile int time_ms)
{
	int j;
	for(j = 0; j < time_ms*4000; j++)
	{}  /* excute NOP for 1ms */
}

int main(void){
	HAL_Init();
	GPIO_Init();
	MX_TIM4_Init();

	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);

	while(1){
		int i;
		for (i = 0; i <= 65535; i += 50)
		{
			// If we want to set PWM duty cycle after initialize,
			// we can't use TIM_OCInitStruct.TIM_Pulse,
			// but directly to TIM4 compare register 1 (CCR1)
			TIM4->CCR1 = i;
			TIM4->CCR2 = i;
			TIM4->CCR3 = i;
			TIM4->CCR4 = i;
			Delay_ms(5);
		}
		for (i = 65535; i > 0; i -= 50)
		{
			// If we want to set PWM duty cycle after initialize,
			// we can't use TIM_OCInitStruct.TIM_Pulse,
			// but directly to TIM4 compare register 1 (CCR1)
			TIM4->CCR1 = i;
			TIM4->CCR2 = i;
			TIM4->CCR3 = i;
			TIM4->CCR4 = i;
			Delay_ms(5);
		}
	}
}
