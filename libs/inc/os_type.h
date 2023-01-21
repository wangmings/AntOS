/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_type.h
|  - Author        : zevorn
|  - Update date   : 2021.05.05
|  - Copyright(C)  : 2021-2021 zevorn. All rights reserved.
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, zevorn (zevorn@yeah.net)

 * This file is part of Ant Real Time Operating System.

 * Ant Real Time Operating System is free software: you can redistribute 
 * it and/or modify it under the terms of the Apache-2.0 License.

 * Ant Real Time Operating System is distributed in the hope that it will 
 * be useful,but WITHOUT ANY WARRANTY; without even the implied warranty 
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * Apache-2.0 License License for more details.

 * You should have received a copy of the Apache-2.0 License.Ant Real Time
 * Operating System. If not, see <http://www.apache.org/licenses/>.
**/

/*------------------------------------------------------------------------
|                                INCLUDES                                |
------------------------------------------------------------------------*/
#ifndef __OS_TYPE_H_
#define __OS_TYPE_H_

#include "os_cfg.h"

/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/*--------------------------------------------------------
| @Description: OS constant define types                 |
--------------------------------------------------------*/

typedef unsigned int     os_size_t;

typedef  bit     os_bool_t;

/*--------------------------------------------------------
| @Description: Exact width signed integer types         |
--------------------------------------------------------*/

typedef    signed char     os_int8_t;	//  8 bits 

typedef    signed int      os_int16_t;	// 16 bits 

typedef    signed long     os_int32_t;	// 32 bits 

/*--------------------------------------------------------
| @Description: Volatile width signed integer types      |
--------------------------------------------------------*/

typedef    volatile os_int8_t     os_vint8_t;	//  8 bits 

typedef    volatile os_int16_t    os_vint16_t;	// 16 bits 

typedef    volatile os_int32_t    os_vint32_t;	// 32 bits 


/*--------------------------------------------------------
| @Description: Exact width unsigned integer types       |
--------------------------------------------------------*/

typedef     unsigned char    os_uint8_t;	//  8 bits 

typedef     unsigned int     os_uint16_t;	// 16 bits 

typedef     unsigned long    os_uint32_t;	// 32 bits 


/*--------------------------------------------------------
| @Description: Volatile width unsigned integer types    |
--------------------------------------------------------*/

typedef volatile os_uint8_t    os_vuint8_t;	//  8 bits 

typedef volatile os_uint16_t   os_vuint16_t;	// 16 bits 

typedef volatile os_uint32_t   os_vuint32_t;	// 32 bits 

/*--------------------------------------------------------
| @Description: pointer types                            |
--------------------------------------------------------*/

typedef int    os_intptr_t;            //16 bits 

typedef unsigned int    os_uintptr_t;  //16 bits 

#define    OS_NULL     ((void *) 0)

/*--------------------------------------------------------
| @Description: OS constant define types                 |
--------------------------------------------------------*/

#define    OS_OK     (0U)

#define    OS_NOK    (1U)

#define    OS_TRUE    (1U)

#define    OS_FALSE    (0U)

#define    OS_ERROR    (os_uint8_t)(-1)

#define   os_err_t    os_uint8_t


/*--------------------------------------------------------
| @Description: System stack redefinition                |
--------------------------------------------------------*/

typedef xdata os_uint8_t os_thread_stk;

typedef os_thread_stk *os_thread_stk_t;
struct os_kernel_stk
{
	os_uint8_t length;
	os_uint8_t block[OS_KERNEL_STK_SIZE];
};

/*--------------------------------------------------------
| @Description: System exception code                    |
--------------------------------------------------------*/

/**
 * @brief     非法跳转的异常代号。
 * @details   The exception code of the illegal jump.
**/
#define     EXC_CODE_ILLEGAL_JUMP  (0x06U)


#endif
/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/