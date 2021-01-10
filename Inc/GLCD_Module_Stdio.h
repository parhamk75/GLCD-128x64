#ifndef __GLCD_MODULE_STDIO_H
#define __GLCD_MODULE_STDIO_H

#include "GLCD.h"
#include "GLCD_Module_Text.h"
#include "stdint.h"
#include "stdbool.h"


// Normal Write Funcs

GLCD_Status_TypeDef   GLCD_Stdio_SetNoLines        (uint8_t no_lines_);

GLCD_Status_TypeDef   GLCD_Stdio_WriteCharLine     (GLCD_Handle_TypeDef* phglcd_, uint16_t char_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_);

GLCD_Status_TypeDef   GLCD_Stdio_WriteStringLine   (GLCD_Handle_TypeDef* phglcd_, uint16_t* str_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_);

// Transparent Write Funcs

GLCD_Status_TypeDef   GLCD_Stdio_TrnsprntWriteCharLine     (GLCD_Handle_TypeDef* phglcd_, uint16_t char_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_Driver_DispColor_TypeDef clr_);

GLCD_Status_TypeDef   GLCD_Stdio_TrnsprntWriteStringLine   (GLCD_Handle_TypeDef* phglcd_, uint16_t* str_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_Driver_DispColor_TypeDef clr_, uint8_t len_, bool one_line_, bool cut_off_);


#endif  //__GLCD_MODULE_STDIO_H