#ifndef __GLCD_L3_H
#define __GLCD_L3_H

#include "GLCD_L1.h"
#include "stdbool.h"
#include "stdint.h"

#define GLCD_L3_X_MAX            ((uint8_t)127)         // X <Max>
#define GLCD_L3_Y_MAX            ((uint8_t)63)          // Y <Max>


// Maintanance Functions
/**
  * @brief  Initialize display and show initial color on it (Entire screen)
  * @param  pglcd_      a pointer to GLCD Handler (L1 type)
  * @param  int_clr_    the desired color to be shown on whole display
  * @note   This is actually just a function call for the analogous func in L2
  * @retval HAL status
*/
HAL_StatusTypeDef GLCD_L3_Init(GLCD_L1_TypeDef* pglcd_, GLCD_L1_DispColor_TypeDef int_clr_);
/**
  * @brief  Turn display on or off
  * @param  pglcd_  a pointer to GLCD Handler (L1 type)
  * @param  on_off_ turn display into this state (on/off)
  * @note   This is actually just a function call for the analogous func in L2
  * @retval HAL status
*/
HAL_StatusTypeDef GLCD_L3_OnOff(GLCD_L1_TypeDef* pglcd_, GLCD_L1_Disp_OnOff_TypeDef on_off_);
/**
  * @brief  Clear display and show the color on it (Entire screen)
  * @param  pglcd_  a pointer to GLCD Handler (L1 type)
  * @param  clr_    the color want to be shown on entire screen
  * @note   This is actually just a function call for the analogous func in L2
  * @retval HAL status
*/
HAL_StatusTypeDef GLCD_L3_ClearScreen(GLCD_L1_TypeDef* pglcd_, GLCD_L1_DispColor_TypeDef clr_);

// Positioning Functions
/**
  * @brief  Scrolling the content of display along Y axis
  * @param  pglcd_  a pointer to GLCD Handler (L1 type)
  * @param  scrl_   the amount of scrolling along Y axis <pixels>
  * @note   Positive values will scroll up, negative values will scroll down,
  *         and zero (0) will reset the scrolling position.
  * @note   this function sets the Z address register (Display Start Line) 
  * @retval HAL status
*/
HAL_StatusTypeDef GLCD_L3_ScrollY(GLCD_L1_TypeDef* pglcd_, int8_t scrl_);
// HAL_StatusTypeDef GLCD_L3_GotoXY(GLCD_L0_TypeDef* pglcd_ uint8_t x_, uint8_t y_);
// HAL_StatusTypeDef GLCD_L3_GotoOrigin(GLCD_L0_TypeDef* pglcd_);  // x, y, z -> 0


// Write Functions
// HAL_StatusTypeDef GLCD_L3_Write(GLCD_L0_TypeDef* pglcd_, GLCD_L1_DispColor_TypeDef clr_);
/**
  * @brief  Write a special color in a pixel in determined position
  * @param  pglcd_      a pointer to GLCD Handler (L1 type)
  * @param  clr_        the desired color to write
  * @param  x_          X of the pixel position <pixels>
  * @param  y_          Y of the pixel position <pixels>
  * @param  is_crclr_   determines whether or not the entered values for x and y
  *                     are circular. It returns with error status if x and/or y
  *                     values are larger than max and is_crclr_ is false. 
  * @retval HAL status
*/
HAL_StatusTypeDef GLCD_L3_WriteXY(GLCD_L1_TypeDef* pglcd_, GLCD_L1_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, bool is_crclr_);
/**
  * @brief  Write a bitmap in determined position
  * @param  pglcd_      a pointer to GLCD Handler (L1 type)
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
HAL_StatusTypeDef GLCD_L3_WriteBitmap(GLCD_L1_TypeDef* pglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, bool is_crclr_);
/**
  * @brief  Add a bitmap to the content of the determined position
  * @param  pglcd_      a pointer to GLCD Handler (L1 type)
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
HAL_StatusTypeDef GLCD_L3_TrnsprntWriteBitmap(GLCD_L1_TypeDef* pglcd_, uint8_t* pdata_, uint8_t x_, uint8_t y_, uint8_t sizex_, uint8_t sizey_, GLCD_L1_DispColor_TypeDef clr_, bool is_crclr_);


#endif  //_GLCD_L3_H