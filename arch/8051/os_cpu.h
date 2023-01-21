/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_cpu.h
|  - Author        : zevorn
|  - Update date   : 2021.03.25
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
#ifndef __OS_CPU_H_
#define __OS_CPU_H_

#include "os_type.h"

/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/**
 * @brief   临界区上锁次数计数器。
 * @details A counter for the number of times the critical section is locked.
 * @note    存放在data区，读写速度最快。
 **/
extern os_uint8_t xdata g_lock_cnt;

/*------------------------------------------------------------------------
|                              API FUNCTIONS                             |
------------------------------------------------------------------------*/

/**
 * @brief   供给系统心跳的定时器宏。
 * @details Provides the timer interrupt label for the heartbeat of the system.
 **/
#define os_update_ticks_handle(void) OS_TIMERx_Handle(void) interrupt 1

/**
 * @brief   进入临界区（CPU进入内核态）。
 * @details Enter the critical section (CPU enters the kernel mode).
 **/
#define OS_ENTER_CRITICAL() \
    do                      \
    {                       \
        g_lock_cnt++;       \
        EA = 0;             \
    } while (0)

/**
 * @brief   退出临界区（CPU退出内核态）。
 * @details Exit the critical section (CPU exits kernel mode)
 **/
#define OS_EXIT_CRITICAL()   \
    do                       \
    {                        \
        if (!(--g_lock_cnt)) \
        {                    \
            EA = 1;          \
        }                    \
    } while (0)

/**
 * @brief   RTOS start runing.
 * @param   None
 * @retval  None
 **/
extern void os_thread_schedule(os_uint8_t status);

/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/
#endif