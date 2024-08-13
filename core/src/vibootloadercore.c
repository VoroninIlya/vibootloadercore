/**
 * @file vibootloadercore.c
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Source file of bootloader core
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

#include "vibootloadercore_private.h"

static BootloaderCore_t bootloaderCore;

bool VIBCORE_Init(const char* buttonName, const TCHAR* firmwarePath, 
    VIBCORE_Jump_t jumpCb) {

  if(NULL == buttonName || NULL == firmwarePath || NULL == jumpCb)
    return false;

  bootloaderCore.jumpCb = jumpCb;

  if(!VISTMACH_Init(&bootloaderCore.stMachineInstatnce, 
    bootloaderCore.statesArray, STATES_NUMBER)) return false;

  if(!VISWTIM_Create(TIMER_NAME)) return false;

  bootloaderCore.buttonName = buttonName;
  bootloaderCore.firmwarePath = firmwarePath;
  bootloaderCore.firmwareStartPoint = NULL;
  // Create and add CheckButton state
  VISTMACH_cbState_t cbEntry1 = {CheckButtonEntry, (void*)bootloaderCore.buttonName};
  VISTMACH_cbState_t cbPending1 = {CheckButtonPending, (void*)bootloaderCore.buttonName};
  VISTMACH_cbState_t cbExit1 = {NULL, NULL};
  VISTMACH_state_t checkButton;
  VISTMACH_InitState(&checkButton, STATE_NAME_CHECK_BUTTON, 
    cbEntry1, cbPending1, cbExit1);
  if(!VISTMACH_AddState(&bootloaderCore.stMachineInstatnce, checkButton)) return false;

  // Create and add HoldBootloader state
  VISTMACH_cbState_t cbEntry2 = {HoldBootloaderEntry, (void*)bootloaderCore.buttonName};
  VISTMACH_cbState_t cbPending2 = {HoldBootloaderPending, (void*)bootloaderCore.buttonName};
  VISTMACH_state_t holdBootloader;
  VISTMACH_InitState(&holdBootloader, STATE_NAME_HOLD_BOOTLOADER, 
    cbEntry2, cbPending2, cbExit1);
  if(!VISTMACH_AddState(&bootloaderCore.stMachineInstatnce, holdBootloader)) return false;
  
  // Create and add SearchFirmware state
  VISTMACH_cbState_t cbEntry3 = {SearchFirmwareEntry, (void*)bootloaderCore.firmwarePath};
  VISTMACH_cbState_t cbPending3 = {SearchFirmwarePending, (void*)bootloaderCore.firmwarePath};
  VISTMACH_state_t searchFirmware;
  VISTMACH_InitState(&searchFirmware, STATE_NAME_SEARCH_FIRMWARE, 
    cbEntry3, cbPending3, cbExit1);
  if(!VISTMACH_AddState(&bootloaderCore.stMachineInstatnce, searchFirmware)) return false;

  // Create and add JumpToFirmware state
  VISTMACH_cbState_t cbEntry4 = {JumpToFirmwareEntry, (void*)bootloaderCore.firmwareStartPoint};
  VISTMACH_cbState_t cbPending4 = {NULL, NULL};
  VISTMACH_state_t jumpToFirmware;
  VISTMACH_InitState(&jumpToFirmware, STATE_NAME_JUMP_TO_FIRMWARE, 
    cbEntry4, cbPending4, cbExit1);
  if(!VISTMACH_AddState(&bootloaderCore.stMachineInstatnce, jumpToFirmware)) return false;

  VISTMACH_Start(&bootloaderCore.stMachineInstatnce);

  return true;
}

void VIBCORE_Runtime(void) {
  VISTMACH_Runtime(&bootloaderCore.stMachineInstatnce);
}

static size_t defineStartFlashAddrForFile(const FIL* file) {
  size_t startDiskAddress = VIFLASH_GetStartDiskAddress();
  size_t startDataAddress = startDiskAddress + file->obj.fs->database*_MIN_SS;
  return startDataAddress + (file->obj.sclust-2)*file->obj.fs->csize*_MIN_SS;
}

static void CheckButtonEntry(void* this, void* context) {

  const char* buttonName = (const char*)context;
  VISTMACH_states_t* thisMachine = (VISTMACH_states_t*)this;

  if(VIBTCTR_IsPressed(buttonName))
    VISTMACH_SwitchToStateByIndex(thisMachine, thisMachine->activeState + 1);
  else
    VISTMACH_SwitchToStateByName(thisMachine, STATE_NAME_SEARCH_FIRMWARE);
}

static void CheckButtonPending(void* this, void* context) {
  const char* buttonName = (const char*)context;
  VISTMACH_states_t* thisMachine = (VISTMACH_states_t*)this;
  if(VIBTCTR_IsPressed(buttonName))
    VISTMACH_SwitchToStateByIndex(thisMachine, thisMachine->activeState + 1);
  else
    VISTMACH_SwitchToStateByName(thisMachine, STATE_NAME_SEARCH_FIRMWARE);
}

static void HoldBootloaderEntry(void* this, void* context) {
  VISWTIM_Start(TIMER_NAME, 10000);
}

static void HoldBootloaderPending(void* this, void* context) {
  const char* buttonName = (const char*)context;
  VISTMACH_states_t* thisMachine = (VISTMACH_states_t*)this;
  if(VISWTIM_IsExpired(TIMER_NAME)) {
    if(VIBTCTR_IsPressed(buttonName))
      VISTMACH_SwitchToStateByIndex(thisMachine, thisMachine->activeState + 1);
  }
  
}

static void SearchFirmwareEntry(void* this, void* context) {
  FILINFO fno;
  FIL fp;
  const TCHAR* firmwarePath = (const TCHAR*)context;
  VISTMACH_states_t* thisMachine = (VISTMACH_states_t*)this;
  FRESULT res = f_stat (firmwarePath, &fno);
  if(FR_OK == res) {
    res = f_open (&fp, firmwarePath, FA_READ);
    bootloaderCore.firmwareStartPoint = (void*)defineStartFlashAddrForFile(&fp);
    VISTMACH_SwitchToStateByIndex(thisMachine, thisMachine->activeState + 1);
  } else
    VISWTIM_Start(TIMER_NAME, 10000);
}

static void SearchFirmwarePending(void* this, void* context) {
  FILINFO fno;
  FIL fp;
  const TCHAR* firmwarePath = (const TCHAR*)context;
  VISTMACH_states_t* thisMachine = (VISTMACH_states_t*)this;
  if(VISWTIM_IsExpired(TIMER_NAME)) {
    FRESULT res = f_stat (firmwarePath, &fno);
    if(FR_OK == res) {
      res = f_open (&fp, firmwarePath, FA_READ);
      bootloaderCore.firmwareStartPoint = (void*)defineStartFlashAddrForFile(&fp);
      VISTMACH_SwitchToStateByIndex(thisMachine, thisMachine->activeState + 1);
    } else
      VISWTIM_Start(TIMER_NAME, 10000);
  }
}

static void JumpToFirmwareEntry(void* this, void* context) {
  if(bootloaderCore.jumpCb)
    bootloaderCore.jumpCb((uint32_t)(bootloaderCore.firmwareStartPoint));
}
