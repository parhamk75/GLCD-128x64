#include "GLCD_L2.h"

// Utilities
uint8_t Extract_Bit(uint8_t mask_,uint8_t data_)
{
    uint8_t tmp_1 = data_ & mask_;
    if(tmp_1 == 0)
    {
        return 0;
    }
    return 1;
}

// Status Checks
GLCD_L2_DispStatOnOff_TypeDef GLCD_L2_IsDispOnOff(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_L2_StatusMask_OnOff, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_L2_StatusMask_OnOff, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left));
        return (GLCD_L2_DispStatOnOff_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_L2_DispStatOnOff_TypeDef)Extract_Bit(GLCD_L2_StatusMask_OnOff, GLCD_L1_Read_Status(pglcd_, hlf_));
}


GLCD_L2_DispStatBusy_TypeDef GLCD_L2_IsDispBusy(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_);
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_L2_StatusMask_Busy, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_L2_StatusMask_Busy, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left));
        return (GLCD_L2_DispStatBusy_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_L2_DispStatBusy_TypeDef)Extract_Bit(GLCD_L2_StatusMask_Busy, GLCD_L1_Read_Status(pglcd_, hlf_));
}


/**
  * @brief This function will read the status reg and return the value of busy flag.
  * @param pglcd_ address of a GLCD_L0_TypeDef structure
  * @param half_ if "Both" is entered the return value will be 'and' of two halves
  * @retval returns 'Busy' flag as a GLCD_L2_DispStatReset_TypeDef value.
  */
GLCD_L2_DispStatReset_TypeDef GLCD_L2_IsDispReset(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_L2_StatusMask_Reset, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_L2_StatusMask_Reset, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left));
        return (GLCD_L2_DispStatReset_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_L2_DispStatReset_TypeDef)Extract_Bit(GLCD_L2_StatusMask_Reset, GLCD_L1_Read_Status(pglcd_, hlf_));
}


uint8_t GLCD_L2_ReadDispStatus(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_);
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        return GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right) & GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left);
    }
    return GLCD_L1_Read_Status(pglcd_, hlf_);
}


// Initialization
HAL_StatusTypeDef   GLCD_L2_Init                (GLCD_L0_TypeDef* pglcd_); // Init GPIO -> LCD Reset Procedure -> Set Addresses to 0
HAL_StatusTypeDef   GLCD_L2_FullInit            (GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_); // GLCD_L2_Init() -> Set Whole Datas (Color)

// Set Whole Display Color
HAL_StatusTypeDef   GLCD_L2_SetWholeDispColor   (GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_, GLCD_L2_HALF_TypeDef hlf_); // Set Whole Datas (Color)
HAL_StatusTypeDef   GLCD_L2_ClearDisplay        (GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_, GLCD_L2_HALF_TypeDef hlf_); // GLCD_L2_SetWholeDispColor(White)

// Write Data
HAL_StatusTypeDef   GLCD_L2_WriteByte           (GLCD_L0_TypeDef* pglcd_, uint8_t data_);
HAL_StatusTypeDef   GLCD_L2_WriteByteXY         (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t data_);
HAL_StatusTypeDef   GLCD_L2_WriteByteXYZ        (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_, uint8_t data_);

// Read Data
uint8_t             GLCD_L2_ReadByte            (GLCD_L0_TypeDef* pglcd_);
uint8_t             GLCD_L2_ReadByteXY          (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_);
uint8_t             GLCD_L2_ReadByteXYZ         (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);

// Set Address Functions (Goto)
HAL_StatusTypeDef   GLCD_L2_GotoXYZ             (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoXY              (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoX               (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoY               (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
HAL_StatusTypeDef   GLCD_L2_GotoZ               (GLCD_L0_TypeDef* pglcd_, uint8_t x_, uint8_t y_, uint8_t z_);
