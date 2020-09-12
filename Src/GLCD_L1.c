#include "GLCD_L1.h"

HAL_StatusTypeDef GLCD_L1_Disp_OnOff(GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  GLCD_L1_Disp_OnOff_TypeDef on_off_)
{
    return GLCD_L0_Write(pglcd_, (uint8_t)(GLCD_L1_InsBase_DispOnOff + on_off_), GLCD_L0_FrameType_Instruction, hlf_);
}


HAL_StatusTypeDef GLCD_L1_Set_Address(GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t y_addr_)
{
    return GLCD_L0_Write(pglcd_, (uint8_t)(GLCD_L1_InsBase_SetAdddress + y_addr_) , GLCD_L0_FrameType_Instruction, hlf_);
}


HAL_StatusTypeDef GLCD_L1_Set_Page(GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t page_)
{
    return GLCD_L0_Write(pglcd_, (uint8_t)(GLCD_L1_InsBase_SetPage + page_), GLCD_L0_FrameType_Instruction, hlf_);
}


HAL_StatusTypeDef GLCD_L1_Set_DispStartLine(GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t dsp_strt_ln_)
{
    return GLCD_L0_Write(pglcd_, (uint8_t)(GLCD_L1_InsBase_SetDispStartLine + dsp_strt_ln_), GLCD_L0_FrameType_Instruction, hlf_);
}


HAL_StatusTypeDef GLCD_L1_Write_DispData(GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_, uint8_t data_)
{
    return GLCD_L0_Write(pglcd_, data_, GLCD_L0_FrameType_Data, hlf_);
}


uint8_t GLCD_L1_Read_Status(GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_)
{
    return GLCD_L0_Read(pglcd_, GLCD_L0_FrameType_Instruction, hlf_);
}


uint8_t GLCD_L1_Read_DispData(GLCD_L0_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_)
{
    return GLCD_L0_Read(pglcd_, GLCD_L0_FrameType_Data, hlf_);
}
