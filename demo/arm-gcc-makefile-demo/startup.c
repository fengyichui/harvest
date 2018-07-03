/**
 * @file sys_vectors.c
 * @brief 
 * @date Mon 10 Aug 2015 04:44:09 PM CST
 * @author liqiang
 *
 * @addtogroup 
 * @ingroup 
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "stdint.h"
#include "hs6620.h"

/*********************************************************************
 * MACROS
 */
#define WEAK __attribute__((weak))
#define NVIC_IRQ_COUNT 48


/*********************************************************************
 * TYPEDEFS
 */
typedef void (*funcp_t) (void);

typedef void (*vector_table_entry_t)(void);

typedef struct {
    uint32_t *initial_sp_value; /**< Initial stack pointer value. */
    vector_table_entry_t reset;
    vector_table_entry_t nmi;
    vector_table_entry_t hard_fault;
    vector_table_entry_t memory_manage_fault; /* not in CM0 */
    vector_table_entry_t bus_fault;           /* not in CM0 */
    vector_table_entry_t usage_fault;         /* not in CM0 */
    vector_table_entry_t soft_fault;
    vector_table_entry_t reserved_x0020;
    vector_table_entry_t reserved_x0024;
    vector_table_entry_t reserved_x0028;
    vector_table_entry_t sv_call;
    vector_table_entry_t debug_monitor;       /* not in CM0 */
    vector_table_entry_t reserved_x0034;
    vector_table_entry_t pend_sv;
    vector_table_entry_t systick;
    vector_table_entry_t irq[NVIC_IRQ_COUNT];
} vector_table_t;

/*********************************************************************
 * EXTERN
 */

/* Symbols exported by the linker script(s): */
extern uint32_t _data_loadaddr, _data, _edata, _ebss, _stack, _estack;

int main(void);
void system_init(void);
void blocking_handler(void);
void null_handler(void);

void WEAK reset_handler(void);
void WEAK nmi_handler(void);
void WEAK hard_fault_handler(void);
void WEAK soft_fault_handler(int code);
void WEAK sv_call_handler(void);
void WEAK pend_sv_handler(void);
void WEAK SysTick_Handler(void);

/* Those defined only on ARMv7 and above */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
void WEAK mem_manage_handler(void);
void WEAK bus_fault_handler(void);
void WEAK usage_fault_handler(void);
void WEAK debug_monitor_handler(void);
#endif

/* External Interrupts */
void WEAK BT_IRQHandler(void);
void WEAK SLEEP_TIMER_IRQHandler(void);
void WEAK DMA_IRQHandler(void);
void WEAK GPIO_IRQHandler(void);
void WEAK TIM_IRQHandler(void);
void WEAK KPP_DEPRESS_IRQHandler(void);
void WEAK KPP_RELEASE_IRQHandler(void);
void WEAK UART0_IRQHandler(void);
void WEAK UART1_IRQHandler(void);
void WEAK I2C_IRQHandler(void);
void WEAK PIN_WAKEUP_IRQHandler(void);
void WEAK ADC_IRQHandler(void);
void WEAK SPI0_IRQHandler(void);
void WEAK SPI1_IRQHandler(void);
void WEAK DMA0_IRQHandler(void);
void WEAK DMA1_IRQHandler(void);
void WEAK DMA2_IRQHandler(void);
void WEAK DMA3_IRQHandler(void);
void WEAK RTC_AF_IRQHandler(void);
void WEAK RTC_1HZ_IRQHandler(void);
void WEAK RTC_BLE_IRQHandler(void);
void WEAK GPIO0_IRQHandler(void);
void WEAK GPIO1_IRQHandler(void);
void WEAK TIM0_IRQHandler(void);
void WEAK TIM1_IRQHandler(void);
void WEAK TIM2_IRQHandler(void);
void WEAK AES_IRQHandler(void);
void WEAK WDT_IRQHandler(void);
void WEAK SF_IRQHandler(void);
void WEAK QDEC_IRQHandler(void);
void WEAK POWER_DOWN_IRQHandler(void);
void WEAK ZIG_MAC_IRQHandler(void);
void WEAK ZIG_TIMER_IRQHandler(void);
void WEAK SOFT0_IRQHandler(void);
void WEAK MAC6200_RF_IRQHandler(void);
void WEAK MAC6200_SPI_IRQHandler(void);
void WEAK SOFT1_IRQHandler(void);
void WEAK SOFT2_IRQHandler(void);
void WEAK SOFT3_IRQHandler(void);
void WEAK SOFT4_IRQHandler(void);
void WEAK SOFT5_IRQHandler(void);
void WEAK SOFT6_IRQHandler(void);
void WEAK SOFT7_IRQHandler(void);

