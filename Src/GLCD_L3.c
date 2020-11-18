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
    // UPDATE: This can be much more efficient
    int8_t  tmp_scrl = scrl_;
    while(tmp_scrl < 0)
    {
        tmp_scrl += 64;
    }
    return GLCD_L2_GotoZ(pglcd_, GLCD_L2_HALF_Both, tmp_scrl%64);
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
    // Y-Size Check
    if(sizey_%8 != 0 ){return HAL_ERROR;}
    // Overflow check
    if((x_+sizex_ > GLCD_L3_X_MAX+1) || (y_+sizey_ > GLCD_L3_Y_MAX+1))
    {
        if(is_crclr_ == 0)
        {
            return HAL_ERROR;
        }        
    }

    uint8_t     tmp_x_L2    = (y_%64)/8,
                tmp_rows    = (y_%8 == 0)? (sizey_/8): (sizey_/8)+1;
                
    uint8_t*    tmp_pdata   = pdata_;
    
    for(uint8_t cntr_row = 0; cntr_row < tmp_rows; cntr_row++)
    {
        tmp_x_L2 %= 8;
        for (uint8_t cntr_col = 0; cntr_col < sizex_; cntr_col++)
        {
            uint8_t tmp_y_L2 = (x_ + cntr_col)%128;
            GLCD_L2_HALF_TypeDef tmp_hlf = (tmp_y_L2 > 63)? GLCD_L2_HALF_Right: GLCD_L2_HALF_Left;
            tmp_y_L2 %= 64;
            
            if(y_%8 == 0)
            {
                GLCD_L2_WriteByteXY(pglcd_, tmp_hlf, *(tmp_pdata++), tmp_x_L2, tmp_y_L2);
            }
            else
            {
                uint8_t tmp_mask = 0xFF,
                        tmp_data = *(pdata_ + cntr_col + sizex_* ( cntr_row < sizey_/8 ? cntr_row : sizey_/8 -1 ) );

                if(cntr_row == 0)
                {
                    tmp_mask = 0xFF >> (8 - y_%8);
                    uint8_t tmp_prev_data = GLCD_L2_ReadByteXY(pglcd_, tmp_hlf, tmp_x_L2, tmp_y_L2) & tmp_mask;
                    tmp_data = tmp_prev_data | (tmp_data << (y_%8));
                    GLCD_L2_WriteByteXY(pglcd_, tmp_hlf, tmp_data, tmp_x_L2, tmp_y_L2);
                }
                else if(cntr_row == tmp_rows-1)
                {
                    tmp_mask = 0xFF << (y_%8);
                    uint8_t tmp_prev_data = GLCD_L2_ReadByteXY(pglcd_, tmp_hlf, tmp_x_L2, tmp_y_L2) & tmp_mask;
                    tmp_data =  tmp_prev_data | (tmp_data >> (8 - (y_%8)) );
                    GLCD_L2_WriteByteXY(pglcd_, tmp_hlf, tmp_data, tmp_x_L2, tmp_y_L2);
                }
                else
                {
                    uint8_t tmp_data_prv_row = *(pdata_ + cntr_col + sizex_* (cntr_row-1));
                    tmp_data = (tmp_data << (y_%8)) | (tmp_data_prv_row >> (8 - (y_%8)));
                    GLCD_L2_WriteByteXY(pglcd_, tmp_hlf, tmp_data, tmp_x_L2, tmp_y_L2);
                }        
            }
        } 
        tmp_x_L2++;
    }
    
    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L3_TrnsprntWriteBitmap(GLCD_L0_TypeDef* pglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, GLCD_L2_DispColor_TypeDef clr_, uint8_t is_crclr_)
{
    // Y-Size Check
    if(sizey_%8 != 0 ){return HAL_ERROR;}
    // Overflow check
    if((x_+sizex_ > GLCD_L3_X_MAX+1) || (y_+sizey_ > GLCD_L3_Y_MAX+1))
    {
        if(is_crclr_ == 0)
        {
            return HAL_ERROR;
        }        
    }

    uint8_t     tmp_x_L2    = (y_%64)/8,
                tmp_rows    = (y_%8 == 0)? (sizey_/8): (sizey_/8)+1;
                
    uint8_t*    tmp_pdata   = pdata_;
    
    for(uint8_t cntr_row = 0; cntr_row < tmp_rows; cntr_row++)
    {
        tmp_x_L2 %= 8;
        for (uint8_t cntr_col = 0; cntr_col < sizex_; cntr_col++)
        {
            uint8_t tmp_y_L2 = (x_ + cntr_col)%128;
            GLCD_L2_HALF_TypeDef tmp_hlf = (tmp_y_L2 > 63)? GLCD_L2_HALF_Right: GLCD_L2_HALF_Left;
            tmp_y_L2 %= 64;
            
            if(y_%8 == 0)
            {
                GLCD_L2_TrnsprntWriteByteXY(pglcd_, tmp_hlf, *(tmp_pdata++), clr_, tmp_x_L2, tmp_y_L2);
            }
            else
            {
                uint8_t tmp_mask = 0xFF,
                        tmp_data = *(pdata_ + cntr_col + sizex_* ( cntr_row < sizey_/8 ? cntr_row : sizey_/8 -1 ) );

                if(cntr_row == 0)
                {
                    tmp_data = tmp_data << (y_%8);
                    if (clr_ == GLCD_L2_DispColor_White)
                    {
                        tmp_data |= 0xFF >> (8 - (y_%8));
                    }
                    GLCD_L2_TrnsprntWriteByteXY(pglcd_, tmp_hlf, tmp_data, clr_, tmp_x_L2, tmp_y_L2);
                }
                else if(cntr_row == tmp_rows-1)
                {
                    tmp_data = tmp_data >> (8 - (y_%8));
                    if (clr_ == GLCD_L2_DispColor_White)
                    {
                        tmp_data |= 0xFF << y_%8;
                    }
                    GLCD_L2_TrnsprntWriteByteXY(pglcd_, tmp_hlf, tmp_data, clr_, tmp_x_L2, tmp_y_L2);
                }
                else
                {
                    uint8_t tmp_data_prv_row = *(pdata_ + cntr_col + sizex_* (cntr_row-1));
                    tmp_data = (tmp_data << (y_%8)) | (tmp_data_prv_row >> (8 - (y_%8)));
                    GLCD_L2_TrnsprntWriteByteXY(pglcd_, tmp_hlf, tmp_data, clr_, tmp_x_L2, tmp_y_L2);
                }        
            }
        } 
        tmp_x_L2++;
    }
    
    return HAL_OK;
}






