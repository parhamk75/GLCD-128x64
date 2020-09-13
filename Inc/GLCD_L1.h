#ifndef __GLCD_L1_H
#define __GLCD_L1_H

#include "GLCD_L0.h"

#define GLCD_L1_InsBase_DispOnOff           ((uint8_t)0x3E)        // Display On/Off Instruction Base
#define GLCD_L1_InsBase_SetAdddress         ((uint8_t)0x40)        // Set Address Instruction Base
#define GLCD_L1_InsBase_SetPage             ((uint8_t)0xB8)        // Set Page (X Address) Instruction Base
#define GLCD_L1_InsBase_SetDispStartLine    ((uint8_t)0xC0)        // Set Display Start Line Instruction Base


typedef enum{
    GLCD_L1_Disp_On     = (uint8_t)1,
    GLCD_L1_Disp_Off    = (uint8_t)0
}GLCD_L1_Disp_OnOff_TypeDef;

HAL_StatusTypeDef   GLCD_L1_Disp_OnOff          (GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  GLCD_L1_Disp_OnOff_TypeDef on_off_);
HAL_StatusTypeDef   GLCD_L1_Set_Address         (GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t y_addr_);
HAL_StatusTypeDef   GLCD_L1_Set_Page            (GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t page_);
HAL_StatusTypeDef   GLCD_L1_Set_DispStartLine   (GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t dsp_strt_ln_);
HAL_StatusTypeDef   GLCD_L1_Write_DispData      (GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_, uint8_t data_);
uint8_t             GLCD_L1_Read_Status         (GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_);
uint8_t             GLCD_L1_Read_DispData       (GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_);


#endif  //_GLCD_L1_H