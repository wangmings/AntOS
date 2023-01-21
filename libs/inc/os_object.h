/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_object.h
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
|                                 INCLUDE                                |
------------------------------------------------------------------------*/
#ifndef __OS_OBJECT_H_
#define __OS_OBJECT_H_

#include "os_type.h"

/*------------------------------------------------------------------------
|                                  MACRO                                 |
------------------------------------------------------------------------*/


/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/**
 * @brief  Kernel object structure, the core type of the kernel 
 *         object-oriented architecture. 
**/
struct os_object
{
	os_uint8_t id;    /*!< Object id. */
	os_uint8_t type;  /*!< Object type. */
};

/**
 * @brief     内核对象结构体，指针类型。
 * @details   Kernel object structure, pointer type. 
**/
typedef struct os_object *os_object_t;

/**
 * @brief    对象类型枚举体。
 * @details  Object type enumerator.
**/
enum os_object_type
{
     OS_Object_Class_Thread = 0,       /*!< 对象是线程 */
     OS_Object_Class_Semaphore,        /*!< 对象是信号量 */
     OS_Object_Class_Mutex,            /*!< 对象是互斥量 */
     OS_Object_Class_MailBox,          /*!< 对象是邮箱 */
     OS_Object_Class_MessageQueue,     /*!< 对象是消息队列 */
     OS_Object_Class_MemPool,          /*!< 对象是内存池 */
     OS_Object_Class_Device,           /*!< 对象是设备 */
     OS_Object_Class_Timer,            /*!< 对象是定时器 */
     OS_Object_Class_Module,           /*!< 对象是模块 */
     OS_Object_Class_Unknown           /*!< 对象未知 */
};

/*------------------------------------------------------------------------
|                              API FUNCTIONS                             |
------------------------------------------------------------------------*/


/**
 * @brief Initialize the object and mount it on the object container.
 * @param[in] object The address of the object.
 * @param[in] type   The type of object.
 * @retval [OS_OK]  The object is initialized successfully.
 * @retval [OS_NOK] The object initialization failed.
**/
extern os_err_t os_object_thread_init(os_object_t *object, enum os_object_type type);

/**
 * @brief     获取对象容器中线程的数量。
 * @details   Gets the number of threads in the object container.
 * @param[in] None.
 * @return    对象容器里线程的数量。The number of threads in the object container.
**/
extern os_uint8_t os_object_get_thrd_num(void);

/**
 * @brief Gets the list of threads in the object container
 * @param[in] none
 * @return The list of threads in the object container
**/
extern os_object_t * os_object_get_thrd_list(void);

/**
 * @brief     删除对象,将其从对象容器上脱离。
 * @details   Delete the object and detach it from the object container.
 * @param[in] type   对象类型。The type of object.
 * @return    [OS_OK]  对象删除成功。The object is detached successfully.
 * @return    [OS_NOK] 对象删除失败。The object detach failed.
**/
extern os_err_t os_object_thread_delete(os_object_t *object);


/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/
#endif