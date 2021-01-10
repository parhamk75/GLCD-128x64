#include "GLCD_Module_Text.h"


// Normal Write Funcs
/**
  * @brief  Write a character in the determined position
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  char_       character (the value with respect to the font)
  * @param  font_       a pointer to the desired font
  * @param  x_          X of upper left corner of the character position <pixels>
  * @param  y_          Y of upper left corner of the character position <pixels>
  * @note   if the entered values of x and/or y are larger than the maximum, it
  *         will return by an error state.
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_Text_WriteCharXY(GLCD_Handle_TypeDef* phglcd_, uint32_t char_, GLCD_Text_Font_TypeDef* font_, uint8_t x_, uint8_t y_)
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

    return GLCD_WriteBitmap(phglcd_, tmp_pfdata+1, x_, y_, *(tmp_pfdata), font_->height, false);
}


/**
  * @brief  Write a string in the determined position
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  str_        a pointer to the array of characters
  * @param  dt_         characters data type (It can be useful in unicode,...)
  * @param  font_       a pointer to the desired font
  * @param  x_          X of upper left corner of the string position <pixels>
  * @param  y_          Y of upper left corner of the string position <pixels>
  * @param  len_        length of the string (length of the array of characters)
  * @param  one_line_   It determines whether the string should be on a
  *                     single line or can be written in multiple lines
  * @param  cut_off_    If this be true, when a string riches to the end of
  *                     the screen (in length or height), it will stop writing.
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_Text_WriteStringXY(GLCD_Handle_TypeDef* phglcd_, void* str_, GLCD_Text_Data_TypeDef dt_, GLCD_Text_Font_TypeDef* font_, uint8_t x_, uint8_t y_, uint16_t len_, bool one_line_, bool cut_off_)
{

    uint32_t* tmp_pstr_32 = (dt_ == GLCD_Text_Data_32bit ? (uint32_t*) str_ : NULL);
    uint16_t* tmp_pstr_16 = (dt_ == GLCD_Text_Data_16bit ? (uint16_t*) str_ : NULL);
    uint8_t*  tmp_pstr_8  = (dt_ == GLCD_Text_Data_8bit  ? (uint8_t*)  str_ : NULL);

    const uint8_t* tmp_pfdata;
    uint16_t no_bytes = font_->width *( (font_->height/8) + (font_->height %8 > 0 ? 1 : 0) );

    for(uint16_t cntr_char = 0; cntr_char < len_; cntr_char++)
    {
        uint32_t tmp_char_val = 0;
        if(dt_==GLCD_Text_Data_32bit)
        {
            tmp_char_val = *(tmp_pstr_32++);
        }
        else if(dt_==GLCD_Text_Data_16bit)
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
        if(x_ + (*tmp_pfdata) > GLCD_X_MAX)
        {
            if(cut_off_)
            {
                return GLCD_ERROR;
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
        if(y_ + font_->height > GLCD_Y_MAX)
        {
            if(cut_off_)
            {
                return GLCD_ERROR;
            }
            else
            {
                if(one_line_)
                {
                    return GLCD_ERROR;
                }
                else
                {
                    y_ = 0;
                }
            }
        }
        
        // Eventually: Writing the Character!
        GLCD_Status_TypeDef tmp_stat = GLCD_OK;
        tmp_stat = GLCD_WriteBitmap(phglcd_, tmp_pfdata+1, x_, y_, *(tmp_pfdata), font_->height, false);
        if(tmp_stat != GLCD_OK)
        {
            // It means a more serious problem occured
            // (e.g. font is bigger than whole screen!)
            return tmp_stat;
        }
        
        x_ += *tmp_pfdata;
    }

    return GLCD_OK;
}


// Transparent Write Funcs
/**
  * @brief  Add a character in the determined position
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  char_       character (the value with respect to the font)
  * @param  font_       a pointer to the desired font
  * @param  x_          X of upper left corner of the character position <pixels>
  * @param  y_          Y of upper left corner of the character position <pixels>
  * @param  clr_        choose which pixel color should be written among 
  *                     the desired font bitmap
  * @note   if the entered values of x and/or y are larger than the maximum, it
  *         will return by an error state.
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_Text_TrnsprntWriteCharXY(GLCD_Handle_TypeDef* phglcd_, uint32_t char_, GLCD_Text_Font_TypeDef* font_, uint8_t x_, uint8_t y_, GLCD_Driver_DispColor_TypeDef clr_)
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

    return GLCD_TrnsprntWriteBitmap(phglcd_, tmp_pfdata+1, x_, y_, *(tmp_pfdata), font_->height, clr_, false);
}


/**
  * @brief  Add a string in the determined position
  * @param  phglcd_     a pointer to GLCD Handler
  * @param  str_        a pointer to the array of characters
  * @param  dt_         characters data type (It can be useful in unicode,...)
  * @param  font_       a pointer to the desired font
  * @param  x_          X of upper left corner of the string position <pixels>
  * @param  y_          Y of upper left corner of the string position <pixels>
  * @param  clr_        choose which pixel color should be written among 
  *                     the desired font bitmap
  * @param  len_        length of the string (length of the array of characters)
  * @param  one_line_   It determines whether the string should be on a
  *                     single line or can be written in multiple lines
  * @param  cut_off_    If this be true, when a string riches to the end of
  *                     the screen (in length or height), it will stop writing.
  * @retval GLCD Status (GLCD_OK, ...)
*/
GLCD_Status_TypeDef GLCD_Text_TrnsprntWriteStringXY(GLCD_Handle_TypeDef* phglcd_, void* str_, GLCD_Text_Data_TypeDef dt_, GLCD_Text_Font_TypeDef* font_, uint8_t x_, uint8_t y_, GLCD_Driver_DispColor_TypeDef clr_, uint8_t len_, bool one_line_, bool cut_off_)
{

    uint32_t* tmp_pstr_32 = (dt_ == GLCD_Text_Data_32bit ? (uint32_t*) str_ : NULL);
    uint16_t* tmp_pstr_16 = (dt_ == GLCD_Text_Data_16bit ? (uint16_t*) str_ : NULL);
    uint8_t*  tmp_pstr_8  = (dt_ == GLCD_Text_Data_8bit  ? (uint8_t*)  str_ : NULL);

    const uint8_t* tmp_pfdata;
    uint16_t no_bytes = font_->width *( (font_->height/8) + (font_->height %8 > 0 ? 1 : 0) );

    for(uint16_t cntr_char = 0; cntr_char < len_; cntr_char++)
    {
        uint32_t tmp_char_val = 0;
        if(dt_==GLCD_Text_Data_32bit)
        {
            tmp_char_val = *(tmp_pstr_32++);
        }
        else if(dt_==GLCD_Text_Data_16bit)
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
        if(x_ + (*tmp_pfdata) > GLCD_X_MAX)
        {
            if(cut_off_)
            {
                return GLCD_ERROR;
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
        if(y_ + font_->height > GLCD_Y_MAX)
        {
            if(cut_off_)
            {
                return GLCD_ERROR;
            }
            else
            {
                if(one_line_)
                {
                    return GLCD_ERROR;
                }
                else
                {
                    y_ = 0;
                }
            }
        }
        
        // Eventually: Writing the Character!
        GLCD_Status_TypeDef tmp_stat = GLCD_OK;
        tmp_stat = GLCD_TrnsprntWriteBitmap(phglcd_, tmp_pfdata+1, x_, y_, *(tmp_pfdata), font_->height, clr_, false);
        if(tmp_stat != GLCD_OK)
        {
            // It means a more serious problem occured
            // (e.g. font is bigger than whole screen!)
            return tmp_stat;
        }
        
        x_ += *tmp_pfdata;
    }

    return GLCD_OK;
}

