#ifndef __GLCD_Driver_H
#define __GLCD_Driver_H

#include "GLCD_LL_STM32.h"
#include "stdint.h"
#include "stdbool.h"

#define GLCD_Driver_InsBase_DispOnOff           ((uint8_t)0x3E)        // 'Display On/Off' Instruction Base
#define GLCD_Driver_InsMask_DispOnOff           ((uint8_t)0x01)        // 'Display On/Off' Instruction Mask
#define GLCD_Driver_InsBase_SetAdddress         ((uint8_t)0x40)        // 'Set Address' Instruction Base
#define GLCD_Driver_InsMask_SetAdddress         ((uint8_t)0x3F)        // 'Set Address' Instruction Mask
#define GLCD_Driver_InsBase_SetPage             ((uint8_t)0xB8)        // 'Set Page (X Address)' Instruction Base
#define GLCD_Driver_InsMask_SetPage             ((uint8_t)0x07)        // 'Set Page (X Address)' Instruction Mask
#define GLCD_Driver_InsBase_SetDispStartLine    ((uint8_t)0xC0)        // 'Set Display Start Line' Instruction Base
#define GLCD_Driver_InsMask_SetDispStartLine    ((uint8_t)0x3F)        // 'Set Display Start Line' Instruction Mask

#define GLCD_Driver_StatusMask_OnOff            ((uint8_t)0x20)
#define GLCD_Driver_StatusMask_Reset            ((uint8_t)0x10)
#define GLCD_Driver_StatusMask_Busy             ((uint8_t)0x80)


typedef enum{
    GLCD_Driver_DispColor_Black         = (uint8_t)1,
    GLCD_Driver_DispColor_White         = (uint8_t)0
}GLCD_Driver_DispColor_TypeDef;

typedef enum{
    GLCD_Driver_DispHalf_Both           = (uint8_t)2,
    GLCD_Driver_DispHalf_Right          = (uint8_t)GLCD_LL_CS_1,
    GLCD_Driver_DispHalf_Left           = (uint8_t)GLCD_LL_CS_2
}GLCD_Driver_DispHalf_TypeDef;

typedef enum{
    GLCD_Driver_DispStatOnOff_Off       = (uint8_t)1,
    GLCD_Driver_DispStatOnOff_On        = (uint8_t)0
}GLCD_Driver_DispStatOnOff_TypeDef;

typedef enum{
    GLCD_Driver_DispStatBusy_Busy       = (uint8_t)1,
    GLCD_Driver_DispStatBusy_Ready      = (uint8_t)0
}GLCD_Driver_DispStatBusy_TypeDef;


// GLCD 128x64 Instruction Set
GLCD_Status_TypeDef GLCD_Driver_Ins_Disp_OnOff              (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_LL_ChipSelect_TypeDef          cs_,
                                                                GLCD_Driver_DispStatOnOff_TypeDef   on_off_);

GLCD_Status_TypeDef GLCD_Driver_Ins_Set_Address             (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_LL_ChipSelect_TypeDef          cs_,
                                                                uint8_t                             y_addr_);

GLCD_Status_TypeDef GLCD_Driver_Ins_Set_Page                (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_LL_ChipSelect_TypeDef          cs_,
                                                                uint8_t                             page_);

GLCD_Status_TypeDef GLCD_Driver_Ins_Set_DispStartLine       (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_LL_ChipSelect_TypeDef          cs_,
                                                                uint8_t                             dsp_strt_ln_);

GLCD_Status_TypeDef GLCD_Driver_Ins_Write_DispData          (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_LL_ChipSelect_TypeDef          cs_,
                                                                uint8_t                             data_);

uint8_t GLCD_Driver_Ins_Read_Status                         (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_LL_ChipSelect_TypeDef          cs_);

uint8_t GLCD_Driver_Ins_Read_DispData                       (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_LL_ChipSelect_TypeDef          cs_);

// Status Checks
GLCD_Driver_DispStatOnOff_TypeDef   GLCD_Driver_IsDispOnOff (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_);

GLCD_Driver_DispStatBusy_TypeDef    GLCD_Driver_IsDispBusy  (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_);


// Initialization
GLCD_Status_TypeDef GLCD_Driver_OnOff                       (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                GLCD_Driver_DispStatOnOff_TypeDef   on_off_);


// Set Whole Display Color
GLCD_Status_TypeDef GLCD_Driver_SetWholeDispColor           (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                GLCD_Driver_DispColor_TypeDef       init_whole_dsp_clr_);


// Write Data
GLCD_Status_TypeDef GLCD_Driver_WriteByte                   (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             data_);

GLCD_Status_TypeDef GLCD_Driver_WriteByteXY                 (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             data_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_);

GLCD_Status_TypeDef GLCD_Driver_WriteByteXYZ                (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             data_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_,
                                                                uint8_t                             z_);


// Read Data
uint8_t GLCD_Driver_ReadByte                                (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_);

uint8_t GLCD_Driver_ReadByteXY                              (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_);

uint8_t GLCD_Driver_ReadByteXYZ                             (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_,
                                                                uint8_t                             z_);


// Set Address Functions (Goto)
GLCD_Status_TypeDef GLCD_Driver_GotoX                       (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             x_);

GLCD_Status_TypeDef GLCD_Driver_GotoY                       (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             y_);

GLCD_Status_TypeDef GLCD_Driver_GotoZ                       (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             z_);

GLCD_Status_TypeDef GLCD_Driver_GotoXY                      (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_);

GLCD_Status_TypeDef GLCD_Driver_GotoXYZ                     (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_,
                                                                uint8_t                             z_);


// Transparent Write Functions
GLCD_Status_TypeDef GLCD_Driver_TrnsprntWriteByte           (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             data_,
                                                                GLCD_Driver_DispColor_TypeDef       clr_);

GLCD_Status_TypeDef GLCD_Driver_TrnsprntWriteByteXY         (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             data_,
                                                                GLCD_Driver_DispColor_TypeDef       clr_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_);

GLCD_Status_TypeDef GLCD_Driver_TrnsprntWriteByteXYZ        (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_,
                                                                uint8_t                             data_,
                                                                GLCD_Driver_DispColor_TypeDef       clr_,
                                                                uint8_t                             x_,
                                                                uint8_t                             y_,
                                                                uint8_t                             z_);


// Buffering Tools
GLCD_Status_TypeDef GLCD_Driver_SyncBuff_WriteToDisp        (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_);

GLCD_Status_TypeDef GLCD_Driver_SyncBuff_ReadFromDisp       (   GLCD_Handle_TypeDef*                phglcd_,
                                                                GLCD_Driver_DispHalf_TypeDef        hlf_);


#endif  //_GLCD_Driver_H