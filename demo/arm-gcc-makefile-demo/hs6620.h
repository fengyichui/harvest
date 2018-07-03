/**
 * @file hs6620.h
 * @brief 
 * @date 2015/07/16 14:52:07
 * @author liqiang
 *
 * @addtogroup 
 * @ingroup 
 * @details 
 *
 * @{
 */

#ifndef __HS6620_H__
#define __HS6620_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * Configuration of the Cortex-M3 Processor and Core Peripherals
 */
#define __MPU_PRESENT             0 /*!< no MPU */
#define __NVIC_PRIO_BITS          4 /*!< 4 Bits for the Priority Levels    */
#define __Vendor_SysTickConfig    0 /*!< Set to 1 if different SysTick Config is used */

/*********************************************************************
 * Interrupt Number Definition
 */
typedef enum IRQn
{
    /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
    MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
    BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
    UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
    SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
    DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
    PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
    SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

    /******  HS6620 specific Interrupt Numbers *********************************************************/
    BT_IRQn                     = 0,
    SLEEP_TIMER_IRQn            = 1,
    DMA_IRQn                    = 2,
    GPIO_IRQn                   = 3,
    TIM_IRQn                    = 4,
    KPP_DEPRESS_IRQn            = 5,
    KPP_RELEASE_IRQn            = 6,
    RSV2_IRQn                   = 7,
    UART0_IRQn                  = 8,
    UART1_IRQn                  = 9,
    I2C_IRQn                    = 10,
    PIN_WAKEUP_IRQn             = 11,
    ADC_IRQn                    = 12,
    SPI0_IRQn                   = 13,
    SPI1_IRQn                   = 14,
    RSV3_IRQn                   = 15,
    DMA0_IRQn                   = 16,
    DMA1_IRQn                   = 17,
    DMA2_IRQn                   = 18,
    DMA3_IRQn                   = 19,
    RTC_AF_IRQn                 = 20,
    RTC_1HZ_IRQn                = 21,
    RTC_BLE_IRQn                = 22,
    RSV4_IRQn                   = 23,
    RSV5_IRQn                   = 24,
    RSV6_IRQn                   = 25,
    GPIO0_IRQn                  = 26,
    GPIO1_IRQn                  = 27,
    TIM0_IRQn                   = 28,
    TIM1_IRQn                   = 29,
    TIM2_IRQn                   = 30,
    AES_IRQn                    = 31,
    WDT_IRQn                    = 32,
    SF_IRQn                     = 33,
    QDEC_IRQn                   = 34,
    POWER_DOWN_IRQn             = 35,
    ZIG_MAC_IRQn                = 36,
    ZIG_TIMER_IRQn              = 37,
    MAC6200_RF_IRQn             = 38,
    MAC6200_SPI_IRQn            = 39,
    SOFT0_IRQn                  = 40,
    SOFT1_IRQn                  = 41,
    SOFT2_IRQn                  = 42,
    SOFT3_IRQn                  = 43,
    SOFT4_IRQn                  = 44,
    SOFT5_IRQn                  = 45,
    SOFT6_IRQn                  = 46,
    SOFT7_IRQn                  = 47,
} IRQn_Type;

/*********************************************************************
 * INCLUDES
 */

#include "core_cm3.h"
#include <stdint.h>

#ifdef __CC_ARM
#pragma anon_unions
#endif

/*********************************************************************
 * MACROS
 */


#ifdef __cplusplus
}
#endif

#endif

/** @} */
