#ifndef __GLCD_L4_T_H
#define __GLCD_L4_T_H

#include "GLCD_L3.h"
#include "stdint.h"
#include "stdbool.h"


// The GLCD Pinout typedef structure
typedef struct{
    // Properties
    uint8_t     Width;                  /* General Width of Characters */
    uint8_t     Height;                 /* General Height of Characters */
    uint16_t    First_char_num;         /* First Character Number (Index) in the Font_Data Array */
    uint16_t    Last_char_num;          /* Last Character Number (Index) in the Font_Data Array */

    // Data
    uint8_t* Font_Data;                 /* Pointer to the Font Data Array in the STD Form */

}GLCD_L4_T_Font_TypeDef;

HAL_StatusTypeDef   GLCD_L4_T_WriteCharXY   (GLCD_L0_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L4_T_WriteStringXY (GLCD_L0_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t x_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_);

HAL_StatusTypeDef   GLCD_L4_T_WriteCharXY_STD      (GLCD_L0_TypeDef* pglcd_, uint8_t char_, uint8_t x_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L4_T_WriteStringXY_STD    (GLCD_L0_TypeDef* pglcd_, uint8_t* str_, uint8_t x_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_);

HAL_StatusTypeDef   GLCD_L4_T_SetNoLines        (uint8_t no_lines_);
HAL_StatusTypeDef   GLCD_L4_T_WriteCharLine     (GLCD_L0_TypeDef* pglcd_, uint16_t char_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_);
HAL_StatusTypeDef   GLCD_L4_T_WriteStringLine   (GLCD_L0_TypeDef* pglcd_, uint16_t* str_, GLCD_L4_T_Font_TypeDef* font_, uint8_t line_, uint8_t y_, uint8_t len_, bool one_line_, bool cut_off_);

#endif  //_GLCD_L4_T_H