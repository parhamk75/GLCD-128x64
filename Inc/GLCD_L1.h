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
    GLCD_L1_DispHalf_Both           = (uint8_t)2,
    GLCD_L1_DispHalf_Right          = (uint8_t)GLCD_L0_CS_1,
    GLCD_L1_DispHalf_Left           = (uint8_t)GLCD_L0_CS_2
}GLCD_L1_DispHalf_TypeDef;

typedef enum{
    GLCD_L1_DispStatOnOff_Off       = (uint8_t)1,
    GLCD_L1_DispStatOnOff_On        = (uint8_t)0
}GLCD_L1_DispStatOnOff_TypeDef;

typedef enum{
    GLCD_L1_DispStatBusy_Busy       = (uint8_t)1,
    GLCD_L1_DispStatBusy_Ready      = (uint8_t)0
}GLCD_L1_DispStatBusy_TypeDef;

// typedef enum{
//     GLCD_L1_DispStatReset_InReset   = (uint8_t)1,
//     GLCD_L1_DispStatReset_Normal    = (uint8_t)0
// }GLCD_L1_DispStatReset_TypeDef;


// GLCD 128x64 Instruction Set
GLCD_Status_TypeDef   GLCD_L1_Ins_Disp_OnOff              (GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  GLCD_L1_DispStatOnOff_TypeDef on_off_);
GLCD_Status_TypeDef   GLCD_L1_Ins_Set_Address             (GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  uint8_t y_addr_);
GLCD_Status_TypeDef   GLCD_L1_Ins_Set_Page                (GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  uint8_t page_);
GLCD_Status_TypeDef   GLCD_L1_Ins_Set_DispStartLine       (GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  uint8_t dsp_strt_ln_);
GLCD_Status_TypeDef   GLCD_L1_Ins_Write_DispData          (GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  uint8_t data_);
uint8_t             GLCD_L1_Ins_Read_Status             (GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_);
uint8_t             GLCD_L1_Ins_Read_DispData           (GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_);

// Status Checks
GLCD_L1_DispStatOnOff_TypeDef   GLCD_L1_IsDispOnOff         (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_);
GLCD_L1_DispStatBusy_TypeDef    GLCD_L1_IsDispBusy          (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_);
// GLCD_L1_DispStatReset_TypeDef   GLCD_L1_IsDispReset         (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_);

// Initialization
GLCD_Status_TypeDef   GLCD_L1_OnOff                   (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, GLCD_L1_DispStatOnOff_TypeDef on_off_); //TODO: move to L3

// Set Whole Display Color
GLCD_Status_TypeDef   GLCD_L1_SetWholeDispColor       (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, GLCD_L1_DispColor_TypeDef init_whole_dsp_clr_); // Set Whole Datas (Color)

// Write Data
GLCD_Status_TypeDef   GLCD_L1_WriteByte               (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_);
GLCD_Status_TypeDef   GLCD_L1_WriteByteXY             (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_);
GLCD_Status_TypeDef   GLCD_L1_WriteByteXYZ            (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_, uint8_t z_);

// Read Data
uint8_t             GLCD_L1_ReadByte                (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_);
uint8_t             GLCD_L1_ReadByteXY              (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_);
uint8_t             GLCD_L1_ReadByteXYZ             (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_);

// Set Address Functions (Goto)
GLCD_Status_TypeDef   GLCD_L1_GotoX                   (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_);
GLCD_Status_TypeDef   GLCD_L1_GotoY                   (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t y_);
GLCD_Status_TypeDef   GLCD_L1_GotoZ                   (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t z_);
GLCD_Status_TypeDef   GLCD_L1_GotoXY                  (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_);
GLCD_Status_TypeDef   GLCD_L1_GotoXYZ                 (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_);

// Transparent Write Functions
GLCD_Status_TypeDef   GLCD_L1_TrnsprntWriteByte       (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_);
GLCD_Status_TypeDef   GLCD_L1_TrnsprntWriteByteXY     (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_);
GLCD_Status_TypeDef   GLCD_L1_TrnsprntWriteByteXYZ    (GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t z_);

// Buffering Tools
GLCD_Status_TypeDef   GLCD_L1_SyncBuff_WriteToDisp    (GLCD_HandleTypeDef* pglcd1_, GLCD_L1_DispHalf_TypeDef hlf_);
GLCD_Status_TypeDef   GLCD_L1_SyncBuff_ReadFromDisp   (GLCD_HandleTypeDef* pglcd1_, GLCD_L1_DispHalf_TypeDef hlf_);

#endif  //_GLCD_L1_H