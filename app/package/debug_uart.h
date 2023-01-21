#ifndef	__DEBUG_UART_H
#define	__DEBUG_UART_H

#include	"main.h"

void debug_uart_init(void);
void debug_print(u8 *puts);
void debug_print_char(u8 dat);

#endif