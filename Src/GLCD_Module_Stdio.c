#include "GLCD_Module_Stdio.h"

uint8_t GLCD_Stdio_No_Lines = 8;


// Normal Write Funcs

GLCD_Status_TypeDef   GLCD_Text_SetNoLines(uint8_t no_lines_)
{}


GLCD_Status_TypeDef   GLCD_Text_WriteCharLine(GLCD_Handle_TypeDef* phglcd_, uint16_t char_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_)
{}


GLCD_Status_TypeDef   GLCD_Text_WriteStringLine(GLCD_Handle_TypeDef* phglcd_, uint16_t* str_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_)
{}


// Transparent Write Funcs

GLCD_Status_TypeDef GLCD_Text_TrnsprntWriteCharLine(GLCD_Handle_TypeDef* phglcd_, uint16_t char_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_Driver_DispColor_TypeDef clr_)
{}


GLCD_Status_TypeDef GLCD_Text_TrnsprntWriteStringLine(GLCD_Handle_TypeDef* phglcd_, uint16_t* str_, GLCD_Text_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_Driver_DispColor_TypeDef clr_, uint8_t len_, bool one_line_, bool cut_off_)
{}

