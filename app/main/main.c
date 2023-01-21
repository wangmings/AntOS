/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_task.c
|  - Author        : zevorn
|  - Update date   : 2021.09.12
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
/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "main.h"
#include "antos.h"
/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/

#define THR_STACK_SIZE (32)

os_thread_t thread1; //定义线程1的指针句柄
os_thread_t thread2; //定义线程2的指针句柄
os_thread_t thread3; //定义线程3的指针句柄

os_thread_stk_t stack1_addr; //定义线程1的堆栈指针句柄
os_thread_stk_t stack2_addr; //定义线程2的堆栈指针句柄
os_thread_stk_t stack3_addr; //定义线程3的堆栈指针句柄

/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/

/**
 * @brief     线程1的入口函数。
 * @details   Entry function of thread 5.
 * @param[in] param 线程5的入口参数。
 * @return    None
 **/
void thread1_entry(void *param)
{
    param = param;

    while (1)
    {
        GPIO_FLIP_PIN_LEVEL(GPIO_P6, Pin_1);
        os_thread_sleep(2);
    }
}

/**
 * @brief     线程2的入口函数。
 * @details   Entry function of thread 5.
 * @param[in] param 线程5的入口参数。
 * @return    None
 **/
void thread2_entry(void *param)
{
    param = param;

    while (1)
    {
        GPIO_FLIP_PIN_LEVEL(GPIO_P6, Pin_2);
        os_thread_sleep(2);
    }
}

/**
 * @brief     线程3的入口函数。
 * @details   Entry function of thread 5.
 * @param[in] param 线程5的入口参数。
 * @return    None
 **/
void thread3_entry(void *param)
{
    param = param;

    while (1)
    {
        GPIO_FLIP_PIN_LEVEL(GPIO_P6, Pin_3);
        os_thread_sleep(10);
    }
}

/**
 * @brief   OS提供的主函数，用于注册线程和其他操作。
 * @details The main function provided by OS
 *          is used to register threads.
 * @param   None
 * @return  None
 **/
void antos_main(void)
{
    static os_uint8_t i = 0;

    i = 1;
    P40 = 0;
    os_create_thread(thread1,         /*!< 线程1的指针句柄，不可重复。 */
                     thread1_entry,   /*!< 线程1入口函数地址。 */
                     &i,              /*!< 线程1的传参。 */
                     1,               /*!< 线程1优先级。 */
                     stack1_addr,     /*!< 返回线程1的堆栈地址。 */
                     THR_STACK_SIZE); /*!< 线程1堆栈大小。 */

    os_create_thread(thread2, thread2_entry, OS_NULL, 2, stack2_addr, THR_STACK_SIZE);

    os_create_thread(thread3, thread3_entry, OS_NULL, 3, stack3_addr, THR_STACK_SIZE);
}

/**
 * @name    STC8x_System_Init
 * @brief   MCU initialization function
 * @param   None
 * @return  None
 ***/
void antos_adf_init(void)
{
    void STC8x_SYSCLK_Config(void);
    void STC8x_UART_Config(void);
    void STC8x_GPIO_Config(void);
    void STC8x_TIMER_Config(void);

    // STC8x_SYSCLK_Config(); /* Initialize system clock */

    STC8x_GPIO_Config();
    STC8x_TIMER_Config();

    // STC8x_UART_Config();

    /*
        Add hardware driver initialization code here.
    */

    // NVIC_GLOBAL_ENABLE();
}

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT zevorn                  |
-----------------------------------------------------------------------*/
