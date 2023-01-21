/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : stdint_os.h
|  - Author        : zevorn
|  - Update date   : 2021.03.23
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
#ifndef __OS_MEM_H_
#define __OS_MEM_H_

#include "os_type.h"

/*------------------------------------------------------------------------
|                                  MACRO                                 |
------------------------------------------------------------------------*/

/* None. */

#define GET_PTR_ADDR(dat)   (&(dat))


/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/**
 * @brief      Dynamic application memory function.
 * @param[in]  ptr The pointer to apply for memory.
 * @param[in]  size memory size.
 * @retval  [address] The memory application is successful, 
 *          and the first address of the memory block is returned for ptr.
 * @retval  [OS_NULL] The memory application is failed, 
 *          and the NULL pointer is returned for ptr.
 * @note  The process of allocating memory is divided into two steps:
 *        1. First obtain a space from the top of the memory pool to store 
 *           the memory block information header, and the direction is to generate downwards;
 *        2. Get a space from the bottom of the memory pool to store the memory block storage area, 
 *           and the direction is to grow upward.
 *        	   Icon:
 *
 *             ------------------                                    ------------------
 *             |  memPool-info  |                                    |  memPool-info  |
 *             ------------------                                    ------------------
 *             | memBlock1-info |                                    | memBlock1-info |
 *             ------------------                                    ------------------ 
 *             |                |    Allocate a block of memory      | memBlock2-info | 
 *             ------------------          ============>             ------------------ 
 *             |                |                                    |                |
 *             ------------------                                    ------------------
 *             |                |                                    |                |
 *             ------------------                                    ------------------
 *             |                |                                    | memBlock2-spac |
 *             ------------------                                    ------------------
 *             | memBlock1-spac |                                    | memBlock1-spac |
 *             ------------------                                    ------------------ 
**/
extern void *os_malloc(void *mptr,os_size_t msize);


/**
 * @brief      释放内存函数,支持内存碎片整理。
 * @details    Free memory function, Does support memory defragmentation.
 * @param[in]  ptr 要释放内存的指针。The pointer to free for memory.
 * @return     [OS_OK] 内存释放成功。
 *             The memory free is successful.
 * @return     [OS_NOK] 内存释放失败，返回NULL空指针给ptr指针。
 *             The memory free is failed.
 * @note       内存块被释放以后，排在这个内存块后面的内存块，会被全部移动，
 *             以填补空出来的位置，同时再更新被移动后的所有内存块的信息头，
 *             关键点是，更新处理信息头里面申请内存块的指针为被移动后的地址。
 * 			   After the memory block is released, 
 *             all the memory blocks behind this memory block will be moved.
 * 			   To fill in the vacant positions, and at the same time update 
 *             the information headers of all memory blocks that have been moved,
 *             The key point is that the pointer of the application memory block 
 *             in the update processing information header is the address after being moved.
 *        	   Icon:
 *
 *             ----------------                                    ----------------
 *             | 内存池-信息头 |                                    | 内存池-信息头 | 
 *             ----------------                                    ----------------
 *             | 内存块1信息头 |                                    | 内存块2信息头 | 
 *             ----------------                                    ---------------- 
 *             | 内存块2信息头 |            释放内存块1             |               | 
 *             ----------------          ============>             ---------------- 
 *             |              |                                    |              |
 *             ----------------                                    ----------------
 *             |              |                                    |              |
 *             ----------------                                    ----------------
 *             | 内存块2信息头 |                                    |              |
 *             ----------------                                    ----------------
 *             | 内存块1存储区 |                                    | 内存块2存储区 | 
 *             ----------------                                    ---------------- 
**/
extern os_err_t os_free(void *mptr);


/**
 * @brief The address of the requested memory pointer to send the memory block to be reclaimedW
 * @param[in] mptr Address of the memory pointer
 * @retval [OS_OK] The address was sent successfully
 * @retval [OS_NOK] Failed to send the address
**/
extern os_err_t os_mem_send_rec_addr(void *mpt);


/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/

#endif