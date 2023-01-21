/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_core.c
|  - Author        : zevorn
|  - Update date   : 2021.10.26
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

#include "os_core.h"
#include "os_cpu.h"
#include "os_mem.h"
#include "os_thread.h"
#include "os_object.h"

/*------------------------------------------------------------------------
|                               DECLARATION                              |
------------------------------------------------------------------------*/

/**
 * @brief  Initialize idle threads.
 *         It can only be called within this file.
 **/
#define OS_IDLE_THREAD_INIT()                             \
    do                                                    \
    {                                                     \
        os_create_thread_static(g_idle_thread_handle,     \
                                antos_idle_thread_entry,  \
                                OS_NULL,                  \
                                0,                        \
                                g_idle_stack,             \
                                OS_IDLE_THREAD_STK_SIZE); \
    } while (0)

/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/**
 * @brief Idle thread stack.
 **/
extern os_thread_stk g_idle_stack[OS_IDLE_THREAD_STK_SIZE];

extern os_thread_t os_rdylist_get_max_prio(os_uint8_t *max_prio_label);

extern void os_rdylist_replace(os_uint8_t index1, os_uint8_t index2);

extern void os_update_list(os_thread_t *thread, os_uint8_t status);

/**
 * @brief   RTOS kernel stack.
 ***/
struct os_kernel_stk volatile idata kernel_stack;

/**
 * @brief  RTOS access external RAM pointer.
 **/
os_uint8_t xdata *data g_pxram_stk;

/**
 * @brief  RTOS accesses the internal RAM pointer..
 **/
os_uint8_t idata *data g_pram_stk;

/**
 @brief  System abnormality code, save the code value
 *       corresponding to the problem when the system is abnormal.
**/
static os_uint8_t G_Exc_Code = 0;

/**
 * @brief  RTOS idle task handle function.
 **/
os_idle_user_handle_t os_idle_excute = OS_NULL;

/**
 @brief  The pointer handle of the idle thread.
**/
os_thread_t g_idle_thread_handle;

/*------------------------------------------------------------------------
|                              API FUNCTIONS                             |
------------------------------------------------------------------------*/

/**
 * @brief  OS exception handling function.
 * @param[in]  excCode 异常代号.Exception code.
 * @return  None.
 * @note  When there is an error in the OS operation,
 *        this exception handling function is called back to indicate the error.
 **/
void os_exce_handle(os_uint8_t excCode)
{
    OS_ENTER_CRITICAL();
    G_Exc_Code = excCode;
    while (1)
        ;
}

/**
 * @brief     Idle thread, used for task scheduling, is created by default.
 * @param[in] None.
 * @return    None.
 **/
void antos_idle_thread_entry(void)
{
    extern os_err_t os_mem_reco(void);
    for (;;)
    {
        /* If user will be use idle task handle function, run it.*/
        if (os_idle_excute)
            os_idle_excute();
        // os_mem_reco();
    }
}

/**
 * @brief     OS initializes the st
 *            artup code, and this function is executed before main
 * @param[in] none
 * @retval    [OS_OK]  AntOS initailized successfully
 * @retval    [OS_NOK] AntOS initailized failed
 **/
int os_startup_init(void)
{
    extern void os_scheduler_init(void);
    extern os_thread_t os_rdylist_get_max_prio(os_uint8_t * max_prio_label);
    extern os_err_t os_object_container_init(void);
    extern os_err_t os_mem_init(void);

    antos_adf_init();    /*!< 初始化系统板极支持包，需要用户自己实现这个同名函数。*/
    OS_ENTER_CRITICAL(); /*!< 进入临界区 */
    os_mem_init();       /*!< 初始化系统内存管理 */
    os_object_container_init();
    OS_IDLE_THREAD_INIT(); /*!< 初始化并创建空闲线程 */
    antos_main();          /*!< 调用OS的main函数 */
    os_scheduler_init();   /*!< 初始化线程调度器 */

    return OS_OK;
}

/**
 * @brief   main
 * @param   None
 * @return  [OS_NOK] AntOS run failed
 **/
int main(void)
{
    os_exce_handle(EXC_CODE_ILLEGAL_JUMP);
    return OS_NOK;
}

/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/