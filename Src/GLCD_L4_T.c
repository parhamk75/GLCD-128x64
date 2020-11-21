#include "GLCD_L4_T.h"


uint8_t GLCD_L4_T_No_Lines = 8;


// Normal Write Funcs
HAL_StatusTypeDef   GLCD_L4_T_WriteCharXY(GLCD_L0_TypeDef* pglcd_, uint32_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_)
{
    const uint8_t* tmp_pfdata;
    uint16_t no_bytes = font_->width *( (font_->height/8) + (font_->height %8 > 0 ? 1 : 0) );

    // Check if it is available in font data
    if(char_ > font_->last_char_num || char_ < font_->first_char_num)
    {
        tmp_pfdata = font_->undifined_char_data;
    }
    else
    {
        tmp_pfdata = font_->font_data + ((char_-font_->first_char_num) * (no_bytes+1));
    }

    return GLCD_L3_WriteBitmap(pglcd_, tmp_pfdata+1, x_, y_, *(tmp_pfdata), font_->height, false);
}


HAL_StatusTypeDef   GLCD_L4_T_WriteStringXY(GLCD_L0_TypeDef* pglcd_, void* str_, GLCD_L4_Data_TypeDef dt_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, uint16_t len_, bool one_line_, bool cut_off_)
{

    uint32_t* tmp_pstr_32 = (dt_ == GLCD_L4_T_Data_32bit ? (uint32_t*) str_ : NULL);
    uint16_t* tmp_pstr_16 = (dt_ == GLCD_L4_T_Data_16bit ? (uint16_t*) str_ : NULL);
    uint8_t*  tmp_pstr_8  = (dt_ == GLCD_L4_T_Data_8bit  ? (uint8_t*)  str_ : NULL);

    const uint8_t* tmp_pfdata;
    uint16_t no_bytes = font_->width *( (font_->height/8) + (font_->height %8 > 0 ? 1 : 0) );

    for(uint16_t cntr_char = 0; cntr_char < len_; cntr_char++)
    {
        uint32_t tmp_char_val = 0;
        if(dt_==GLCD_L4_T_Data_32bit)
        {
            tmp_char_val = *(tmp_pstr_32++);
        }
        else if(dt_==GLCD_L4_T_Data_16bit)
        {
            tmp_char_val = *(tmp_pstr_16++);
        }
        else
        {
            tmp_char_val = *(tmp_pstr_8++);
        }

        // Check if it is available in font data
        if(tmp_char_val > font_->last_char_num || tmp_char_val < font_->first_char_num)
        {
            tmp_pfdata = font_->undifined_char_data;
        }
        else
        {
            tmp_pfdata = font_->font_data + ((tmp_char_val-font_->first_char_num) * (no_bytes+1));
        }

        // Checking for the EOL Problems (X)
        if(x_ + (*tmp_pfdata) > GLCD_L3_X_MAX)
        {
            if(cut_off_)
            {
                return HAL_ERROR;
            }
            else
            {
                x_ = 0;
                if(!one_line_)
                {
                    y_ += font_->height;
                }
            }
        }

        // Checking for the EOL Problems (Y) [Notice that this should happen
        // after the y incrementing by a 'font height' in previous step, so 
        // don't change its place!]
        if(y_ + font_->height > GLCD_L3_Y_MAX)
        {
            if(cut_off_)
            {
                return HAL_ERROR;
            }
            else
            {
                if(one_line_)
                {
                    return HAL_ERROR;
                }
                else
                {
                    y_ = 0;
                }
            }
        }
        
        // Eventually: Writing the Character!
        HAL_StatusTypeDef tmp_stat = HAL_OK;
        tmp_stat = GLCD_L3_WriteBitmap(pglcd_, tmp_pfdata+1, x_, y_, *(tmp_pfdata), font_->height, false);
        if(tmp_stat != HAL_OK)
        {
            // It means a more serious problem occured
            // (e.g. font is bigger than whole screen!)
            return tmp_stat;
        }
        
        x_ += *tmp_pfdata;
    }

    return HAL_OK;
}



HAL_StatusTypeDef   GLCD_L4_T_SetNoLines(uint8_t no_lines_)
{}


HAL_StatusTypeDef   GLCD_L4_T_WriteCharLine(GLCD_L0_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_)
{}


HAL_StatusTypeDef   GLCD_L4_T_WriteStringLine(GLCD_L0_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_)
{}


// Transparent Write Funcs
HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteCharXY   (GLCD_L0_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_);
HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteStringXY (GLCD_L0_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_, uint8_t len_, bool one_line_, bool cut_off_);

HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteCharLine     (GLCD_L0_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_);
HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteStringLine   (GLCD_L0_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_, uint8_t len_, bool one_line_, bool cut_off_);


