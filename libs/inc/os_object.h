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
 * @brief     ???????????????????????????????????????
 * @details   Kernel object structure, pointer type. 
**/
typedef struct os_object *os_object_t;

/**
 * @brief    ????????????????????????
 * @details  Object type enumerator.
**/
enum os_object_type
{
     OS_Object_Class_Thread = 0,       /*!< ??????????????? */
     OS_Object_Class_Semaphore,        /*!< ?????????????????? */
     OS_Object_Class_Mutex,            /*!< ?????????????????? */
     OS_Object_Class_MailBox,          /*!< ??????????????? */
     OS_Object_Class_MessageQueue,     /*!< ????????????????????? */
     OS_Object_Class_MemPool,          /*!< ?????????????????? */
     OS_Object_Class_Device,           /*!< ??????????????? */
     OS_Object_Class_Timer,            /*!< ?????????????????? */
     OS_Object_Class_Module,           /*!< ??????????????? */
     OS_Object_Class_Unknown           /*!< ???????????? */
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
 * @brief     ???????????????????????????????????????
 * @details   Gets the number of threads in the object container.
 * @param[in] None.
 * @return    ?????????????????????????????????The number of threads in the object container.
**/
extern os_uint8_t os_object_get_thrd_num(void);

/**
 * @brief Gets the list of threads in the object container
 * @param[in] none
 * @return The list of threads in the object container
**/
extern os_object_t * os_object_get_thrd_list(void);

/**
 * @brief     ????????????,?????????????????????????????????
 * @details   Delete the object and detach it from the object container.
 * @param[in] type   ???????????????The type of object.
 * @return    [OS_OK]  ?????????????????????The object is detached successfully.
 * @return    [OS_NOK] ?????????????????????The object detach failed.
**/
extern os_err_t os_object_thread_delete(os_object_t *object);


/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/
#endif