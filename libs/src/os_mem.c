/*------------------------------------------------------------------------
|                             FILE DESCRIPTION                           |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_mem.c
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
|                               INCLUDES                                 |
------------------------------------------------------------------------*/

#include "os_mem.h"
#include "os_cpu.h"

/*------------------------------------------------------------------------
|                             DECLARATION                                |
------------------------------------------------------------------------*/

/**
 * @brief The address is shifted to the higher address.
 **/
#define ADDR_HIGH_OFFSET(addr, length) \
    do                                 \
    {                                  \
        (addr) += (length);            \
    } while (0)

/**
 * @brief The address is shifted to the lower address.
 **/
#define ADDR_LOW_OFFSET(addr, length) \
    do                                \
    {                                 \
        (addr) -= (length);           \
    } while (0)

#if (OS_SOFT_TIMER_CTRL == 0)

/**
 * @brief The length of the thread list is macro,
 *        plus 1 (idle thread) on the original basis.
 **/
#define OS_THREAD_LIST_LEN (OS_THREAD_MAX_NUM + 1)

#elif

/**
 * @brief The length of the thread list is macro,
 *        plus 1 (idle thread) on the original basis.
 **/
#define OS_THREAD_LIST_LEN (OS_THREAD_MAX_NUM + 2)

#endif

/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/**
 * @brief  Memory pool structure type
 **/
struct mpool
{
    os_vuint8_t *remaddr;               /*!< Address of the current free memory block */
    os_vuint8_t *tail_info;             /*!< Information address of the current free memory block */
    os_vuint8_t mblk_cnt;               /*!< Number of memory blocks applied */
    os_vuint16_t relength;              /*!< The length of the remaining free memory */
    os_vuint8_t pool[OS_MEM_POOL_SIZE]; /*!< Memory pool */
};

/**
 * @brief     定义内存池,声明储存在XRAM当中。
 * @details   Define memory pool,the statement is stored in XRAM.
 **/
static xdata struct mpool g_mpool;

/**
 * @brief     内存块信息结构体类型。
 * @details   Memory block information structure type.
 * @note      内存块信息储存在内存池的顶部，向下生长。
 *            内存块储存在内存池的底部，向上生长。
 *            The memory block information is stored
 *            at the top of the memory pool and grows downward.
 *            The memory block is stored
 *            at the bottom of the memory pool and grows upward.
 **/
struct mblock
{
    volatile void **mptr; /*!< 要申请内存的指针地址。*/
    os_vuint8_t *maddr;   /*!< 内存块首地址。 */
    os_vuint16_t length;  /*！< 内存块长度。*/
};

/**
 * @brief     内存回收器结构体。
 * @details   Memory collector structure.
 * @note      内存块信息储存在内存池的顶部，向下生长。
 *            内存块储存在内存池的底部，向上生长。
 *            The memory block information is stored
 *            at the top of the memory pool and grows downward.
 *            The memory block is stored
 *            at the bottom of the memory pool and grows upward.
 **/
struct rec_mem_type
{
    void *mptr[OS_THREAD_LIST_LEN];
    os_uint8_t num;
};

static struct rec_mem_type g_mem_rec_clt = {0};

#define MEM_BLOK_INFO_SIZE (sizeof(struct mblock))

/*------------------------------------------------------------------------
|                                FUNCTION                                |
------------------------------------------------------------------------*/

/**
 * @brief  The memory initialization function initializes
 *         the memory pool,Divided into two parts,static and dynamic,
 *         the kernel mainly uses static methods.
 * @param[in]  None.
 * @retval  [OS_OK] The memory is initialized successfully.
 * @retval  [OS_NOK] The memory initialization failed.
 **/
