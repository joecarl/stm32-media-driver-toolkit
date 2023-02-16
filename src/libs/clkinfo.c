#include <stm32f4xx.h>

static uint32_t PCLK1TIM(void)
{
	/* Get PCLK1 frequency */
	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	
	/* Get PCLK1 prescaler */
	if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
	{
		/* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
		return (pclk1);
	}
	else
	{
		/* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
		return(2 * pclk1);
	}
}

static uint32_t PCLK2TIM(void)
{
	/* Get PCLK2 frequency */
	uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
	
	/* Get PCLK2 prescaler */
	if ((RCC->CFGR & RCC_CFGR_PPRE2) == 0)
	{
		/* PCLK2 prescaler equal to 1 => TIMCLK = PCLK2 */
		return (pclk2);
	}
	else
	{
		/* PCLK2 prescaler different from 1 => TIMCLK = 2 * PCLK2 */
		return(2 * pclk2);
	}
}

float GetAPB1TimersMHz() {

	return ((float) PCLK1TIM()) / 1000000.0;

}

float GetAPB2TimersMHz() {

	return ((float) PCLK2TIM()) / 1000000.0;

}