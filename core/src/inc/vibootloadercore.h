/**
 * @file vibootloadercore.h
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Header file of bootloader core
 *
 * @copyright Copyright (c) 2024 Ilia Voronin
 * 
 * This software is licensed under GNU GENERAL PUBLIC LICENSE 
 * The terms can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS,
 * Without warranty of any kind, express or implied, 
 * including but not limited to the warranties of merchantability, 
 * fitness for a particular purpose and noninfringement. 
 * In no event shall the authors or copyright holders be liable for any claim, 
 * damages or other liability, whether in an action of contract, tort or otherwise, 
 * arising from, out of or in connection with the software 
 * or the use or other dealings in the software.
 * 
 */

#ifndef VIBOOTCORE_H
#define VIBOOTCORE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "fatfs.h"

/**
 * @brief Jump callback function type
 * 
 */
typedef void (*VIBCORE_Jump_t)(uint32_t fileAddrInFlash);

/**
 * @brief Initialization of bootloader core driver
 * 
 * @param buttonName Button name, that is used to control bootloader core
 * @param firmwarePath Path of firmware file which will be tryed to load
 * @param jumpCb Jump call-back funktion to jumping to firmware start address
 * @return true if driver initialized successfull
 * @return false if any error occurred through initialization
 */
bool VIBCORE_Init(const char* buttonName, const TCHAR* firmwarePath, 
    VIBCORE_Jump_t jumpCb);

/**
 * @brief Driver runtime, should be placed in main loop
 * 
 */
void VIBCORE_Runtime(void);

#endif // VIBOOTCORE_H