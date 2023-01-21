/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_port.C
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

#include "os_cpu.h"
#include "os_thread.h"
#include "os_core.h"

/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/**
 * @brief A counter for the number of times the critical section is locked.
 * @note Stored in the data area, the fastest reading and writing speed.
 **/
os_uint8_t xdata g_lock_cnt = 0;

/**
 * @brief The thread node that is running.
 **/
extern os_thread_t *g_thrd_run_node;

/**
 * @brief Thread ready linked list.
 **/
extern struct os_thread_list g_thrd_rdylist;

/**
 * @brief RTOS kernel stack.
 ***/
extern struct os_kernel_stk volatile idata kernel_stack;

os_uint8_t data len;

/**
 * @brief RTOS access external RAM pointer.
 **/
extern os_uint8_t xdata *data g_pxram_stk;

/**
 * @brief RTOS accesses the internal RAM pointer..
 **/
extern os_uint8_t idata *data g_pram_stk;

/*------------------------------------------------------------------------
|                              API FUNCTIONS                             |
------------------------------------------------------------------------*/

extern os_thread_t os_rdylist_get_max_prio(os_uint8_t *max_prio_label);
extern void os_rdylist_replace(os_uint8_t index1, os_uint8_t index2);
extern void os_update_list(os_thread_t *thread, os_uint8_t status);
/**
 * @brief   Copy the core stack (RAM) to the thread stack (XRAM).
 **/
#define COPY_STACK_TO_XRAM()                        \
    do                                              \
    {                                               \
        g_pxram_stk = (*g_thrd_run_node)->stk_addr; \
        g_pram_stk = kernel_stack.block;            \
        len = SP - (os_uint8_t)kernel_stack.block;  \
        *g_pxram_stk = len;                         \
        while (len--)                               \
        {                                           \
            *(++g_pxram_stk) = *(++g_pram_stk);     \
        }                                           \
    } while (0)

/**
 * @brief  Copy the thread stack (XRAM) to the core stack (RAM).
 **/
#define COPY_XRAM_TO_STACK()                        \
    do                                              \
    {                                               \
        g_pxram_stk = (*g_thrd_run_node)->stk_addr; \
        g_pram_stk = kernel_stack.block;            \
        len = g_pxram_stk[0];                       \
        while (len--)                               \
        {                                           \
            *(++g_pram_stk) = *(++g_pxram_stk);     \
        }                                           \
        SP = (os_uint8_t)(g_pram_stk);              \
    } while (0)

/**
 * @brief     OS starts system scheduling and selects the highest priority task for execution.
 * @param[in] None.
 * @return    None.
 **/
void os_scheduler_init(void)
{
    os_uint8_t index;

    g_thrd_run_node = (os_thread_t *)os_rdylist_get_max_prio(&index);
    os_rdylist_replace(index, --g_thrd_rdylist.num);
    (*g_thrd_run_node)->status = OS_THREAD_RUNNING;

    COPY_XRAM_TO_STACK();

    #pragma asm
    POP AR3
    POP AR2
    POP AR1
    POP AR7
    POP AR6
    POP AR5
    POP AR4
    POP AR0
    POP PSW
    POP DPL
    POP DPH
    POP B
    POP ACC
    #pragma endasm

    OS_EXIT_CRITICAL();
}

/**
 * @brief RTOS Context Schedule.
 * @param[in] none
 * @return none
 * @attention this is a atomic operation,
 * You need to provide critical protection if you want to call it
 * @example
 * OS_ENTER_CRITICAL();
 * os_thread_schedule();
 * OS_EXIT_CRITICAL();
 **/
void os_thread_schedule(os_uint8_t status)
{
    os_uint8_t index;
    OS_ENTER_CRITICAL();
    #pragma asm
    PUSH ACC
    PUSH B
    PUSH DPH
    PUSH DPL
    PUSH PSW
    MOV  PSW, #00H
    PUSH AR0
    PUSH AR4
    PUSH AR5
    PUSH AR6
    PUSH AR7
    PUSH AR1
    PUSH AR2
    PUSH AR3
    #pragma endasm
    COPY_STACK_TO_XRAM();

    os_update_list(g_thrd_run_node, status);
    g_thrd_run_node = (os_thread_t *)os_rdylist_get_max_prio(&index);
    os_rdylist_replace(index, --g_thrd_rdylist.num);
    (*g_thrd_run_node)->status = OS_THREAD_RUNNING;

    COPY_XRAM_TO_STACK();
    #pragma asm
    POP AR3
    POP AR2
    POP AR1
    POP AR7
    POP AR6
    POP AR5
    POP AR4
    POP AR0
    POP PSW
    POP DPL
    POP DPH
    POP B
    POP ACC
    #pragma endasm
    OS_EXIT_CRITICAL();
}

/**
 * @brief     MCU TIMER Interrupt request service function
 * @param[in] None
 * @return    None
 **/
void os_update_ticks_handle(void)
{
    os_uint8_t index;   /*!< Count variable of sleeping threads */
    os_uint8_t cnt = 0; /*!< Count variable for the end of the sleeping threads */

    OS_ENTER_CRITICAL();
    COPY_STACK_TO_XRAM();

    if (g_thrd_slplist.num != 0)
    {
        for (index = 0; index < OS_THREAD_MAX_NUM; index++)
        {
            if (g_thrd_slplist.node[index] != OS_NULL &&
                (--((os_thread_t)*g_thrd_slplist.node[index])->slpctr == 0))
            {
                /** If there are tasks whose sleep time has expired, they are added to the linked list */
                ((os_thread_t)*g_thrd_slplist.node[index])->status = OS_THREAD_READY;
                g_thrd_rdylist.node[g_thrd_rdylist.num++] = g_thrd_slplist.node[index];
                g_thrd_slplist.node[index] = OS_NULL;
                cnt++;
            }
        }
    }
    g_thrd_slplist.num -= cnt;

    /** Thread scheduling occurs immediately if a task completes sleep or if a higher priority task is ready */
    if (cnt && (*(os_thread_t *)os_rdylist_get_max_prio(OS_NULL))->priority > (*g_thrd_run_node)->priority)
    {
        os_update_list(g_thrd_run_node, OS_THREAD_READY);
        g_thrd_run_node = (os_thread_t *)os_rdylist_get_max_prio(&index);
        os_rdylist_replace(index, --g_thrd_rdylist.num);
        (*g_thrd_run_node)->status = OS_THREAD_RUNNING;
    }

    COPY_XRAM_TO_STACK();
    OS_EXIT_CRITICAL();
}

/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/