os_err_t os_mem_init(void)
{
    //	os_uint16_t *test1 = OS_NULL;
    //	os_uint16_t *test2 = OS_NULL;
    //	os_uint16_t *test3 = OS_NULL;

    g_mpool.mblk_cnt = 0;
    g_mpool.relength = OS_MEM_POOL_SIZE;
    g_mpool.remaddr = g_mpool.pool;
    g_mpool.tail_info = &g_mpool.pool[OS_MEM_POOL_SIZE];

    //	test1 = os_malloc(&test1, sizeof(os_uint16_t));
    //	test2 = os_malloc(&test2, sizeof(os_uint16_t));
    //	test3 = os_malloc(&test3, sizeof(os_uint16_t));
    //
    //	*test1 = 1;
    //	*test2 = 2;
    //	*test3 = 3;
    //
    //	os_free(&test1);
    //	os_free(&test2);
    //
    //	os_free(&test3);

    return OS_OK;
}

/**
 * @brief Dynamic application memory function
 * @param[in]  ptr The pointer to apply for memory.
 * @param[in]  size memory size
 * @retval  [address] The memory application is successful,
 *          and the first address of the memory block is returned for ptr
 * @retval  [OS_NULL] The memory application is failed,
 *          and the NULL pointer is returned for ptr
 * @note  The process of allocating memory is divided into two steps:
 *        1. First obtain a space from the bottom of the memory pool to store the memory
 *           block information header, and the direction is to generate upward;
 *        2. Get a space from the top of the memory pool to store the memory block storage area,
 *           and the direction is to grow downwards
 *
 *        	   Icon:
 *                                             low address
 *             ------------------                                    ------------------
 *             |  memPool-info  |                                    |  memPool-info  |
 *             ------------------                                    ------------------
 *             | memBlock1-spac |                                    | memBlock1-spac |
 *             ------------------                                    ------------------
 *             |                |     allocate a block of memory     | memBlock2-spac |
 *             ------------------            ============>           ------------------
 *             |                |                                    |                |
 *             ------------------                                    ------------------
 *             |                |                                    |                |
 *             ------------------                 tail_info ---->    ------------------
 *             |                |                                    | memBlock2-info |
 *             ------------------ <---- tail_info                    ------------------
 *             | memBlock1-info |                                    | memBlock1-info |
 *             ------------------                                    ------------------
 *                                             high address
 **/
void *os_malloc(void *mptr, os_size_t msize)
{
    os_uint8_t *ptr;

    if (msize >= OS_MEM_POOL_SIZE)
    {
        return OS_NULL;
    }

    OS_ENTER_CRITICAL();

    /** Update the memory block header address of the memory pool to the lower address */
    ADDR_LOW_OFFSET(g_mpool.tail_info, MEM_BLOK_INFO_SIZE);

    /** Updated the memory block header */
    ((struct mblock *)g_mpool.tail_info)->mptr = mptr;
    ((struct mblock *)g_mpool.tail_info)->maddr = g_mpool.remaddr;
    ((struct mblock *)g_mpool.tail_info)->length = msize;

    ptr = g_mpool.remaddr;
    ADDR_HIGH_OFFSET(g_mpool.remaddr, msize);

    /** Updated the memory pool header */
    g_mpool.relength -= (MEM_BLOK_INFO_SIZE + msize);
    g_mpool.mblk_cnt += 1;

    OS_EXIT_CRITICAL();

    return ptr; /*!< Gets the first address of the memory block storage area */
}

/**
 * @brief Free memory function, Does support memory defragmentation
 * @param[in] ptr   The pointer to free for memory
 * @retval [OS_OK]  The memory free is successful
 * @retval [OS_NOK] The memory free is failed
 * @note After the memory block is released, all the memory blocks behind this memory block
 *       will be moved. To fill in the vacant positions, and at the same time update the information
 *       headers of all memory blocks that have been moved. The key point is that the pointer of
 *       the application memory block in the update processing information header is the address after
 *       being moved
 *
 *        	   Icon:
 *                                             low address
 *             ------------------                                    ------------------
 *             |  memPool-info  |                                    |  memPool-info  |
 *             ------------------                                    ------------------
 *             | memBlock1-spac |                                    | memBlock1-spac |
 *             ------------------                                    ------------------
 *             | memBlock2-spac |       free a block of memory       |                |
 *             ------------------            ============>           ------------------
 *             |                |                                    |                |
 *             ------------------                                    ------------------
 *             |                |                                    |                |
 *             ------------------ <---- tail_info                    ------------------
 *             | memBlock2-info |                                    |                |
 *             ------------------                   tail_info ---->  ------------------
 *             | memBlock1-info |                                    | memBlock1-info |
 *             ------------------                                    ------------------
 *                                             high address
 **/
