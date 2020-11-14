#include "GLCD_L3.h"



// Maintanance Functions
HAL_StatusTypeDef GLCD_L3_Init(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef int_clr_)
{
    return GLCD_L2_FullInit(pglcd_, int_clr_);
}


HAL_StatusTypeDef GLCD_L3_OnOff(GLCD_L0_TypeDef* pglcd_, GLCD_L1_Disp_OnOff_TypeDef on_off_)
{
    return GLCD_L2_OnOff(pglcd_, GLCD_L2_HALF_Both, on_off_);
}


HAL_StatusTypeDef GLCD_L3_ClearScreen(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef clr_)
{
    return GLCD_L2_SetWholeDispColor(pglcd_, GLCD_L2_HALF_Both, clr_);
}



// Positioning Functions
HAL_StatusTypeDef GLCD_L3_ScrollY(GLCD_L0_TypeDef* pglcd_, int8_t scrl_)
{
    return GLCD_L2_GotoZ(pglcd_, GLCD_L2_HALF_Both, scrl_);
}



// Write Functions
HAL_StatusTypeDef GLCD_L3_WriteXY(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t is_crclr_)
{
    // Overflow check
    if((x_ > GLCD_L3_X_MAX) || (y_ > GLCD_L3_Y_MAX))
    {
        if(is_crclr_ == 0)
        {
            return HAL_ERROR;
        }
    }

    // Extract Required values for writing data
    uint8_t tmp_x_L2 = y_%64,
            tmp_y_L2 = x_%128,
            tmp_data = (uint8_t)(0x01 << (tmp_x_L2 % 8));
    GLCD_L2_HALF_TypeDef tmp_hlf = (tmp_y_L2 > 63)? GLCD_L2_HALF_Right: GLCD_L2_HALF_Left;
    tmp_x_L2 /= 8;
    tmp_y_L2 %= 64;

    // Write Procedure
    return GLCD_L2_TrnsprntWriteByteXY(pglcd_, tmp_hlf, (clr_ == GLCD_L2_DispColor_Black)? tmp_data: ~tmp_data, clr_, tmp_x_L2, tmp_y_L2);
}


HAL_StatusTypeDef GLCD_L3_WriteBitmap(GLCD_L0_TypeDef* pglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, uint8_t is_crclr_)
{

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L3_TrnsprntWriteBitmap(GLCD_L0_TypeDef* pglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, GLCD_L2_DispColor_TypeDef clr_, uint8_t is_crclr_)
{

    return HAL_OK;
}






