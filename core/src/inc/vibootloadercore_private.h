/**
 * @file vibootloadercore_private.h
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Private header file of bootloader core
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

#ifndef VIBOOTCORE_PRIVATE_H
#define VIBOOTCORE_PRIVATE_H

#include "vibootloadercore.h"
#include "vistmachine.h"
#include "viswtimer.h"
#include "vibuttonctrl.h"
#include "viflashdrv.h"

#define STATES_NUMBER 4 /*!< states number of state machine */

/**
 * @brief States identification names
 * 
 */
#define STATE_NAME_CHECK_BUTTON     "CheckButt"
#define STATE_NAME_HOLD_BOOTLOADER  "HoldBootld"
#define STATE_NAME_SEARCH_FIRMWARE  "SearchFw"
#define STATE_NAME_JUMP_TO_FIRMWARE "JumpToFw"

#define TIMER_NAME "BootCoreTim1"  /*!< timer name used in bootloader core */

/**
 * @brief Bootloader core control type
 * 
 */
typedef struct {
  VISTMACH_state_t statesArray[STATES_NUMBER]; /*!< states of state machine */
  VISTMACH_states_t stMachineInstatnce;        /*!< state machine instance*/
  const char* buttonName;                      /*!< button name to control bootloader core*/
  const TCHAR* firmwarePath;                   /*!< firmware path in filesystem */
  void* firmwareStartPoint;                    /*!< pointer to firmware entry point*/
  VIBCORE_Jump_t jumpCb;                       /*!< jump call-back funktion */
}BootloaderCore_t;

/**
 * @brief Funtion to define start flash address of a file, 
 * this address is used to jump to frimeware
 * @param file file descriptor
 * @return size_t flash address
 */
static size_t defineStartFlashAddrForFile(const FIL* file);

/**
 * @brief Control funktions for state CheckButton
 * This funktions will used as callback by init a state of state machine
 * @param this pointer to state mashine instance 
 * @param context user call context
 */
static void CheckButtonEntry(void* this, void* context);
static void CheckButtonPending(void* this, void* context);
//static void CheckButtonExit(void* this, void* context);

/**
 * @brief Control funktions for state HoldBootloader
 * This funktions will used as callback by init a state of state machine
 * @param this pointer to state mashine instance 
 * @param context user call context
 */
static void HoldBootloaderEntry(void* this, void* context);
static void HoldBootloaderPending(void* this, void* context);
//static void HoldBootloaderExit(void* this, void* context);

/**
 * @brief Control funktions for state SearchFirmware
 * This funktions will used as callback by init a state of state machine
 * @param this pointer to state mashine instance 
 * @param context user call context
 */
static void SearchFirmwareEntry(void* this, void* context);
static void SearchFirmwarePending(void* this, void* context);
//static void SearchFirmwareExit(void* this, void* context);

/**
 * @brief Control funktions for state SearchFirmware
 * This funktions will used as callback by init a state of state machine
 * @param this pointer to state mashine instance 
 * @param context user call context
 */
static void JumpToFirmwareEntry(void* this, void* context);
//static void JumpToFirmwarePending(void* this, void* context);
//static void JumpToFirmwareExit(void* this, void* context);

#endif // VIBOOTCORE_PRIVATE_H
