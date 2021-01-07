#ifndef __GLCD_L3_H
#define __GLCD_L3_H

#include "GLCD_Driver_KS0108.h"
#include "stdbool.h"
#include "stdint.h"

#define GLCD_L3_X_MAX            ((uint8_t)127)         // X <Max>
#define GLCD_L3_Y_MAX            ((uint8_t)63)          // Y <Max>

typedef enum{
    GLCD_L3_DispColor_Black     = (uint8_t)GLCD_Driver_DispColor_Black,
    GLCD_L3_DispColor_White     = (uint8_t)GLCD_Driver_DispColor_White
}GLCD_L3_DispColor_TypeDef;

typedef enum{
    GLCD_L3_DispOnOff_Off       = (uint8_t)GLCD_Driver_DispStatOnOff_Off,
    GLCD_L3_DispOnOff_On        = (uint8_t)GLCD_Driver_DispStatOnOff_On
}GLCD_L3_DispOnOff_TypeDef;


// Maintanance Functions
/**
  * @brief  Initialize display and show initial color on it (Entire screen)
  * @param  phglcd_      a pointer to GLCD Handler (L1 type)
  * @param  int_clr_    the desired color to be shown on whole display
  * @note   This is actually just a function call for the analogous func in L2
  * @retval HAL status
*/
GLCD_Status_TypeDef GLCD_L3_Init(GLCD_Handle_TypeDef* phglcd_, GLCD_L3_DispColor_TypeDef int_clr_);
/**
  * @brief  Turn display on or off
  * @param  phglcd_  a pointer to GLCD Handler (L1 type)
  * @param  on_off_ turn display into this state (on/off)
  * @note   This is actually just a function call for the analogous func in L2
  * @retval HAL status
*/
GLCD_Status_TypeDef GLCD_L3_OnOff(GLCD_Handle_TypeDef* phglcd_, GLCD_L3_DispOnOff_TypeDef on_off_);
/**
  * @brief  Clear display and show the color on it (Entire screen)
  * @param  phglcd_  a pointer to GLCD Handler (L1 type)
  * @param  clr_    the color want to be shown on entire screen
  * @note   This is actually just a function call for the analogous func in L2
  * @retval HAL status
*/
GLCD_Status_TypeDef GLCD_L3_ClearScreen(GLCD_Handle_TypeDef* phglcd_, GLCD_L3_DispColor_TypeDef clr_);

// Positioning Functions
/**
  * @brief  Scrolling the content of display along Y axis
  * @param  phglcd_  a pointer to GLCD Handler (L1 type)
  * @param  scrl_   the amount of scrolling along Y axis <pixels>
  * @note   Positive values will scroll up, negative values will scroll down,
  *         and zero (0) will reset the scrolling position.
  * @note   this function sets the Z address register (Display Start Line) 
  * @retval HAL status
*/
GLCD_Status_TypeDef GLCD_L3_ScrollY(GLCD_Handle_TypeDef* phglcd_, int8_t scrl_);
// GLCD_Status_TypeDef GLCD_L3_GotoXY(GLCD_LL_TypeDef* pglcd_ uint8_t x_, uint8_t y_);
// GLCD_Status_TypeDef GLCD_L3_GotoOrigin(GLCD_LL_TypeDef* phglcd_);  // x, y, z -> 0


// Write Functions
// GLCD_Status_TypeDef GLCD_L3_Write(GLCD_LL_TypeDef* phglcd_, GLCD_L3_DispColor_TypeDef clr_);
/**
  * @brief  Write a special color in a pixel in determined position
  * @param  phglcd_      a pointer to GLCD Handler (L1 type)
  * @param  clr_        the desired color to write
  * @param  x_          X of the pixel position <pixels>
  * @param  y_          Y of the pixel position <pixels>
  * @param  is_crclr_   determines whether or not the entered values for x and y
  *                     are circular. It returns with error status if x and/or y
  *                     values are larger than max and is_crclr_ is false. 
  * @retval HAL status
*/
GLCD_Status_TypeDef GLCD_L3_WriteXY(GLCD_Handle_TypeDef* phglcd_, GLCD_L3_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, bool is_crclr_);
/**
  * @brief  Write a bitmap in determined position
  * @param  phglcd_      a pointer to GLCD Handler (L1 type)
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
  * @retval HAL status
*/
GLCD_Status_TypeDef GLCD_L3_WriteBitmap(GLCD_Handle_TypeDef* phglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, bool is_crclr_);
/**
  * @brief  Add a bitmap to the content of the determined position
  * @param  phglcd_      a pointer to GLCD Handler (L1 type)
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
  * @retval HAL status
*/
GLCD_Status_TypeDef GLCD_L3_TrnsprntWriteBitmap(GLCD_Handle_TypeDef* phglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, GLCD_L3_DispColor_TypeDef clr_, bool is_crclr_);


#endif  //_GLCD_L3_H