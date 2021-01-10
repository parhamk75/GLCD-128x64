#ifndef __GLCD_H
#define __GLCD_H

#include "GLCD_Driver_KS0108.h"
#include "stdbool.h"
#include "stdint.h"

#define GLCD_X_MAX            ((uint8_t)127)         // X <Max>
#define GLCD_Y_MAX            ((uint8_t)63)          // Y <Max>

typedef enum{
    GLCD_DispColor_Black     = (uint8_t)GLCD_Driver_DispColor_Black,
    GLCD_DispColor_White     = (uint8_t)GLCD_Driver_DispColor_White
}GLCD_DispColor_TypeDef;

typedef enum{
    GLCD_DispOnOff_Off       = (uint8_t)GLCD_Driver_DispStatOnOff_Off,
    GLCD_DispOnOff_On        = (uint8_t)GLCD_Driver_DispStatOnOff_On
}GLCD_DispOnOff_TypeDef;


// Maintanance

GLCD_Status_TypeDef GLCD_Init                 ( GLCD_Handle_TypeDef*    phglcd_,
                                                GLCD_DispColor_TypeDef  int_clr_);

GLCD_Status_TypeDef GLCD_OnOff                ( GLCD_Handle_TypeDef*    phglcd_, 
                                                GLCD_DispOnOff_TypeDef  on_off_);

GLCD_Status_TypeDef GLCD_ClearScreen          ( GLCD_Handle_TypeDef*    phglcd_,
                                                GLCD_DispColor_TypeDef  clr_);


// Positioning

GLCD_Status_TypeDef GLCD_ScrollY              ( GLCD_Handle_TypeDef*    phglcd_,
                                                int8_t                  scrl_);


// Write

GLCD_Status_TypeDef GLCD_WriteXY              ( GLCD_Handle_TypeDef*    phglcd_,
                                                GLCD_DispColor_TypeDef  clr_,
                                                uint8_t                 x_,
                                                uint8_t                 y_,
                                                bool                    is_crclr_);

GLCD_Status_TypeDef GLCD_WriteBitmap          ( GLCD_Handle_TypeDef*    phglcd_,
                                                uint8_t*                pdata_,
                                                uint8_t                 x_,
                                                uint8_t                 y_,
                                                uint8_t                 sizex_,
                                                uint8_t                 sizey_,
                                                bool                    is_crclr_);

GLCD_Status_TypeDef GLCD_TrnsprntWriteBitmap  ( GLCD_Handle_TypeDef*    phglcd_,
                                                uint8_t*                pdata_,
                                                uint8_t                 x_,
                                                uint8_t                 y_,
                                                uint8_t                 sizex_,
                                                uint8_t                 sizey_,
                                                GLCD_DispColor_TypeDef  clr_,
                                                bool                    is_crclr_);


#endif  //_GLCD_H