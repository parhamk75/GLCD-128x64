#ifndef __GLCD_L3_H
#define __GLCD_L3_H

#include "GLCD_L2.h"

#define GLCD_L3_X_MAX            ((uint8_t)127)         // X <Max>
#define GLCD_L3_Y_MAX            ((uint8_t)63)          // Y <Max>


// Maintanance Functions
HAL_StatusTypeDef GLCD_L3_Init(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef int_clr_);
HAL_StatusTypeDef GLCD_L3_OnOff(GLCD_L0_TypeDef* pglcd_, GLCD_L1_Disp_OnOff_TypeDef on_off_);
HAL_StatusTypeDef GLCD_L3_ClearScreen(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef clr_);

// Positioning Functions
HAL_StatusTypeDef GLCD_L3_ScrollY(GLCD_L0_TypeDef* pglcd_, int8_t scrl_);   // Scroll Up(+) Down(-) Reset(0)
// HAL_StatusTypeDef GLCD_L3_GotoXY(GLCD_L0_TypeDef* pglcd_ uint8_t x_, uint8_t y_);
// HAL_StatusTypeDef GLCD_L3_GotoOrigin(GLCD_L0_TypeDef* pglcd_);  // x, y, z -> 0


// Write Functions
// HAL_StatusTypeDef GLCD_L3_Write(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef clr_);
HAL_StatusTypeDef GLCD_L3_WriteXY(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t is_crclr_);
HAL_StatusTypeDef GLCD_L3_WriteBitmap(GLCD_L0_TypeDef* pglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, uint8_t is_crclr_);
HAL_StatusTypeDef GLCD_L3_TrnsprntWriteBitmap(GLCD_L0_TypeDef* pglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, GLCD_L2_DispColor_TypeDef clr_, uint8_t is_crclr_);


#endif  //_GLCD_L3_H