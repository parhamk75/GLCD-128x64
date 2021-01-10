#ifndef __GLCD_MODULE_TEXT_H
#define __GLCD_MODULE_TEXT_H

#include "GLCD.h"
#include "stdint.h"
#include "stdbool.h"

typedef enum{
    GLCD_Text_Data_8bit   = (uint_fast8_t)0,
    GLCD_Text_Data_16bit  = (uint_fast8_t)1,
    GLCD_Text_Data_32bit  = (uint_fast8_t)2
}GLCD_Text_Data_TypeDef;

// The GLCD Pinout typedef structure
typedef struct{
    // Properties
    uint8_t     width;                  // General Width of Characters
    uint8_t     height;                 // General Height of Characters
    uint32_t    first_char_num;         // First Character Number (Index) in the Font_Data Array
    uint32_t    last_char_num;          // Last Character Number (Index) in the Font_Data Array

    // Data
    const uint8_t* undifined_char_data; // Pointer to the Font Data for Undifined Character Data Array in the STD Form
    const uint8_t* font_data;           // Pointer to the Font Data Array in the STD Form

}GLCD_Text_Font_TypeDef;


// Normal Write Funcs

GLCD_Status_TypeDef GLCD_Text_WriteCharXY           ( GLCD_Handle_TypeDef*          phglcd_,
                                                      uint32_t                      char_,
                                                      GLCD_Text_Font_TypeDef*       font_,
                                                      uint8_t                       x_,
                                                      uint8_t                       y_);

GLCD_Status_TypeDef GLCD_Text_WriteStringXY         ( GLCD_Handle_TypeDef*          phglcd_,
                                                      void*                         str_,
                                                      GLCD_Text_Data_TypeDef        dt_,
                                                      GLCD_Text_Font_TypeDef*       font_,
                                                      uint8_t                       x_,
                                                      uint8_t                       y_,
                                                      uint16_t                      len_,
                                                      bool                          one_line_,
                                                      bool                          cut_off_);


// Transparent Write Funcs

GLCD_Status_TypeDef GLCD_Text_TrnsprntWriteCharXY   ( GLCD_Handle_TypeDef*          phglcd_,
                                                      uint32_t                      char_,
                                                      GLCD_Text_Font_TypeDef*       font_,
                                                      uint8_t                       x_,
                                                      uint8_t                       y_,
                                                      GLCD_Driver_DispColor_TypeDef clr_);

GLCD_Status_TypeDef GLCD_Text_TrnsprntWriteStringXY ( GLCD_Handle_TypeDef*          phglcd_,
                                                      void*                         str_,
                                                      GLCD_Text_Data_TypeDef        dt_,
                                                      GLCD_Text_Font_TypeDef*       font_,
                                                      uint8_t                       x_,
                                                      uint8_t                       y_,
                                                      GLCD_Driver_DispColor_TypeDef clr_,
                                                      uint8_t                       len_,
                                                      bool                          one_line_,
                                                      bool                          cut_off_);

#endif  //__GLCD_MODULE_TEXT_H