// This file includes some definitions and functions
// as the basis of higher level libraries of GLCD_Lib Family
#ifndef __GLCD_L0_H
#define __GLCD_L0_H

// Includes
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "stdbool.h"

// MPU Definitions
#define GLCD_L0_MPU_10NS_CNST   ((uint8_t)1)          // MPU Clock Period in 10x nanosecond

// Timing Parameters [in 10x nano Seconds]
#define GLCD_L0_T_C             ((uint8_t)100)        // E Cycle <Min>
#define GLCD_L0_T_WH            ((uint8_t)45)         // E High Level Width <Min>
#define GLCD_L0_T_WL            ((uint8_t)45)         // E Low Level Width <Min>
#define GLCD_L0_T_R             ((uint8_t)2)          // E Rise Time <Max>
#define GLCD_L0_T_F             ((uint8_t)2)          // E Fall Time <Max>
#define GLCD_L0_T_ASU           ((uint8_t)14)         // Address Set-Up Time <Min>
#define GLCD_L0_T_AH            ((uint8_t)1)          // Address Hold Time <Min>
#define GLCD_L0_T_DSU           ((uint8_t)20)         // Data Set-Up Time <Min>
#define GLCD_L0_T_D             ((uint8_t)32)         // Data Delay Time <Max>
#define GLCD_L0_T_DHW           ((uint8_t)1)          // Data Hold Time (Write) <Min>
#define GLCD_L0_T_DHR           ((uint8_t)2)          // Data Delay Time (Read) <Min>
#define GLCD_L0_T_RS            ((uint8_t)100)        // Reset Time <Min>

#define GLCD_L0_ROW_PIXELS      ((uint8_t)128)        // GLCD Dimentions -> Number of pixels in one Row
#define GLCD_L0_COL_PIXELS      ((uint8_t)64)         // GLCD Dimentions -> Number of pixels in one Column

#define GLCD_L0_X_MAX            ((uint8_t)7)         // X Address <Max>
#define GLCD_L0_Y_MAX            ((uint8_t)63)        // Y Address <Max>
#define GLCD_L0_Z_MAX            ((uint8_t)63)        // Z Address <Max>


typedef enum 
{
  GLCD_OK       = 0x00U,
  GLCD_ERROR    = 0x01U,
  GLCD_BUSY     = 0x02U,
  GLCD_TIMEOUT  = 0x03U
} GLCD_Status_TypeDef;

typedef enum{
    GLCD_L0_Mode_Write  = (uint8_t)0,
    GLCD_L0_Mode_Read   = (uint8_t)1
}GLCD_L0_Mode_TypeDef;

typedef enum{
    GLCD_L0_FrameType_Data          = (uint8_t)1,
    GLCD_L0_FrameType_Instruction   = (uint8_t)0
}GLCD_L0_FrameType_TypeDef;

typedef enum{
    GLCD_L0_CS_1    = (uint8_t)1,
    GLCD_L0_CS_2    = (uint8_t)0
}GLCD_L0_ChipSelect_TypeDef;


// The GLCD Pinout typedef structure
typedef struct{
    // GPIO Ports
    GPIO_TypeDef* DB_Ports[8];
    GPIO_TypeDef* EN_Port;
    GPIO_TypeDef* RS_Port;
    GPIO_TypeDef* RW_Port;
    GPIO_TypeDef* RST_Port;
    GPIO_TypeDef* CS1_Port;
    GPIO_TypeDef* CS2_Port;

    // GPIO Pins
    uint16_t DB_Pins[8];
    uint16_t EN_Pin;
    uint16_t RS_Pin;
    uint16_t RW_Pin;
    uint16_t RST_Pin;
    uint16_t CS1_Pin;
    uint16_t CS2_Pin;

    // GLCD Last Mode Flag (Read/Write)
    GLCD_L0_Mode_TypeDef Mode;

}GLCD_L0_TypeDef;

// The GLCD Buffer typedef structure (It Stores Data and Addresses)
typedef struct{
    // Address Buffers
    uint8_t x;
    uint8_t y;
    uint8_t z;

    // Data Buffer
    uint8_t data[GLCD_L0_ROW_PIXELS *GLCD_L0_COL_PIXELS /8 /2];

}GLCD_L0_Buffer_TypeDef;

// The GLCD_L1 typedef structure (Adds buffering features to GLCD_L0_TypeDef)
typedef struct{
    // Pointer to a GLCD_L0_TypeDef instance
    GLCD_L0_TypeDef*            pglcd0;
    
    // Pointer to GLCD_L0_Buffer_TypeDef instances for right and left halves
    GLCD_L0_Buffer_TypeDef*     pbuffer_right;
    GLCD_L0_Buffer_TypeDef*     pbuffer_left;

    // Buffering flags
    bool                        buffered;
    bool                        synchronized;

}GLCD_HandleTypeDef;

GLCD_Status_TypeDef   GLCD_L0_Delay       (uint16_t td_10xns_);
GLCD_Status_TypeDef   GLCD_L0_Write       (GLCD_L0_TypeDef* pglcd_, uint8_t DBs_, GLCD_L0_FrameType_TypeDef frm_typ_, GLCD_L0_ChipSelect_TypeDef cs_);  //TODO: implement flags with a boolean data type
uint8_t               GLCD_L0_Read        (GLCD_L0_TypeDef* pglcd_, GLCD_L0_FrameType_TypeDef frm_typ_, GLCD_L0_ChipSelect_TypeDef cs_);  //TODO: implement flags with a boolean data type
GLCD_Status_TypeDef   GLCD_L0_StartReset  (GLCD_L0_TypeDef* pglcd_);
GLCD_Status_TypeDef   GLCD_L0_StopReset   (GLCD_L0_TypeDef* pglcd_);
GPIO_PinState         GLCD_L0_CheckReset  (GLCD_L0_TypeDef* pglcd_);
GLCD_Status_TypeDef   GLCD_L1_Init        (GLCD_HandleTypeDef* pglcd_); // Init GPIO -> LCD Reset Procedure -> Set Addresses to 0


#endif  //_GLCD_L0_H