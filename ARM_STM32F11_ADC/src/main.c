#include "stm32f4xx_hal.h"

void Configure_GPIO(void);
void configure_ADC2_Channel_0(void);
void Delay_ms(volatile int time_ms);
void MX_TIM4_Init(void);

uint32_t Adc_value;
ADC_HandleTypeDef myADC2Handle;

static TIM_HandleTypeDef htim4 = {
    .Instance = TIM4
};

int main(void)
{
	Configure_GPIO(); // initialize PA0 pin
	configure_ADC2_Channel_0(); // configure ADC2
	MX_TIM4_Init();

	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	while(1)
	{
	 HAL_ADC_Start(&myADC2Handle); // start A/D conversion
		if(HAL_ADC_PollForConversion(&myADC2Handle, 5) == HAL_OK) //check if conversion is completed
		{
			Adc_value  = HAL_ADC_GetValue(&myADC2Handle); // read digital value and save it inside uint32_t variable
			TIM4->CCR1 = Adc_value;
		}
		HAL_ADC_Stop(&myADC2Handle); // stop conversion
		Delay_ms(200);
	}
}

/* Configure and initialize PA0 pin as analog input pin for A/D conversion */
void Configure_GPIO(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock to GPIOA
	GPIO_InitTypeDef ADCpin; //create an instance of GPIO_InitTypeDef C struct
	ADCpin.Pin = GPIO_PIN_0; // Select pin PA0
	ADCpin.Mode = GPIO_MODE_ANALOG; // Select Analog Mode
	ADCpin.Pull = GPIO_NOPULL; // Disable internal pull-up or pull-down resistor
	HAL_GPIO_Init(GPIOA, &ADCpin); // initialize PA0 as analog input pin

	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Infra;

	GPIO_Infra.Pin = GPIO_PIN_6;
	GPIO_Infra.Mode = GPIO_MODE_AF_PP;
	GPIO_Infra.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Infra.Pull = GPIO_NOPULL;
	GPIO_Infra.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOB, &GPIO_Infra);
}
void configure_ADC2_Channel_0(void)
{
	__HAL_RCC_ADC1_CLK_ENABLE(); // enable clock to ADC2 module
	myADC2Handle.Instance = ADC1; // create an instance of ADC2
	myADC2Handle.Init.Resolution = ADC_RESOLUTION_12B; // select 12-bit resolution
	myADC2Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV; //select  single conversion as a end of conversion event
	myADC2Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT; // set digital output data right justified
	myADC2Handle.Init.ClockPrescaler =ADC_CLOCK_SYNC_PCLK_DIV8;
	HAL_ADC_Init(&myADC2Handle); // initialize AD2 with myADC2Handle configuration settings

  /*select ADC2 channel */
	ADC_ChannelConfTypeDef Channel_AN0; // create an instance of ADC_ChannelConfTypeDef
	Channel_AN0.Channel = ADC_CHANNEL_0; // select analog channel 0
	Channel_AN0.Rank = 1; // set rank to 1
	Channel_AN0.SamplingTime = ADC_SAMPLETIME_15CYCLES; // set sampling time to 15 clock cycles
	HAL_ADC_ConfigChannel(&myADC2Handle, &Channel_AN0); // select channel_0 for ADC2 module.
}
void Delay_ms(volatile int time_ms)
{
	      int j;
        for(j = 0; j < time_ms*4000; j++)
            {}  /* excute NOP for 1ms */
}

void MX_TIM4_Init(void){
	TIM_MasterConfigTypeDef sMasterConf;
	TIM_OC_InitTypeDef sConfOC;
	__TIM4_CLK_ENABLE();
	htim4.Init.Prescaler = 5;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 4095;
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