os_err_t os_free(void *mptr)
{
    os_uint8_t index;
    os_uint16_t msize;
    os_uint8_t *info;
    os_uint8_t *free_mem, *mov_mem;

    if ((g_mpool.mblk_cnt == 0) || (mptr == OS_NULL))
    {
        return OS_NOK;
    }

    OS_ENTER_CRITICAL();

    /** Gets the header address of the current free memory block,
     *  which is used to traverse the applied memory block */
    info = g_mpool.tail_info;

    for (index = 0; index < g_mpool.mblk_cnt; index++)
    {

        /** Find a block of memory by retrieving the physical address of the
         *  pointer to which the memory was requested */
        if (((struct mblock *)info)->mptr == mptr)
        {
            *(((struct mblock *)(info))->mptr) = OS_NULL;

            while (index--)
            {
                free_mem = ((struct mblock *)(info))->maddr;
                mov_mem = ((struct mblock *)(info - 8))->maddr;
                msize = ((struct mblock *)(info - 8))->length;
                while (msize--)
                {
                    *(free_mem++) = *(mov_mem++);
                }

                /** Gets the size of the memory block */
                msize = ((struct mblock *)info)->length;

                ((struct mblock *)(info))->mptr = ((struct mblock *)(info - 8))->mptr;
                *(((struct mblock *)(info))->mptr) -= msize;

                ((struct mblock *)(info))->length = ((struct mblock *)(info - 8))->length;
                ((struct mblock *)(info))->maddr = ((struct mblock *)(info - 8))->maddr - msize;

                ADDR_LOW_OFFSET(info, MEM_BLOK_INFO_SIZE);
            }

            /** Finally, the memory pool information is updated */
            g_mpool.mblk_cnt -= 1;
            g_mpool.relength += (msize + MEM_BLOK_INFO_SIZE);
            ADDR_LOW_OFFSET(g_mpool.remaddr, msize);
            ADDR_HIGH_OFFSET(g_mpool.tail_info, MEM_BLOK_INFO_SIZE);

            OS_EXIT_CRITICAL();
            return OS_OK;
        }

        /** Each cycle, the memory block header address is offset by MEM_BLOK_INFO_SIZE bytes
         *  (this is the size of the header space) */
        ADDR_HIGH_OFFSET(info, MEM_BLOK_INFO_SIZE);
    }

    OS_EXIT_CRITICAL();
    return OS_NOK;
}

/**
 * @brief The address of the requested memory pointer to send the memory block to be reclaimedW
 * @param[in] mptr Address of the memory pointer
 * @retval [OS_OK] The address was sent successfully
 * @retval [OS_NOK] Failed to send the address
 **/
os_err_t os_mem_send_rec_addr(void *mptr)
{
    if (g_mem_rec_clt.num < OS_THREAD_MAX_NUM)
    {
        g_mem_rec_clt.mptr[g_mem_rec_clt.num++] = mptr;
        return OS_OK;
    }
    return OS_NOK;
}

/**
 * @brief  recovery of memory
 * @param[in]  none
 * @return  none
 **/
os_err_t os_mem_reco(void)
{
    while (g_mem_rec_clt.num)
    {
        if (os_free(g_mem_rec_clt.mptr[--g_mem_rec_clt.num]) != OS_OK)
        {
            return OS_NOK;
        }
    }
    return OS_OK;
}
/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/