#ifndef __GLCD_L1_H
#define __GLCD_L1_H

#include "GLCD_L0.h"
#include "stdint.h"
#include "stdbool.h"

#define GLCD_L1_InsBase_DispOnOff           ((uint8_t)0x3E)        // 'Display On/Off' Instruction Base
#define GLCD_L1_InsMask_DispOnOff           ((uint8_t)0x01)        // 'Display On/Off' Instruction Mask
#define GLCD_L1_InsBase_SetAdddress         ((uint8_t)0x40)        // 'Set Address' Instruction Base
#define GLCD_L1_InsMask_SetAdddress         ((uint8_t)0x3F)        // 'Set Address' Instruction Mask
#define GLCD_L1_InsBase_SetPage             ((uint8_t)0xB8)        // 'Set Page (X Address)' Instruction Base
#define GLCD_L1_InsMask_SetPage             ((uint8_t)0x07)        // 'Set Page (X Address)' Instruction Mask
#define GLCD_L1_InsBase_SetDispStartLine    ((uint8_t)0xC0)        // 'Set Display Start Line' Instruction Base
#define GLCD_L1_InsMask_SetDispStartLine    ((uint8_t)0x3F)        // 'Set Display Start Line' Instruction Mask

#define GLCD_L1_StatusMask_OnOff            ((uint8_t)0x20)
#define GLCD_L1_StatusMask_Reset            ((uint8_t)0x10)
#define GLCD_L1_StatusMask_Busy             ((uint8_t)0x80)


typedef enum{
    GLCD_L1_DispColor_Black     = (uint8_t)1,
    GLCD_L1_DispColor_White     = (uint8_t)0
}GLCD_L1_DispColor_TypeDef;

typedef enum{
    GLCD_L1_HALF_Both           = (uint8_t)2,
    GLCD_L1_HALF_Right          = (uint8_t)1,
    GLCD_L1_HALF_Left           = (uint8_t)0
}GLCD_L1_HALF_TypeDef;

typedef enum{
    GLCD_L1_DispStatOnOff_Off       = (uint8_t)1,
    GLCD_L1_DispStatOnOff_On        = (uint8_t)0
}GLCD_L1_DispStatOnOff_TypeDef;

typedef enum{
    GLCD_L1_DispStatBusy_Busy       = (uint8_t)1,
    GLCD_L1_DispStatBusy_Ready      = (uint8_t)0
}GLCD_L1_DispStatBusy_TypeDef;

typedef enum{
    GLCD_L1_DispStatReset_InReset   = (uint8_t)1,
    GLCD_L1_DispStatReset_Normal    = (uint8_t)0
}GLCD_L1_DispStatReset_TypeDef;

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
HAL_StatusTypeDef   GLCD_L1_Ins_Disp_OnOff              (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  GLCD_L1_DispStatOnOff_TypeDef on_off_);
HAL_StatusTypeDef   GLCD_L1_Ins_Set_Address             (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t y_addr_);
HAL_StatusTypeDef   GLCD_L1_Ins_Set_Page                (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t page_);
HAL_StatusTypeDef   GLCD_L1_Ins_Set_DispStartLine       (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t dsp_strt_ln_);
HAL_StatusTypeDef   GLCD_L1_Ins_Write_DispData          (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t data_);
uint8_t             GLCD_L1_Ins_Read_Status             (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_);
uint8_t             GLCD_L1_Ins_Read_DispData           (GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_);

// Status Checks
GLCD_L1_DispStatOnOff_TypeDef   GLCD_L1_IsDispOnOff         (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_);
GLCD_L1_DispStatBusy_TypeDef    GLCD_L1_IsDispBusy          (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_);
// GLCD_L1_DispStatReset_TypeDef   GLCD_L1_IsDispReset         (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_);

// Initialization
HAL_StatusTypeDef   GLCD_L1_Init                    (GLCD_L1_TypeDef* pglcd_); // Init GPIO -> LCD Reset Procedure -> Set Addresses to 0
HAL_StatusTypeDef   GLCD_L1_FullInit                (GLCD_L1_TypeDef* pglcd_, GLCD_L1_DispColor_TypeDef init_whole_dsp_clr_); // GLCD_L1_Init() -> Set Whole Datas (Color)
HAL_StatusTypeDef   GLCD_L1_OnOff                   (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, GLCD_L1_DispStatOnOff_TypeDef on_off_);


// Set Whole Display Color
HAL_StatusTypeDef   GLCD_L1_SetWholeDispColor       (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, GLCD_L1_DispColor_TypeDef init_whole_dsp_clr_); // Set Whole Datas (Color)
HAL_StatusTypeDef   GLCD_L1_ClearDisplay            (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_); // GLCD_L1_SetWholeDispColor(White)

// Write Data
HAL_StatusTypeDef   GLCD_L1_WriteByte               (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t data_);
HAL_StatusTypeDef   GLCD_L1_WriteByteXY             (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L1_WriteByteXYZ            (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_, uint8_t z_);

// Read Data
uint8_t             GLCD_L1_ReadByte                (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_);
uint8_t             GLCD_L1_ReadByteXY              (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_);
uint8_t             GLCD_L1_ReadByteXYZ             (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_);

// Set Address Functions (Goto)
HAL_StatusTypeDef   GLCD_L1_GotoX                   (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t x_);
HAL_StatusTypeDef   GLCD_L1_GotoY                   (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L1_GotoZ                   (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L1_GotoXY                  (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L1_GotoXYZ                 (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_);

// Complex Write Functions
HAL_StatusTypeDef   GLCD_L1_TrnsprntWriteByte       (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_);
HAL_StatusTypeDef   GLCD_L1_TrnsprntWriteByteXY     (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L1_TrnsprntWriteByteXYZ    (GLCD_L1_TypeDef* pglcd_, GLCD_L1_HALF_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t z_);

// Buffering Tools
HAL_StatusTypeDef   GLCD_L1_SyncBuff_WriteToDisp    (GLCD_L1_TypeDef* pglcd1_, GLCD_L1_HALF_TypeDef hlf_);
HAL_StatusTypeDef   GLCD_L1_SyncBuff_ReadFromDisp   (GLCD_L1_TypeDef* pglcd1_, GLCD_L1_HALF_TypeDef hlf_);

#endif  //_GLCD_L1_H