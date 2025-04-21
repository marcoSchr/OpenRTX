/**
  **************************************************************************
  * @file     system_at32f423.c
  * @brief    contains all the functions for cmsis cortex-m4 system source file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup AT32F423_system
  * @{
  */

#include "at32f423.h"

/** @addtogroup AT32F423_system_private_defines
  * @{
  */
#define VECT_TAB_OFFSET                  0x0 /*!< vector table base offset field. this value must be a multiple of 0x200. */
/**
  * @}
  */

/** @addtogroup AT32F423_system_private_variables
  * @{
  */
unsigned int system_core_clock           = 144000000; /*!< system clock frequency (core clock) */
extern uint32_t __Vectors[];
/**
  * @}
  */

/** @addtogroup AT32F423_system_private_functions
  * @{
  */

/** @addtogroup AT32F421_system_private_functions
  * @{
  */
static void SetSysClock()
{
    FLASH->psr = flash_psr_set(FLASH_WAIT_CYCLE_4); /* Three wait cycles, prefetch enabled */

    CRM_REG(CRM_PWC_PERIPH_CLOCK) |= (CRM_REG_BIT(TRUE));

    pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);

    CRM->ctrl_bit.hicken = TRUE;
    while (CRM->ctrl_bit.hickstbl == 0) ;

    CRM->pllcfg_bit.pllrcs = CRM_PLL_SOURCE_HICK;
    CRM->pllcfg_bit.pllns = 72;
    CRM->pllcfg_bit.pllms = 1;
    CRM->pllcfg_bit.pllfr = CRM_PLL_FR_2;

    CRM->ctrl_bit.pllen = 1;                        /* Start PLL and wait until is stable  */
    while (CRM->ctrl_bit.pllstbl == 0) ;

    CRM->misc2_bit.auto_step_en = CRM_AUTO_STEP_MODE_ENABLE;
    CRM->cfg_bit.ahbdiv = CRM_AHB_DIV_1;
    CRM->misc2_bit.auto_step_en = CRM_AUTO_STEP_MODE_DISABLE;

    CRM->cfg_bit.apb2div = CRM_APB2_DIV_1;

    CRM->cfg_bit.apb1div = CRM_APB1_DIV_2;

    CRM->misc2_bit.auto_step_en = CRM_AUTO_STEP_MODE_ENABLE;
    CRM->cfg_bit.sclksel = CRM_SCLK_PLL;            /* Switch to PLL clock                 */
    CRM->misc2_bit.auto_step_en = CRM_AUTO_STEP_MODE_DISABLE;
    while (CRM->cfg_bit.sclksts != CRM_SCLK_PLL) ;

    CRM->misc2_bit.auto_step_en = CRM_AUTO_STEP_MODE_DISABLE;
}


/**
  * @brief  setup the microcontroller system
  *         initialize the flash interface.
  * @note   this function should be used only after reset.
  * @param  none
  * @retval none
  */
void SystemInit (void)
{
#if defined (__FPU_USED) && (__FPU_USED == 1U)
  SCB->CPACR |= ((3U << 10U * 2U) |         /* set cp10 full access */
                 (3U << 11U * 2U)  );       /* set cp11 full access */
#endif

  CRM->misc2_bit.auto_step_en = CRM_AUTO_STEP_MODE_ENABLE;

  /* set hicken bit */
  CRM->ctrl_bit.hicken = TRUE;

  /* wait hick stable */
  while(CRM->ctrl_bit.hickstbl != SET);

  /* hick used as system clock */
  CRM->cfg_bit.sclksel = CRM_SCLK_HICK;

  /* wait sclk switch status */
  while(CRM->cfg_bit.sclksts != CRM_SCLK_HICK);

  /* reset hexten, hextbyps, cfden and pllen bits */
  CRM->ctrl &= ~(0x010D0000U);

  /* reset cfg register, include sclk switch, ahbdiv, apb1div, apb2div, adcdiv, clkout bits */
  CRM->cfg = 0x40000000U;

  /* reset pllms pllns pllfr pllrcs bits */
  CRM->pllcfg = 0x00033002U;

  /* reset clkout_sel, clkoutdiv, pllclk_to_adc, hick_to_usb */
  CRM->misc1 &= 0x00005000U;
  CRM->misc1 |= 0x000F0000U;

  /* disable all interrupts enable and clear pending bits  */
  CRM->clkint = 0x009F0000U;

  SetSysClock();
  wait_for_power_stable();

  SCB->VTOR = (uint32_t)(&__Vectors[0]);
}

/**
  * @brief  update system_core_clock variable according to clock register values.
  *         the system_core_clock variable contains the core clock (hclk), it can
  *         be used by the user application to setup the systick timer or configure
  *         other parameters.
  * @param  none
  * @retval none
  */
void system_core_clock_update(void)
{
  uint32_t pll_ns = 0, pll_ms = 0, pll_fr = 0, pll_clock_source = 0, pllrcsfreq = 0;
  uint32_t temp = 0, div_value = 0, psc = 0;
  crm_sclk_type sclk_source;

  static const uint8_t sys_ahb_div_table[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
  static const uint8_t pll_fr_table[6] = {1, 2, 4, 8, 16, 32};

  /* get sclk source */
  sclk_source = CRM->cfg_bit.sclksts;

  switch(sclk_source)
  {
    case CRM_SCLK_HICK:
      if(((CRM->misc1_bit.hick_to_sclk) != RESET) && ((CRM->misc1_bit.hickdiv) != RESET))
        system_core_clock = HICK_VALUE * 6;
      else
        system_core_clock = HICK_VALUE;

      psc = CRM->misc2_bit.hick_to_sclk_div;
      system_core_clock = system_core_clock >> psc;
      break;
    case CRM_SCLK_HEXT:
      system_core_clock = HEXT_VALUE;
      psc = CRM->misc2_bit.hext_to_sclk_div;
      system_core_clock = system_core_clock >> psc;
      break;
    case CRM_SCLK_PLL:
      /* get pll clock source */
      pll_clock_source = CRM->pllcfg_bit.pllrcs;

      /* get multiplication factor */
      pll_ns = CRM->pllcfg_bit.pllns;
      pll_ms = CRM->pllcfg_bit.pllms;
      pll_fr = pll_fr_table[CRM->pllcfg_bit.pllfr];

      if (pll_clock_source == CRM_PLL_SOURCE_HICK)
      {
        /* hick selected as pll clock entry */
        pllrcsfreq = HICK_VALUE;
      }
      else
      {
        /* hext selected as pll clock entry */
        pllrcsfreq = HEXT_VALUE;
      }

      system_core_clock = (uint32_t)(((uint64_t)pllrcsfreq * pll_ns) / (pll_ms * pll_fr) / 2);
      break;
    default:
      system_core_clock = HICK_VALUE;
      break;
  }

  /* compute sclk, ahbclk frequency */
  /* get ahb division */
  temp = CRM->cfg_bit.ahbdiv;
  div_value = sys_ahb_div_table[temp];
  /* ahbclk frequency */
  system_core_clock = system_core_clock >> div_value;
}

/**
  * @brief  take some delay for waiting power stable, delay is about 60ms with frequency 8MHz.
  * @param  none
  * @retval none
  */
void wait_for_power_stable(void)
{
  volatile uint32_t delay = 0;
  for(delay = 0; delay < 50000; delay++);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

