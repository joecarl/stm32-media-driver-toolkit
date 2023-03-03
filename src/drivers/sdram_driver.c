#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_sdram.h>
#include "mdt/drivers/sdram_driver.h"

#define MDT_SDRAM_BASE_ADDR 0xD0000000

#define MDT_SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define MDT_SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define MDT_SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define MDT_SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define MDT_SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define MDT_SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define MDT_SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define MDT_SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define MDT_SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define MDT_SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define MDT_SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)   


static bool sdram_ready = false;

static uint32_t last_addr = MDT_SDRAM_BASE_ADDR;


void MDT_SDRAM_Init(void) {

	#ifdef HAL_SDRAM_MODULE_ENABLED

	MDT_SDRAM_HandleTypeDef hsdram;
	FMC_SDRAM_TimingTypeDef SdramTiming = {0};

	/** Perform the SDRAM1 memory initialization sequence
	 */
	hsdram.Instance = FMC_SDRAM_DEVICE;
	/* hsdram.Init */
	hsdram.Init.SDBank = FMC_SDRAM_BANK2;
	hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
	hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
	hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
	hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	hsdram.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
	hsdram.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hsdram.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	hsdram.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
	hsdram.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
	/* SdramTiming */
	SdramTiming.LoadToActiveDelay = 2;
	SdramTiming.ExitSelfRefreshDelay = 7;
	SdramTiming.SelfRefreshTime = 4;
	SdramTiming.RowCycleDelay = 7;
	SdramTiming.WriteRecoveryTime = 3;
	SdramTiming.RPDelay = 2;
	SdramTiming.RCDDelay = 2;

	if (HAL_SDRAM_Init(&hsdram, &SdramTiming) != HAL_OK)
	{
		Error_Handler( );
	}


	FMC_SDRAM_CommandTypeDef Command;

    __IO uint32_t tmpmrd = 0;
	/* Step 3:  Configure a clock configuration enable command */
	Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
	Command.AutoRefreshNumber      = 1;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&hsdram, &Command, 0x1000);

	/* Step 4: Insert 100 ms delay */
	HAL_Delay(100);

	/* Step 5: Configure a PALL (precharge all) command */ 
	Command.CommandMode            = FMC_SDRAM_CMD_PALL;
	Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
	Command.AutoRefreshNumber      = 1;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&hsdram, &Command, 0x1000);

	/* Step 6 : Configure a Auto-Refresh command */ 
	Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
	Command.AutoRefreshNumber      = 4;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&hsdram, &Command, 0x1000);

	/* Step 7: Program the external memory mode register */
	tmpmrd = (uint32_t) MDT_SDRAM_MODEREG_BURST_LENGTH_2          |
						MDT_SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
						MDT_SDRAM_MODEREG_CAS_LATENCY_3           |
						MDT_SDRAM_MODEREG_OPERATING_MODE_STANDARD |
						MDT_SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
	Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
	Command.AutoRefreshNumber      = 1;
	Command.ModeRegisterDefinition = tmpmrd;

	/* Send the command */
	HAL_SDRAM_SendCommand(&hsdram, &Command, 0x1000);

	HAL_SDRAM_ProgramRefreshRate(&hsdram, 0x56A-20);

	//*(__IO uint16_t*) (0xD0000000) = (uint16_t)0x03;

	sdram_ready = true;

	#endif
	
}


bool MDT_SDRAM_IsReady() {
	
	return sdram_ready;

}


__IO void* MDT_SDRAM_malloc(size_t size) {

	uint32_t addr = last_addr;

	last_addr += size;

	return (__IO void*) addr;

}
