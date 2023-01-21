/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_task.h
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
/*------------------------------------------------------------------------
|                                INCLUDES                                |
------------------------------------------------------------------------*/
#ifndef __OS_TASK_H_
#define __OS_TASK_H_

#include "os_type.h"
#include "os_object.h"

/*------------------------------------------------------------------------
|                                  MACRO                                 |
------------------------------------------------------------------------*/


/**
 * @brief  The thread is in the running state macro code. 
**/
#define OS_THREAD_RUNNING   0x04

/**
 * @brief  The thread is in the ready state macro code. 
**/
#define OS_THREAD_READY     0x00

/**
 * @brief  The thread is in the sleep state macro code. 
**/
#define OS_THREAD_SLEEP     0x01

/**
 * @brief  The thread is in the blocking state macro code. 
**/
#define OS_THREAD_BLOCK     0x02

/**
 * @brief  The thread is in the suspended state macro code. 
**/
#define OS_THREAD_SUSPEND   0x02


#if (OS_SOFT_TIMER_CTRL == 0)

	/**
	 * @brief  The length of the thread list is macro, 
	 *         plus 1 (idle thread) on the original basis.
	**/
	#define    OS_THREAD_REAL_LEN  (OS_THREAD_MAX_NUM + 1)
	
#else

	/**
	 * @brief  The length of the thread list is macro, 
	 *         plus 1 (idle thread) on the original basis.
	**/
	#define    OS_THREAD_REAL_LEN  (OS_THREAD_MAX_NUM + 2)

#endif


/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/


/**
 * @brief  Thread control block structure (object).
**/
struct os_thread
{
	struct os_object attribute; /* Object parent class */
	void             *entry;    /* Thread entry function pointer */
	os_uint8_t       priority; /* Thread priority */
	os_uint8_t       status;   /* Thread running status */
	os_uint16_t      slpctr;   /* Thread sleep counter */
	os_thread_stk_t  stk_addr;  /* Thread stack address */
	os_uint8_t      stk_size;  /* Thread stack size */
};


/**
 * @brief  Thread control block structure (object), pointer type.
**/
typedef struct os_thread *os_thread_t;

/**
 * @brief Thread linked list structure.
**/
struct os_thread_list
{
	volatile void **node[OS_THREAD_REAL_LEN];
	os_vuint8_t num;
};

/**
 * @brief Thread queue structure (object), pointer type.
**/
typedef struct os_thread_list *os_thread_list_t;

/**
 * @brief  The weight definition of the dormant task linked list structure.
**/
typedef void (*  os_idle_user_handle_t) (void);

/**
 * @brief  The thread node that is running.
**/
extern os_thread_t *g_thrd_run_node;

/**
 * @brief  Thread ready linked list.
**/
extern struct os_thread_list g_thrd_rdylist;

/**
 * @brief  Thread sleep linked list queue.
**/
extern struct os_thread_list g_thrd_slplist;

/*------------------------------------------------------------------------
|                              API FUNCTIONS                             |
------------------------------------------------------------------------*/

/**
 * @brief  Create a thread, and return the handle address of the thread object if it succeeds
 * @param[in]  thread Thread pointer handle, not repeatable
 * @param[in]  entry Thread entry addres
 * @param[in]  param Thread entry parameters
 * @param[in]  priority Thread priority
 * @param[in]  stk_addr Thread stack address
 * @param[in]  stk_size Thread stack size
 * @param[in]  slice Thread running time slice, the unit is tick
 * @return  Return thread entity
 * @retval  [os_thread_t] The thread is initialized successfully and the address is returned
 * @retval  [OS_NULL] Creation failure returns NULL
 * @note  To create a thread statically, the user needs to define a thread stack 
 *        and thread control block pointer (optional),The pointer is mainly used for thread control, 
 *        such as suspend and delete.
 * @par example:
 * @code
    os_thread_t pthread;
    os_thread_stk_t Stack_Addr;
    pthread = os_thread_static_create(&pthread, 1,thread_entry, OS_NULL, 1, Stack_Addr, THR_STACK_SIZE);
    void thread_entry(void *param)
    {
        for(;;){};
    }
 * @endcode
**/
extern os_thread_t os_thread_static_create(os_thread_t *pthread, void(*entry)(void *param), void *param,
                                      os_uint8_t priority, os_thread_stk_t stk_addr, os_uint8_t stk_size);

