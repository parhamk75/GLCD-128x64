#include "GLCD_L4_T.h"


uint8_t GLCD_L4_T_No_Lines = 8;


// Normal Write Funcs
HAL_StatusTypeDef   GLCD_L4_T_WriteCharXY(GLCD_L0_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_)
{
    const uint8_t* tmp_pdata;
    uint16_t no_bytes = font_->width *( (font_->height/8) + (font_->height %8 > 0 ? 1 : 0) );

    // Check if it is available in font data
    if(char_ > font_->last_char_num || char_ < font_->first_char_num)
    {
        tmp_pdata = font_->undifined_char_data;
    }
    else
    {
        tmp_pdata = font_->font_data + ((char_-font_->first_char_num) * (no_bytes+1));
    }

    return GLCD_L3_WriteBitmap(pglcd_, tmp_pdata+1, x_, y_, *(tmp_pdata), font_->height, false);
}


HAL_StatusTypeDef   GLCD_L4_T_WriteStringXY(GLCD_L0_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_)
{}



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


