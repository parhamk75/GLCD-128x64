#ifndef __GLCD_L2_H
#define __GLCD_L2_H

#include "GLCD_L1.h"

#define GLCD_L2_StatusMask_OnOff    (uint8_t)0x20
#define GLCD_L2_StatusMask_Reset    (uint8_t)0x10
#define GLCD_L2_StatusMask_Busy     (uint8_t)0x80

typedef enum{
    GLCD_L2_DispColor_Black     = (uint8_t)1,
    GLCD_L2_DispColor_White     = (uint8_t)0
}GLCD_L2_DispColor_TypeDef;

typedef enum{
    GLCD_L2_HALF_Both           = (uint8_t)2,
    GLCD_L2_HALF_Right          = (uint8_t)1,
    GLCD_L2_HALF_Left           = (uint8_t)0
}GLCD_L2_HALF_TypeDef;

typedef enum{
    GLCD_L2_DispStatBusy_Busy       = (uint8_t)1,
    GLCD_L2_DispStatBusy_Ready      = (uint8_t)0
}GLCD_L2_DispStatBusy_TypeDef;

typedef enum{
    GLCD_L2_DispStatOnOff_Off       = (uint8_t)1,
    GLCD_L2_DispStatOnOff_On        = (uint8_t)0
}GLCD_L2_DispStatOnOff_TypeDef;

typedef enum{
    GLCD_L2_DispStatReset_InReset   = (uint8_t)1,
    GLCD_L2_DispStatReset_Normal    = (uint8_t)0
}GLCD_L2_DispStatReset_TypeDef;

// Status Checks
GLCD_L2_DispStatOnOff_TypeDef   GLCD_L2_IsDispOnOff         (GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_);
GLCD_L2_DispStatBusy_TypeDef    GLCD_L2_IsDispBusy          (GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_);
GLCD_L2_DispStatReset_TypeDef   GLCD_L2_IsDispReset         (GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_);
uint8_t                         GLCD_L2_ReadDispStatus      (GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_);

// Initialization
HAL_StatusTypeDef   GLCD_L2_Init                (GLCD_L0_TypeDef* pglcd_); // Init GPIO -> LCD Reset Procedure -> Set Addresses to 0
HAL_StatusTypeDef   GLCD_L2_FullInit            (GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_); // GLCD_L2_Init() -> Set Whole Datas (Color)

// Set Whole Display Color
HAL_StatusTypeDef   GLCD_L2_SetWholeDispColor   (GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_, GLCD_L2_HALF_TypeDef hlf_); // Set Whole Datas (Color)
HAL_StatusTypeDef   GLCD_L2_ClearDisplay        (GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_, GLCD_L2_HALF_TypeDef hlf_); // GLCD_L2_SetWholeDispColor(White)

// Write Data
HAL_StatusTypeDef   GLCD_L2_WriteByte           (GLCD_L0_TypeDef* pglcd_, uint8_t data_);
HAL_StatusTypeDef   GLCD_L2_WriteByteXY         (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t data_);
HAL_StatusTypeDef   GLCD_L2_WriteByteXYZ        (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_, uint8_t data_);

// Read Data
uint8_t             GLCD_L2_ReadByte            (GLCD_L0_TypeDef* pglcd_);
uint8_t             GLCD_L2_ReadByteXY          (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_);
uint8_t             GLCD_L2_ReadByteXYZ         (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);

// Set Address Functions (Goto)
HAL_StatusTypeDef   GLCD_L2_GotoXYZ             (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoXY              (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoX               (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoY               (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoZ               (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);


#endif  //_GLCD_L2_H