/**
 * @brief Create a thread macro function statically, 
 *        and return the handle address of the thread object if it succeeds
 * @param[in] thread    Thread pointer handle, not repeatable
 * @param[in] entry     Thread entry addres
 * @param[in] param     Thread entry parameters
 * @param[in] priority  Thread priority
 * @param[in] stk_addr   Thread stack address
 * @param[in] stk_size   Thread stack size
 * @retval [os_thread_t] The thread is initialized successfully and the address is returned
 * @retval [OS_NULL]     Creation failure returns NULL
 * @note To create a thread statically, the user needs to define a thread stack 
 *       and thread control block pointer (optional),The pointer is mainly used for thread control, 
 *       such as suspend and delete
 * @par example:
 * @code
    os_thread_t pthread;
    os_thread_stk_t Stack_Addr;
    os_create_thread_static(&pthread, 1,thread_entry, OS_NULL, 1, Stack_Addr, THR_STACK_SIZE);
    void thread_entry(void *param)
    {
        for(;;){};
    }
 * @endcode
**/
#define os_create_thread_static(pthread, \
                                entry, \
                                param, \
                                priority, \
                                stk_addr, \
                                stk_size) \
do{ \
	pthread = os_thread_static_create(&pthread, \
                                      entry, \
                                      param, \
                                      priority, \
                                      stk_addr, \
                                      stk_size); \
}while(0)


/**
 * @brief Create a thread, and return the handle address of the thread object if it succeeds
 * @param[in] thread    Thread pointer handle, not repeatable
 * @param[in] entry     Thread entry address
 * @param[in] param     Thread entry parameters
 * @param[in] priority  Thread priority
 * @param[in] stk_addr   Thread stack address
 * @param[in] stk_size   Thread stack size
 * @retval [os_thread_t] The thread is initialized successfully and the address is returned
 * @retval [OS_NULL]     The thread initialization failed
 * @note To dynamically create threads, users need to define a thread stack pointer
 *       and thread control block pointer (optional),
 *       The pointer is mainly used for thread control, such as suspend and delete
**/
extern os_thread_t os_thread_create(os_thread_t *pthread,void(*entry)(void *param), void *param,
                             os_uint8_t priority, os_thread_stk_t *stk_addr, os_uint8_t stk_size);

/**
 * @brief  Create a thread macro function statically, 
 *         and return the handle address of the thread object if it succeeds.
 * @param[in]  thread     Thread pointer handle, not repeatable.
 * @param[in]  entry      Thread entry address.
 * @param[in]  param      Thread entry parameters.
 * @param[in]  priority   Thread priority
 * @param[in]  stk_addr    Thread stack address.
 * @param[in]  stk_size    Thread stack size.
**/
#define os_create_thread(pthread, \
                         entry, \
                         param, \
                         priority, \
                         stk_addr, \
                         stk_size) \
do{ \
	pthread = os_thread_create(&pthread, \
                               entry, \
                               param, \
                               priority, \
                               &stk_addr, \
                               stk_size); \
}while(0)
									   
/**
 * @brief The thread gives up the right to use the CPU and enters the ready state
 * @param[in] none
 * @retval [OS_OK]  Thread yields successfully
 * @retval [OS_NOK] Thread yield failure
 * @note  If the priority of the operated thread is the highest in the ready queue,
 *        it will be scheduled again
**/
extern os_err_t os_thread_yield(void);


/**
 * @brief     休眠一个线程（只能是自身），被休眠的线程主动让出CPU使用权。
 * @details   When a thread is hibernated, the hibernated thread voluntarily surrenders 
 *            the right to use the CPU.
 * @param[in] ticks 系统节拍数。The number of system beats.
 * @return    [OS_OK]  线程休眠成功。 The thread is sleep successfully.
 * @return    [OS_NOK] 线程休眠失败。 Thread sleep failure.
**/
extern os_err_t os_thread_sleep(os_uint16_t ticks);


/**
 * @brief  Delete a process. This process can be itself or other threads.
 * @param[in] *thread  Thread pointer handle.
 * @param[in] *stk_addr Thread stack pointer.
 * @retval  [OS_OK] The thread is deleted successfully.
 * @retval [OS_NOK] The thread delete failed.
 * @note  If you delete itself, it will trigger a task schedule
**/
extern os_err_t os_thread_delete(os_thread_t *thread,os_thread_stk_t *stk_addr);

/**
 * @brief     挂起一个线程，可以是自身，也可以是其他线程。
 * @details   Suspend a process. This process can be itself or other threads.
 * @param[in]     thread   要被挂起的线程。Thread to be suspended.
 * @return    [OS_OK]  线程挂起成功。 The thread is suspended successfully.
 * @return    [OS_NOK] 线程挂起失败。 The thread suspend failed.
 * @note      如果挂起自身，那么将会触发一次任务调度。
 *            If you suspend itself, it will trigger a task schedule
**/
extern os_err_t os_thread_suspend(os_thread_t *thread);


/**
 * @brief     恢复一个线程，不可以是自身，只能是其他线程。
 * @details   Resume a process. This process can be itself or other threads.
 * @param[in] thread   要被恢复线程。Thread to be resumed.
 * @return    [OS_OK]  线程恢复成功。 The thread is resumeed successfully.
 * @return    [OS_NOK] 线程恢复失败。 The thread resume failed.
**/
extern os_err_t os_thread_resume(os_thread_t *thread);


#endif
/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/