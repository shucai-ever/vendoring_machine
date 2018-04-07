#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "stm32f10x_cfg.h"
#include "dbgserial.h"
#include "global.h"

/**
 * @brief init debug serial port
 */
void dbg_serial_setup(void)
{
    USART_Config config;
    USART_StructInit(&config);
    
    NVIC_Config nvicConfig = {USART1_IRQChannel, USART1_PRIORITY, 0, TRUE};
    USART_Setup(USART1, &config);
    USART_EnableInt(USART1, USART_IT_RXNE, TRUE);
    NVIC_Init(&nvicConfig);
    USART_Enable(USART1, TRUE);
}

/**
 * @brief put char
 * @param data - data to put
 */
void dbug_putchar(char data)
{
    USART_WriteData_Wait(USART1, data);
}

/**
 * @brief put string
 * @param string - string to put
 * @param length - string length
 */
void dbg_putstring(const char *string, uint32_t length)
{
    const char *pNext = string;
    while(length--)
        dbug_putchar(*pNext++);
}


#ifdef __DEBUG
void assert_failed(const char *file, const char *line, const char *exp)
{
    dbg_putstring("assert failed: ", 15);
    dbg_putstring(file, strlen(file));
    dbg_putstring(":", 1);
    dbg_putstring(line, strlen(line));
    dbg_putstring("(", 1);
    dbg_putstring(exp, strlen(exp));
    dbg_putstring(")\n", 2);
    while(1);
}
#endif

#ifdef __ENABLE_TRACE
void trace(const char *fmt, ...)
{
    char buf[64];
    va_list argptr;
    int cnt;
    va_start(argptr, fmt);
    cnt = vsprintf(buf, fmt, argptr);
    va_end(argptr);
    dbg_putstring(buf, cnt);
}
#endif



