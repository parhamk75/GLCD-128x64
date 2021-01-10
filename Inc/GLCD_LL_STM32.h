// This file includes some definitions and functions
// as the basis of higher level libraries of GLCD_Lib Family
#ifndef __GLCD_LL_H
#define __GLCD_LL_H

// Includes
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "stdbool.h"

// MPU Definitions
#define GLCD_LL_MPU_10NS_CNST   ((uint8_t)1)          // MPU Clock Period in 10x nanosecond

// Timing Parameters [in 10x nano Seconds]
#define GLCD_LL_T_C             ((uint8_t)100)        // E Cycle <Min>
#define GLCD_LL_T_WH            ((uint8_t)45)         // E High Level Width <Min>
#define GLCD_LL_T_WL            ((uint8_t)45)         // E Low Level Width <Min>
#define GLCD_LL_T_R             ((uint8_t)2)          // E Rise Time <Max>
#define GLCD_LL_T_F             ((uint8_t)2)          // E Fall Time <Max>
#define GLCD_LL_T_ASU           ((uint8_t)14)         // Address Set-Up Time <Min>
#define GLCD_LL_T_AH            ((uint8_t)1)          // Address Hold Time <Min>
#define GLCD_LL_T_DSU           ((uint8_t)20)         // Data Set-Up Time <Min>
#define GLCD_LL_T_D             ((uint8_t)32)         // Data Delay Time <Max>
#define GLCD_LL_T_DHW           ((uint8_t)1)          // Data Hold Time (Write) <Min>
#define GLCD_LL_T_DHR           ((uint8_t)2)          // Data Delay Time (Read) <Min>
#define GLCD_LL_T_RS            ((uint8_t)100)        // Reset Time <Min>

#define GLCD_LL_ROW_PIXELS      ((uint8_t)128)        // GLCD Dimentions -> Number of pixels in one Row
#define GLCD_LL_COL_PIXELS      ((uint8_t)64)         // GLCD Dimentions -> Number of pixels in one Column

#define GLCD_LL_X_MAX           ((uint8_t)7)         // X Address <Max>
#define GLCD_LL_Y_MAX           ((uint8_t)63)        // Y Address <Max>
#define GLCD_LL_Z_MAX           ((uint8_t)63)        // Z Address <Max>


typedef enum 
{
  GLCD_OK                           = 0x00U,
  GLCD_ERROR                        = 0x01U,
  GLCD_BUSY                         = 0x02U,
  GLCD_TIMEOUT                      = 0x03U
} GLCD_Status_TypeDef;

typedef enum{
    GLCD_LL_Mode_Write              = (uint8_t)0,
    GLCD_LL_Mode_Read               = (uint8_t)1
}GLCD_LL_Mode_TypeDef;

typedef enum{
    GLCD_LL_FrameType_Data          = (uint8_t)1,
    GLCD_LL_FrameType_Instruction   = (uint8_t)0
}GLCD_LL_FrameType_TypeDef;

typedef enum{
    GLCD_LL_CS_1                    = (uint8_t)1,
    GLCD_LL_CS_2                    = (uint8_t)0
}GLCD_LL_ChipSelect_TypeDef;


// The GLCD LL typedef structure
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
    GLCD_LL_Mode_TypeDef Mode;

}GLCD_LL_TypeDef;

// The GLCD Buffer typedef structure (It Stores Data and Addresses)
typedef struct{
    // Address Buffers
    uint8_t x;
    uint8_t y;
    uint8_t z;

    // Data Buffer
    uint8_t data[GLCD_LL_ROW_PIXELS *GLCD_LL_COL_PIXELS /8 /2];

}GLCD_LL_Buffer_TypeDef;

// The GLCD handler typedef structure (Adds buffering features to GLCD_LL_TypeDef)
typedef struct{
    // Pointer to a GLCD_LL_TypeDef instance
    GLCD_LL_TypeDef*            pglcd_ll;
    
    // Pointer to GLCD_LL_Buffer_TypeDef instances for right and left halves
    GLCD_LL_Buffer_TypeDef*     pbuffer_right;
    GLCD_LL_Buffer_TypeDef*     pbuffer_left;

    // Buffering flags
    bool                        buffered;
    bool                        synchronized;

}GLCD_Handle_TypeDef;


GLCD_Status_TypeDef   GLCD_LL_Delay         (   uint16_t td_10xns_);                    //TODO: Inline

GLCD_Status_TypeDef   GLCD_LL_Write         (   GLCD_LL_TypeDef* pglcd_ll_,
                                                uint8_t DBs_,
                                                GLCD_LL_FrameType_TypeDef frm_typ_,
                                                GLCD_LL_ChipSelect_TypeDef cs_);        //TODO: implement flags with a boolean data type

uint8_t               GLCD_LL_Read          (   GLCD_LL_TypeDef* pglcd_ll_,
                                                GLCD_LL_FrameType_TypeDef frm_typ_,
                                                GLCD_LL_ChipSelect_TypeDef cs_);        //TODO: implement flags with a boolean data type

GLCD_Status_TypeDef   GLCD_LL_StartReset    (   GLCD_LL_TypeDef* pglcd_ll_);

GLCD_Status_TypeDef   GLCD_LL_StopReset     (   GLCD_LL_TypeDef* pglcd_ll_);

GPIO_PinState         GLCD_LL_CheckReset    (   GLCD_LL_TypeDef* pglcd_ll_);

GLCD_Status_TypeDef   GLCD_LL_Init          (   GLCD_LL_TypeDef* pglcd_ll_);


#endif  //_GLCD_LL_H