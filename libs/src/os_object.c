/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_object.c
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

#include "os_object.h"
#include "os_cpu.h"
#include "os_mem.h"

#if (OS_SOFT_TIMER_CTRL == 0)
/**
 * @brief  The length of the thread list is macro,
 *         plus 1 (idle thread) on the original basis.
 **/
#define OS_THREAD_LIST_LEN (OS_THREAD_MAX_NUM + 1)
#elif
/**
 * @brief  The length of the thread list is macro,
 *         plus 1 (idle thread) on the original basis.
 **/
#define OS_THREAD_LIST_LEN (OS_THREAD_MAX_NUM + 2)
#endif

#define os_object_ctn_THREAD_REPLACE(index1, index2)                             \
do                                                                               \
{                                                                                \
    (*os_object_ctn).thread.list[index1] = (*os_object_ctn).thread.list[index2]; \
    (os_object_t *)(*os_object_ctn).thread.list[index2] = OS_NULL;               \
} while (0)

/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/**
 * @brief  Object information container structure.
 * @note  Each member of the type array corresponds to the member of
 *        the Threaded list object in the structure.
 *        Example: type[0] <--> os_object_thread_List.
 **/
struct os_object_info
{
    struct object
    {
        os_object_t *list[OS_THREAD_LIST_LEN]; /*!< 线程链表 */
        os_uint8_t num;                        /*!< 线程挂载的数量 */
    } thread;
};

/**
 * @brief  Object information container structure,pointer type.
 **/
typedef struct os_object_info *os_object_info_t;

/**
 * @brief  The kernel object container, all object information, is hung on it.
 **/
static volatile os_object_info_t os_object_ctn;

/*------------------------------------------------------------------------
|                              API FUNCTIONS                             |
------------------------------------------------------------------------*/

/**
 * @brief  Initialize the object container
 * @param[in]  none
 * @retval  [OS_OK] The container is initialized successfully
 * @retval  [OS_NOK] The container initialization failed
 **/
os_err_t os_object_container_init(void)
{
    os_object_ctn = os_malloc((void *)os_object_ctn, sizeof(struct os_object_info));
    return OS_OK;
}

/**
 * @brief Initialize the object and mount it on the object container.
 * @param[in] object The address of the object.
 * @param[in] type   The type of object.
 * @retval [OS_OK]  The object is initialized successfully.
 * @retval [OS_NOK] The object initialization failed.
 **/
os_err_t os_object_thread_init(os_object_t *object, os_uint8_t type)
{
    os_uint8_t index, id = 0;
    for (index = 0; index < (*os_object_ctn).thread.num; index++)
    {
        if ((*((*os_object_ctn).thread.list[index]))->id != id++)
            break;
    }
    (*object)->id = id;
    (*object)->type = type;
    (*os_object_ctn).thread.list[(*os_object_ctn).thread.num++] = object;
    return OS_OK;
}

/**
 * @brief Gets the number of threads in the object container
 * @param[in] none
 * @return The number of threads in the object container
 **/
os_uint8_t os_object_get_thrd_num(void)
{

    return (*os_object_ctn).thread.num;
}

/**
 * @brief Gets the list of threads in the object container
 * @param[in] none
 * @return The list of threads in the object container
 **/
os_object_t *os_object_get_thrd_list(void)
{
    return (*os_object_ctn).thread.list;
}

/**
 * @brief  Delete the object and detach it from the object container.
 * @param[in] type The type of object.
 * @retval [OS_OK]  The object is detached successfully.
 * @retval [OS_NOK] The object detach failed.
 **/
os_err_t os_object_thread_delete(os_object_t *object)
{
    os_uint8_t index = 0;
    for (index = 0; index < (*os_object_ctn).thread.num; index++)
    {
        if (object == (os_object_t *)(*os_object_ctn).thread.list[index])
        {
            (*os_object_ctn).thread.num--;
            os_object_ctn_THREAD_REPLACE(index, (*os_object_ctn).thread.num);
            return OS_OK;
        }
    }
    return OS_NOK;
}

/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/