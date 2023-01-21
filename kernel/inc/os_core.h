/*------------------------------------------------------------------------
|                            FILE DESCRIPTION                            |
------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|  - File name     : os_core.h
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
#ifndef __OS_CORE_H_
#define __OS_CORE_H_

#include "os_type.h"

/*------------------------------------------------------------------------
|                                  DATA                                  |
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
|                              API FUNCTIONS                             |
------------------------------------------------------------------------*/

/**
 * @brief  AntOS initializes the startup code, and this function is executed before main.
 * @param[in]  None.
 * @retval  [OS_OK] Ant RTOS initailized successfully. 
 * @retval  [OS_NOK] Ant RTOS initailized failed. 
**/
int antos_startup_init(void);


/**
 * @brief  To initialize the board support package, 
 *         you need to implement this function with the same name externally, 
 *         otherwise an error will be reported.
 * @param[in]  None.
 * @return  None.
 * @note  It is necessary to initialize a timer and start its interrupt. 
 *        It is recommended to set the interrupt time to 5~10ms.
 *        Use this interruption time as the system heartbeat beat time.
**/
extern void antos_adf_init(void);


/**
 * @brief  The main function provided by AntOS is used to create threads. 
 *         This function of the same name needs to be implemented externally, 
 *         otherwise an error will be reported.
 * @param[in]  None.
 * @return  None.
**/
extern void antos_main(void);

#endif
/*------------------------------------------------------------------------
|                    END OF FLIE.  (C) COPYRIGHT zevorn                  |
------------------------------------------------------------------------*/