#pragma weak nmi_handler                = null_handler
#pragma weak sv_call_handler                = null_handler
#pragma weak pend_sv_handler             = null_handler
#pragma weak SysTick_Handler            = null_handler

/* Those are defined only on CM3 or CM4 */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#pragma weak mem_manage_handler         = blocking_handler
#pragma weak bus_fault_handler          = blocking_handler
#pragma weak usage_fault_handler        = blocking_handler
#pragma weak debug_monitor_handler      = null_handler
#endif

#pragma weak BT_IRQHandler              = blocking_handler
#pragma weak SLEEP_TIMER_IRQHandler     = blocking_handler
#pragma weak DMA_IRQHandler             = blocking_handler
#pragma weak GPIO_IRQHandler            = blocking_handler
#pragma weak TIM_IRQHandler             = blocking_handler
#pragma weak KPP_DEPRESS_IRQHandler     = blocking_handler
#pragma weak KPP_RELEASE_IRQHandler     = blocking_handler
#pragma weak UART0_IRQHandler           = blocking_handler
#pragma weak UART1_IRQHandler           = blocking_handler
#pragma weak I2C_IRQHandler             = blocking_handler
#pragma weak PIN_WAKEUP_IRQHandler      = blocking_handler
#pragma weak ADC_IRQHandler             = blocking_handler
#pragma weak SPI0_IRQHandler            = blocking_handler
#pragma weak SPI1_IRQHandler            = blocking_handler
#pragma weak DMA0_IRQHandler            = blocking_handler
#pragma weak DMA1_IRQHandler            = blocking_handler
#pragma weak DMA2_IRQHandler            = blocking_handler
#pragma weak DMA3_IRQHandler            = blocking_handler
#pragma weak RTC_AF_IRQHandler          = blocking_handler
#pragma weak RTC_1HZ_IRQHandler         = blocking_handler
#pragma weak RTC_BLE_IRQHandler         = blocking_handler
#pragma weak GPIO0_IRQHandler           = blocking_handler
#pragma weak GPIO1_IRQHandler           = blocking_handler
#pragma weak TIM0_IRQHandler            = blocking_handler
#pragma weak TIM1_IRQHandler            = blocking_handler
#pragma weak TIM2_IRQHandler            = blocking_handler
#pragma weak AES_IRQHandler             = blocking_handler
#pragma weak WDT_IRQHandler             = blocking_handler
#pragma weak SF_IRQHandler              = blocking_handler
#pragma weak QDEC_IRQHandler            = blocking_handler
#pragma weak POWER_DOWN_IRQHandler      = blocking_handler
#pragma weak ZIG_MAC_IRQHandler         = blocking_handler
#pragma weak ZIG_TIMER_IRQHandler       = blocking_handler
#pragma weak SOFT0_IRQHandler           = blocking_handler
#pragma weak MAC6200_RF_IRQHandler      = blocking_handler
#pragma weak MAC6200_SPI_IRQHandler     = blocking_handler
#pragma weak SOFT1_IRQHandler           = blocking_handler
#pragma weak SOFT2_IRQHandler           = blocking_handler
#pragma weak SOFT3_IRQHandler           = blocking_handler
#pragma weak SOFT4_IRQHandler           = blocking_handler
#pragma weak SOFT5_IRQHandler           = blocking_handler
#pragma weak SOFT6_IRQHandler           = blocking_handler
#pragma weak SOFT7_IRQHandler           = blocking_handler

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */
__attribute__ ((section(".vectors")))
vector_table_t vector_table = {
    .initial_sp_value = &_estack,
    .reset = reset_handler,
    .nmi = nmi_handler,
    .hard_fault = hard_fault_handler,
    .soft_fault = (vector_table_entry_t)soft_fault_handler,

/* Those are defined only on CM3 or CM4 */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    .memory_manage_fault = mem_manage_handler,
    .bus_fault = bus_fault_handler,
    .usage_fault = usage_fault_handler,
    .debug_monitor = debug_monitor_handler,
#endif

    .sv_call = sv_call_handler,
    .pend_sv = pend_sv_handler,
    .systick = SysTick_Handler,
    .irq = {
        BT_IRQHandler             , //0
        SLEEP_TIMER_IRQHandler    , //1
        DMA_IRQHandler            , //2
        GPIO_IRQHandler           , //3
        TIM_IRQHandler            , //4
        KPP_DEPRESS_IRQHandler    , //5
        KPP_RELEASE_IRQHandler    , //6
        blocking_handler          , //7
        UART0_IRQHandler          , //8
        UART1_IRQHandler          , //9
        I2C_IRQHandler            , //10
        PIN_WAKEUP_IRQHandler     , //11
        ADC_IRQHandler            , //12
        SPI0_IRQHandler           , //13
        SPI1_IRQHandler           , //14
        blocking_handler          , //15
        DMA0_IRQHandler           , //16
        DMA1_IRQHandler           , //17
        DMA2_IRQHandler           , //18
        DMA3_IRQHandler           , //19
        RTC_AF_IRQHandler         , //20
        RTC_1HZ_IRQHandler        , //21
        RTC_BLE_IRQHandler        , //22
        blocking_handler          , //23
        blocking_handler          , //24
        blocking_handler          , //25
        GPIO0_IRQHandler          , //26
        GPIO1_IRQHandler          , //27
        TIM0_IRQHandler           , //28
        TIM1_IRQHandler           , //29
        TIM2_IRQHandler           , //30
        AES_IRQHandler            , //31
        WDT_IRQHandler            , //32
        SF_IRQHandler             , //33
        QDEC_IRQHandler           , //34
        POWER_DOWN_IRQHandler     , //35
        ZIG_MAC_IRQHandler        , //36
        ZIG_TIMER_IRQHandler      , //37
        MAC6200_RF_IRQHandler     , //38
        MAC6200_SPI_IRQHandler    , //39
        SOFT0_IRQHandler          , //40
        SOFT1_IRQHandler          , //41
        SOFT2_IRQHandler          , //42
        SOFT3_IRQHandler          , //43
        SOFT4_IRQHandler          , //44
        SOFT5_IRQHandler          , //45
        SOFT6_IRQHandler          , //46
        SOFT7_IRQHandler          , //47
    }
};


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief reset_handler()
 *
 * @return 
 **/
void WEAK __attribute__ ((naked)) reset_handler(void)
{
    register uint32_t *dst, *src;

    // RAM version or gdb download needs override SP
    __set_MSP((uint32_t)&_estack);

//    system_init();

    for (src = &_data_loadaddr, dst = &_data;
        dst < &_edata;
        src++, dst++) {
        *dst = *src;
    }

    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call the application's entry point. */
    main();
}

/**
 * @brief null_handler()
 *
 * @return 
 **/
void null_handler(void)
{
    __disable_irq();
    while (1);
}

/**
 * @brief blocking_handler()
 *
 * @return 
 **/
void blocking_handler(void)
{
    __disable_irq();
    while (1);
}

/**
 * @brief hard_fault_handler()
 *
 * @return 
 **/
void WEAK hard_fault_handler(void)
{
    while (1);
}

/**
 * @brief soft_fault_handler()
 *
 * @return 
 **/
void WEAK soft_fault_handler(int code)
{
    __disable_irq();
    while (1);
}

/** @} */


