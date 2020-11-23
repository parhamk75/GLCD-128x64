#ifndef __GLCD_L1_H
#define __GLCD_L1_H

#include "GLCD_L0.h"
#include "stdint.h"
#include "stdbool.h"

#define GLCD_L1_InsBase_DispOnOff           ((uint8_t)0x3E)        // Display On/Off Instruction Base
#define GLCD_L1_InsMask_DispOnOff           ((uint8_t)0x01)        // Display On/Off Instruction Mask
#define GLCD_L1_InsBase_SetAdddress         ((uint8_t)0x40)        // Set Address Instruction Base
#define GLCD_L1_InsMask_SetAdddress         ((uint8_t)0x3F)        // Set Address Instruction Mask
#define GLCD_L1_InsBase_SetPage             ((uint8_t)0xB8)        // Set Page (X Address) Instruction Base
#define GLCD_L1_InsMask_SetPage             ((uint8_t)0x07)        // Set Page (X Address) Instruction Mask
#define GLCD_L1_InsBase_SetDispStartLine    ((uint8_t)0xC0)        // Set Display Start Line Instruction Base
#define GLCD_L1_InsMask_SetDispStartLine    ((uint8_t)0x3F)        // Set Display Start Line Instruction Mask


typedef enum{
    GLCD_L1_Disp_On     = (uint8_t)1,
    GLCD_L1_Disp_Off    = (uint8_t)0
}GLCD_L1_Disp_OnOff_TypeDef;

// The GLCD Buffer typedef structure (It Stores Data and Addresses)
typedef struct{
    // Address Buffers
    uint8_t x;
    uint8_t y;
    uint8_t z;

    // Data Buffer
    uint8_t data[GLCD_L0_ROW_PIXELS *GLCD_L0_COL_PIXELS /8 /2];

}GLCD_L1_Buffer_TypeDef;

// The GLCD_L1 typedef structure (Adds buffering features to GLCD_L0_TypeDef)
typedef struct{
    // Pointer to a GLCD_L0_TypeDef instance
    GLCD_L0_TypeDef*            pglcd0;
    
    // Pointer to GLCD_L1_Buffer_TypeDef instances for right and left halves
    GLCD_L1_Buffer_TypeDef*     pbuffer_right;
    GLCD_L1_Buffer_TypeDef*     pbuffer_left;

    // Buffering flags
    bool                        buffered;
    bool                        synchronized;

}GLCD_L1_TypeDef;

// GLCD 128x64 Instruction Set
HAL_StatusTypeDef   GLCD_L1_Disp_OnOff          (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  GLCD_L1_Disp_OnOff_TypeDef on_off_);
HAL_StatusTypeDef   GLCD_L1_Set_Address         (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t y_addr_);
HAL_StatusTypeDef   GLCD_L1_Set_Page            (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t page_);
HAL_StatusTypeDef   GLCD_L1_Set_DispStartLine   (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t dsp_strt_ln_);
HAL_StatusTypeDef   GLCD_L1_Write_DispData      (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t data_);
uint8_t             GLCD_L1_Read_Status         (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_);
uint8_t             GLCD_L1_Read_DispData       (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_);

// GLCD Tools (Wrappers for L0 Functions)
HAL_StatusTypeDef   GLCD_L1_Delay       (uint16_t td_10xns_);
HAL_StatusTypeDef   GLCD_L1_StartReset  (GLCD_L1_TypeDef* pglcd_);
HAL_StatusTypeDef   GLCD_L1_StopReset   (GLCD_L1_TypeDef* pglcd_);
GPIO_PinState       GLCD_L1_CheckReset  (GLCD_L1_TypeDef* pglcd_);

#endif  //_GLCD_L1_H