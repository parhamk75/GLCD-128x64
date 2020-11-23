#ifndef __GLCD_L4_T_H
#define __GLCD_L4_T_H

#include "GLCD_L3.h"
#include "stdint.h"
#include "stdbool.h"

typedef enum{
    GLCD_L4_T_Data_8bit   = (uint_fast8_t)0,
    GLCD_L4_T_Data_16bit  = (uint_fast8_t)1,
    GLCD_L4_T_Data_32bit  = (uint_fast8_t)2
}GLCD_L4_Data_TypeDef;

// The GLCD Pinout typedef structure
typedef struct{
    // Properties
    uint8_t     width;                  /* General Width of Characters */
    uint8_t     height;                 /* General Height of Characters */
    uint32_t    first_char_num;         /* First Character Number (Index) in the Font_Data Array */
    uint32_t    last_char_num;          /* Last Character Number (Index) in the Font_Data Array */

    // Data
    const uint8_t* undifined_char_data; /* Pointer to the Font Data for Undifined Character Data Array in the STD Form */
    const uint8_t* font_data;           /* Pointer to the Font Data Array in the STD Form */

}GLCD_L4_T_Font_TypeDef;

// Normal Write Funcs
HAL_StatusTypeDef   GLCD_L4_T_WriteCharXY   (GLCD_L1_TypeDef* pglcd_, uint32_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L4_T_WriteStringXY (GLCD_L1_TypeDef* pglcd_, void* str_, GLCD_L4_Data_TypeDef dt_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, uint16_t len_, bool one_line_, bool cut_off_);

HAL_StatusTypeDef   GLCD_L4_T_SetNoLines        (uint8_t no_lines_);
HAL_StatusTypeDef   GLCD_L4_T_WriteCharLine     (GLCD_L1_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L4_T_WriteStringLine   (GLCD_L1_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_);

// Transparent Write Funcs
HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteCharXY   (GLCD_L1_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_);
HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteStringXY (GLCD_L1_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_, uint8_t len_, bool one_line_, bool cut_off_);

HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteCharLine     (GLCD_L1_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_);
HAL_StatusTypeDef   GLCD_L4_T_TrnsprntWriteStringLine   (GLCD_L1_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_, GLCD_L2_DispColor_TypeDef clr_, uint8_t len_, bool one_line_, bool cut_off_);

#endif  //_GLCD_L4_T_H