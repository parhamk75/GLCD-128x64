#include "GLCD.h"


// Maintanance Functions
/**
  * @brief  Initialize display and show initial color on it (Entire screen)
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  int_clr_    the desired color to be shown on whole display
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_Init(GLCD_Handle_TypeDef* phglcd_, GLCD_DispColor_TypeDef int_clr_)
{
    GLCD_LL_Init(phglcd_->pglcd_ll);
    return GLCD_Driver_SetWholeDispColor(phglcd_, GLCD_Driver_DispHalf_Both, int_clr_);
}


/**
  * @brief  Turn display on or off
  * @param  phglcd_ a pointer to GLCD Handler
  * @param  on_off_ turn display into this state (on/off)
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_OnOff(GLCD_Handle_TypeDef* phglcd_, GLCD_DispOnOff_TypeDef on_off_)
{
    return GLCD_Driver_OnOff(phglcd_, GLCD_Driver_DispHalf_Both, on_off_);
}


/**
  * @brief  Clear display and show the color on it (Entire screen)
  * @param  phglcd_ a pointer to GLCD Handler
  * @param  clr_    the color want to be shown on entire screen
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_ClearScreen(GLCD_Handle_TypeDef* phglcd_, GLCD_DispColor_TypeDef clr_)
{
    return GLCD_Driver_SetWholeDispColor(phglcd_, GLCD_Driver_DispHalf_Both, clr_);
}


// Positioning Functions
/**
  * @brief  Scrolling the content of display along Y axis
  * @param  phglcd_ a pointer to GLCD Handler
  * @param  scrl_   the amount of scrolling along Y axis <pixels>
  * @note   Positive values will scroll up, negative values will scroll down,
  *         and zero (0) will reset the scrolling position.
  * @note   this function sets the Z address register (Display Start Line) 
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_ScrollY(GLCD_Handle_TypeDef* phglcd_, int8_t scrl_)
{
    // UPDATE: This can be much more efficient
    int8_t  tmp_scrl = scrl_;
    while(tmp_scrl < 0)
    {
        tmp_scrl += 64;
    }
    return GLCD_Driver_GotoZ(phglcd_, GLCD_Driver_DispHalf_Both, tmp_scrl%64);
}


// Write Functions
/**
  * @brief  Write a special color in a pixel in determined position
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  clr_        the desired color to write
  * @param  x_          X of the pixel position <pixels>
  * @param  y_          Y of the pixel position <pixels>
  * @param  is_crclr_   determines whether or not the entered values for x and y
  *                     are circular. It returns with error status if x and/or y
  *                     values are larger than max and is_crclr_ is false. 
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_WriteXY(GLCD_Handle_TypeDef* phglcd_, GLCD_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, bool is_crclr_)
{
    // Overflow check
    if((x_ > GLCD_X_MAX) || (y_ > GLCD_Y_MAX))
    {
        if(is_crclr_ == false)
        {
            return GLCD_ERROR;
        }
    }

    // Extract Required values for writing data
    uint8_t tmp_x_L2 = y_%64,
            tmp_y_L2 = x_%128,
            tmp_data = (uint8_t)(0x01 << (tmp_x_L2 % 8));
    GLCD_Driver_DispHalf_TypeDef tmp_hlf = (tmp_y_L2 > 63)? GLCD_Driver_DispHalf_Right: GLCD_Driver_DispHalf_Left;
    tmp_x_L2 /= 8;
    tmp_y_L2 %= 64;

    // Write Procedure
    return GLCD_Driver_TrnsprntWriteByteXY(phglcd_, tmp_hlf, (clr_ == GLCD_Driver_DispColor_Black)? tmp_data: ~tmp_data, clr_, tmp_x_L2, tmp_y_L2);
}


/**
  * @brief  Write a bitmap in determined position
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  pdata_      a pointer to bitmap data
  * @param  x_          X of upper left corner of the bitmap position <pixels>
  * @param  y_          Y of upper left corner of the bitmap position <pixels>
  * @param  sizex_      size of the bitmap along X axis (width) <pixels>
  * @param  sizey_      size of the bitmap along Y axis (height) <pixels>
  * @param  is_crclr_   determines whether or not the entered values for x, y, 
  *                     sizex, and sizey are circular. It returns with error
  *                     status if x and/or y values are larger than max and
  *                     is_crclr_ is false. 
  * @note   The bitmap data should have vertical byte orientation
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_WriteBitmap(GLCD_Handle_TypeDef* phglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, bool is_crclr_)
{
    // Y-Size Check
    if(sizey_%8 != 0 ){return GLCD_ERROR;}
    // Overflow check
    if((x_+sizex_ > GLCD_X_MAX+1) || (y_+sizey_ > GLCD_Y_MAX+1))
    {
        if(is_crclr_ == false)
        {
            return GLCD_ERROR;
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
            GLCD_Driver_DispHalf_TypeDef tmp_hlf = (tmp_y_L2 > 63)? GLCD_Driver_DispHalf_Right: GLCD_Driver_DispHalf_Left;
            tmp_y_L2 %= 64;
            
            if(y_%8 == 0)
            {
                GLCD_Driver_WriteByteXY(phglcd_, tmp_hlf, *(tmp_pdata++), tmp_x_L2, tmp_y_L2);
            }
            else
            {
                uint8_t tmp_mask = 0xFF,
                        tmp_data = *(pdata_ + cntr_col + sizex_* ( cntr_row < sizey_/8 ? cntr_row : sizey_/8 -1 ) );

                if(cntr_row == 0)
                {
                    tmp_mask = 0xFF >> (8 - y_%8);
                    uint8_t tmp_prev_data = GLCD_Driver_ReadByteXY(phglcd_, tmp_hlf, tmp_x_L2, tmp_y_L2) & tmp_mask;
                    tmp_data = tmp_prev_data | (tmp_data << (y_%8));
                    GLCD_Driver_WriteByteXY(phglcd_, tmp_hlf, tmp_data, tmp_x_L2, tmp_y_L2);
                }
                else if(cntr_row == tmp_rows-1)
                {
                    tmp_mask = 0xFF << (y_%8);
                    uint8_t tmp_prev_data = GLCD_Driver_ReadByteXY(phglcd_, tmp_hlf, tmp_x_L2, tmp_y_L2) & tmp_mask;
                    tmp_data =  tmp_prev_data | (tmp_data >> (8 - (y_%8)) );
                    GLCD_Driver_WriteByteXY(phglcd_, tmp_hlf, tmp_data, tmp_x_L2, tmp_y_L2);
                }
                else
                {
                    uint8_t tmp_data_prv_row = *(pdata_ + cntr_col + sizex_* (cntr_row-1));
                    tmp_data = (tmp_data << (y_%8)) | (tmp_data_prv_row >> (8 - (y_%8)));
                    GLCD_Driver_WriteByteXY(phglcd_, tmp_hlf, tmp_data, tmp_x_L2, tmp_y_L2);
                }        
            }
        } 
        tmp_x_L2++;
    }
    
    return GLCD_OK;
}


/**
  * @brief  Add a bitmap to the content of the determined position
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  pdata_      a pointer to bitmap data
  * @param  x_          X of upper left corner of the bitmap position <pixels>
  * @param  y_          Y of upper left corner of the bitmap position <pixels>
  * @param  sizex_      size of the bitmap along X axis (width) <pixels>
  * @param  sizey_      size of the bitmap along Y axis (height) <pixels>
  * @param  clr_        choose which pixel color should be written among bitmap
  * @param  is_crclr_   determines whether or not the entered values for x, y, 
  *                     sizex, and sizey are circular. It returns with error
  *                     status if x and/or y values are larger than max and
  *                     is_crclr_ is false. 
  * @note   The bitmap data should have vertical byte orientation
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_TrnsprntWriteBitmap(GLCD_Handle_TypeDef* phglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, GLCD_DispColor_TypeDef clr_, bool is_crclr_)
{
    // Y-Size Check
    if(sizey_%8 != 0 ){return GLCD_ERROR;}
    // Overflow check
    if((x_+sizex_ > GLCD_X_MAX+1) || (y_+sizey_ > GLCD_Y_MAX+1))
    {
        if(is_crclr_ == false)
        {
            return GLCD_ERROR;
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
            GLCD_Driver_DispHalf_TypeDef tmp_hlf = (tmp_y_L2 > 63)? GLCD_Driver_DispHalf_Right: GLCD_Driver_DispHalf_Left;
            tmp_y_L2 %= 64;
            
            if(y_%8 == 0)
            {
                GLCD_Driver_TrnsprntWriteByteXY(phglcd_, tmp_hlf, *(tmp_pdata++), clr_, tmp_x_L2, tmp_y_L2);
            }
            else
            {
                uint8_t tmp_data = *(pdata_ + cntr_col + sizex_* ( cntr_row < sizey_/8 ? cntr_row : sizey_/8 -1 ) );

                if(cntr_row == 0)
                {
                    tmp_data = tmp_data << (y_%8);
                    if (clr_ == GLCD_Driver_DispColor_White)
                    {
                        tmp_data |= 0xFF >> (8 - (y_%8));
                    }
                    GLCD_Driver_TrnsprntWriteByteXY(phglcd_, tmp_hlf, tmp_data, clr_, tmp_x_L2, tmp_y_L2);
                }
                else if(cntr_row == tmp_rows-1)
                {
                    tmp_data = tmp_data >> (8 - (y_%8));
                    if (clr_ == GLCD_Driver_DispColor_White)
                    {
                        tmp_data |= 0xFF << y_%8;
                    }
                    GLCD_Driver_TrnsprntWriteByteXY(phglcd_, tmp_hlf, tmp_data, clr_, tmp_x_L2, tmp_y_L2);
                }
                else
                {
                    uint8_t tmp_data_prv_row = *(pdata_ + cntr_col + sizex_* (cntr_row-1));
                    tmp_data = (tmp_data << (y_%8)) | (tmp_data_prv_row >> (8 - (y_%8)));
                    GLCD_Driver_TrnsprntWriteByteXY(phglcd_, tmp_hlf, tmp_data, clr_, tmp_x_L2, tmp_y_L2);
                }        
            }
        } 
        tmp_x_L2++;
    }
    
    return GLCD_OK;
}






