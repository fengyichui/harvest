/**
 * @file retarget.c
 * @brief 
 * @date 2016/04/15 19:35:33
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
#include <stdio.h>
#include "hs6620.h"

/*********************************************************************
 * MACROS
 */
#define RETARGET_UART  HS_UART1


/*********************************************************************
 * TYPEDEFS
 */
#if !defined(__ICCARM__)
struct __FILE 
{
    int handle;
};
#endif

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */
FILE __stdout;
FILE __stdin;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static char retarget_write(char ch)
{
//    while((RETARGET_UART->LSR & UART_LSR_THRE) == 0x00);
//    RETARGET_UART->THR = (uint8_t)ch;
    return ch;
}

static char retarget_read(void)
{
//    while(!(RETARGET_UART->LSR & UART_LSR_DR));
//    return RETARGET_UART->RBR;
    return '1';
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

#if defined(__CC_ARM) ||  defined(__ICCARM__)

int fgetc(FILE * p_file)
{
    return (int)retarget_read();
}

int fputc(int ch, FILE * p_file)
{
    if((char)ch == '\n')
        retarget_write('\r');
    retarget_write((char)ch);
    return ch;
}

#elif defined(__GNUC__)

#include <sys/stat.h>

// Default _sbrk just return 'end' value (defined in linker).
caddr_t _sbrk(int incr)
{
#ifdef CONFIG_STACK_DEBUG
    // In Stack Debug mode, can't use the real the stack for sbrk, it will effect co_stack_check()
    static uint8_t fake_stack[2048];
    uint8_t * const stack_base  = fake_stack;
    uint8_t * const stack_limit = fake_stack + sizeof(fake_stack);
#else
    extern unsigned _stack;
    uint8_t * const stack_base  = (uint8_t *)&_stack;
    uint8_t * const stack_limit = (uint8_t *)__get_MSP();
#endif

    static uint8_t* psbrk = stack_base;
    uint8_t*        prev  = psbrk;
    uint8_t*        next  = psbrk + incr;

    if (next >= stack_limit)
        return (caddr_t)-1;

    psbrk = next;

    return (caddr_t) prev;
}

int _write(int file, const char * p_char, int len)
{
    for(; len>0; --len, ++p_char)
    {
        if(*p_char == '\n')
            retarget_write('\r');
        retarget_write(*p_char);
    }
    return len;
}

int _read(int file, char * p_char, int len)
{
    *p_char = retarget_read();
    return 1;
}

#endif

/** @} */


