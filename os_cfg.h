/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_config.h
|  - Author        : zevorn
|  - Update date   : 2021.05.07
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
|                                CONFIG                                  |
------------------------------------------------------------------------*/
#ifndef __OS_CONFIG_H_
#define __OS_CONFIG_H_

#include "STC8Hx_REG.h"
/**
 * @brief The kernel stack size, the maximum allowed is 256 byte
 *        The thread stack cannot exceed this size
**/
#define    OS_KERNEL_STK_SIZE   (128)

/**
 * @brief The idle thread stack size, the maximum allowable 
 *        OS_KERNEL_STK_SIZE bytes
**/
#define    OS_IDLE_THREAD_STK_SIZE   (64)


/**
 * @brief The size of the system memory pool the maximum cannot exceed the size of XRAM
 * @note  The smallest unit of the memory pool is a memory block, 
 *        which consists of a message header + memory, 
 *        and the message header occupies 5 bytes.
 *        Suppose you have applied for 10 bytes of memory, 
 *        which actually occupies 15 bytes, remember.
**/
#define    OS_MEM_POOL_SIZE   (256*2)


/**
 * @brief  The number of threads can be up to 31. 
 * @note   If the software timer is enabled, there are only 30.
**/
#define    OS_THREAD_MAX_NUM    (3)


/**
 * @brief  Software timer switch, write 1 to open, write 0 to close.
**/
#define    OS_SOFT_TIMER_CTRL    (0)


/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/

#endif