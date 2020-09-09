// This file includes some definitions and functions
// as the basis of higher level libraries of GLCD_Lib Family
#ifndef __GLCD_L0_H
#define __GLCD_L0_H

// Timing Parameters [in nano Seconds]
#define GLCD_L0_T_C     ((uint16_t)1000)        // E Cycle
#define GLCD_L0_T_WH    ((uint16_t)450)         // E High Level Width
#define GLCD_L0_T_WL    ((uint16_t)450)         // E Low Level Width
#define GLCD_L0_T_R     ((uint16_t)25)          // E Rise Time <Max>
#define GLCD_L0_T_F     ((uint16_t)25)          // E Fall Time <Max>
#define GLCD_L0_T_ASU   ((uint16_t)140)         // Address Set-Up Time
#define GLCD_L0_T_AH    ((uint16_t)10)          // Address Hold Time
#define GLCD_L0_T_SU    ((uint16_t)200)         // Data Set-Up Time
#define GLCD_L0_T_D     ((uint16_t)320)         // Data Delay Time <Max>
#define GLCD_L0_T_DHW   ((uint16_t)10)          // Data Hold Time (Write)
#define GLCD_L0_T_DHR   ((uint16_t)20)          // Data Delay Time (Read)



#endif  _GLCD_L0_H