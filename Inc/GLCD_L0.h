// This file includes some definitions and functions
// as the basis of higher level libraries of GLCD_Lib Family
#ifndef __GLCD_L0_H
#define __GLCD_L0_H

// Includes
#include "stm32f4xx_hal.h"

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

typedef enum{
    GLCD_L0_Mode_Write  = (uint8_t)0,
    GLCD_L0_Mode_Read   = (uint8_t)1
}GLCD_L0_MODE_TypeDef;

typedef enum{
    GLCD_L0_FrameType_Data          = (uint8_t)1,
    GLCD_L0_FrameType_Instruction   = (uint8_t)0
}GLCD_L0_FrameType_TypeDef;

typedef enum{
    GLCD_L0_HALF_Right  = (uint8_t)1,
    GLCD_L0_HALF_Left   = (uint8_t)0
}GLCD_L0_HALF_TypeDef;


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
    GLCD_L0_MODE_TypeDef Mode;

}GLCD_L0_TypeDef;

HAL_StatusTypeDef   GLCD_L0_Delay       (uint16_t td_10xns_);
HAL_StatusTypeDef   GLCD_L0_Write       (GLCD_L0_TypeDef* pglcd_, uint8_t DBs_, GLCD_L0_FrameType_TypeDef frm_typ_, GLCD_L0_HALF_TypeDef hlf_);  //TODO: implement flags with a boolean data type
uint8_t             GLCD_L0_Read        (GLCD_L0_TypeDef* pglcd_, GLCD_L0_FrameType_TypeDef frm_typ_, GLCD_L0_HALF_TypeDef hlf_);  //TODO: implement flags with a boolean data type
HAL_StatusTypeDef   GLCD_L0_StartReset  (GLCD_L0_TypeDef* pglcd_);
HAL_StatusTypeDef   GLCD_L0_StopReset   (GLCD_L0_TypeDef* pglcd_);
GPIO_PinState       GLCD_L0_CheckReset  (GLCD_L0_TypeDef* pglcd_);

#endif  //_GLCD_L0_H