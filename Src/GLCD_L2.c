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


GLCD_L2_DispStatBusy_TypeDef GLCD_L2_IsDispBusy(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
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
  * @return 'Busy' flag as a GLCD_L2_DispStatReset_TypeDef value.
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


uint8_t GLCD_L2_ReadDispStatus(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        return GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right) & GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left);
    }
    return GLCD_L1_Read_Status(pglcd_, hlf_);
}


// Initialization
HAL_StatusTypeDef GLCD_L2_Init(GLCD_L0_TypeDef* pglcd_)
{
    // GPIO Initializations
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // ~ RS
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->RS_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->RS_Port, &GPIO_InitStruct);

    // ~ R/W
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->RW_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->RW_Port, &GPIO_InitStruct);

    // ~ CS1, CS2
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->CS1_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->CS1_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin   = pglcd_->CS2_Pin;
    HAL_GPIO_Init(pglcd_->CS2_Port, &GPIO_InitStruct);

    // ~ EN
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->EN_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->EN_Port, &GPIO_InitStruct);

    // ~ RST
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->RST_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->RST_Port, &GPIO_InitStruct);

    // ~ DBs
    if(pglcd_->Mode == GLCD_L0_Mode_Write)
    {
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    }
    else
    {
        GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pin   = pglcd_->DB_Pins[i];
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(pglcd_->DB_Ports[i], &GPIO_InitStruct);
    }

    // Perform a Reset procedure
    GLCD_L0_StartReset(pglcd_);
    GLCD_L0_Delay(GLCD_L0_T_RS* 10);
    GLCD_L0_StopReset(pglcd_);
    while(GLCD_L2_IsDispReset(pglcd_, GLCD_L2_HALF_Both) == GLCD_L2_DispStatReset_InReset);
    
    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_FullInit(GLCD_L0_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_)
{
    GLCD_L2_Init(pglcd_);
    GLCD_L2_SetWholeDispColor(pglcd_, GLCD_L2_HALF_Both, init_whole_dsp_clr_);
    return HAL_OK;
}



// Set Whole Display Color
HAL_StatusTypeDef GLCD_L2_SetWholeDispColor(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_)
{
    if(GLCD_L2_GotoXYZ(pglcd_, hlf_, 0, 0, 0) == HAL_BUSY){return HAL_BUSY;}

    for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
    {
        GLCD_L2_GotoX(pglcd_, GLCD_L2_HALF_Both, i);
        for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
        {
            HAL_StatusTypeDef tmp_state = GLCD_L2_WriteByte(pglcd_, hlf_, (uint8_t)init_whole_dsp_clr_* 0xFF);
            if( tmp_state != HAL_OK )
            {
                if( tmp_state == HAL_BUSY )
                {
                    while(GLCD_L2_WriteByte(pglcd_, hlf_, (uint8_t)init_whole_dsp_clr_* 0xFF) != HAL_OK );
                }
            }
        }
    }
    
    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_ClearDisplay(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    return GLCD_L2_SetWholeDispColor(pglcd_, GLCD_L2_HALF_Both, GLCD_L2_DispColor_White);
}


// Write Data
HAL_StatusTypeDef GLCD_L2_WriteByte(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_)
{
    if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Write_DispData(pglcd_, GLCD_L2_HALF_Left, data_);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Right) == GLCD_L2_DispStatBusy_Busy);
        GLCD_L1_Write_DispData(pglcd_, GLCD_L2_HALF_Right, data_);

        return HAL_OK;
    }
    GLCD_L1_Write_DispData(pglcd_, hlf_, data_);

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_WriteByteXY(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_)
{
    if(GLCD_L2_GotoXY(pglcd_, hlf_, x_, y_) == HAL_BUSY){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Write_DispData(pglcd_, GLCD_L2_HALF_Left, data_);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Right) == GLCD_L2_DispStatBusy_Busy);
        GLCD_L1_Write_DispData(pglcd_, GLCD_L2_HALF_Right, data_);

        return HAL_OK;
    }
    GLCD_L1_Write_DispData(pglcd_, hlf_, data_);

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_WriteByteXYZ(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    if(GLCD_L2_GotoXYZ(pglcd_, hlf_, x_, y_, z_) == HAL_BUSY){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Write_DispData(pglcd_, GLCD_L2_HALF_Left, data_);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Right) == GLCD_L2_DispStatBusy_Busy);
        GLCD_L1_Write_DispData(pglcd_, GLCD_L2_HALF_Right, data_);

        return HAL_OK;
    }
    GLCD_L1_Write_DispData(pglcd_, hlf_, data_);

    return HAL_OK;
}



// Read Data
uint8_t GLCD_L2_ReadByte(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Left);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Left) == GLCD_L2_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Left);
        GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Right);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Right) == GLCD_L2_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Right);

        return tmp_l & tmp_r;
    }
    GLCD_L1_Read_DispData(pglcd_, hlf_);    // Dummy read request (Reason in datasheet!)
    while(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy);
    
    return GLCD_L1_Read_DispData(pglcd_, hlf_);
}


uint8_t GLCD_L2_ReadByteXY(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_)
{
    if(GLCD_L2_GotoXY(pglcd_, hlf_, x_, y_) == HAL_BUSY){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Left);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Left) == GLCD_L2_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Left);
        GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Right);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Right) == GLCD_L2_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Right);

        return tmp_l & tmp_r;
    }
    GLCD_L1_Read_DispData(pglcd_, hlf_);
    while(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy);
    
    return GLCD_L1_Read_DispData(pglcd_, hlf_);
}


uint8_t GLCD_L2_ReadByteXYZ(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    if(GLCD_L2_GotoXYZ(pglcd_, hlf_, x_, y_, z_) == HAL_BUSY){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Left);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Left) == GLCD_L2_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Left);
        GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Right);
        while(GLCD_L2_IsDispBusy(pglcd_, GLCD_L2_HALF_Right) == GLCD_L2_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_L1_Read_DispData(pglcd_, GLCD_L2_HALF_Right);

        return tmp_l & tmp_r;
    }
    
    GLCD_L1_Read_DispData(pglcd_, hlf_);
    while(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy);
    
    return GLCD_L1_Read_DispData(pglcd_, hlf_);
}


// Set Address Functions (Goto)

HAL_StatusTypeDef GLCD_L2_GotoX(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_)
{
    if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_Page(pglcd_, GLCD_L2_HALF_Right, x_);
        GLCD_L1_Set_Page(pglcd_, GLCD_L2_HALF_Left, x_);
    }
    else
    {
        GLCD_L1_Set_Page(pglcd_, hlf_, x_);
    }

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_GotoY(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t y_)
{
    if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_Address(pglcd_, GLCD_L2_HALF_Right, y_);
        GLCD_L1_Set_Address(pglcd_, GLCD_L2_HALF_Left, y_);
    }
    else
    {
        GLCD_L1_Set_Address(pglcd_, hlf_, y_);
    }

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_GotoZ(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t z_)
{
    if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_DispStartLine(pglcd_, GLCD_L2_HALF_Right, z_);
        GLCD_L1_Set_DispStartLine(pglcd_, GLCD_L2_HALF_Left, z_);
    }
    else
    {
        GLCD_L1_Set_DispStartLine(pglcd_, hlf_, z_);
    }

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_GotoXY(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_)
{
    if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_Page(pglcd_, GLCD_L2_HALF_Right, x_);
        GLCD_L1_Set_Page(pglcd_, GLCD_L2_HALF_Left, x_);
    }
    else
    {
        GLCD_L1_Set_Page(pglcd_, hlf_, x_);
    }
    while(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy);
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_Address(pglcd_, GLCD_L2_HALF_Right, y_);
        GLCD_L1_Set_Address(pglcd_, GLCD_L2_HALF_Left, y_);
    }
    else
    {
        GLCD_L1_Set_Address(pglcd_, hlf_, y_);
    }

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_GotoXYZ(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_Page(pglcd_, GLCD_L2_HALF_Right, x_);
        GLCD_L1_Set_Page(pglcd_, GLCD_L2_HALF_Left, x_);
    }
    else
    {
        GLCD_L1_Set_Page(pglcd_, hlf_, x_);
    }
    while(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy);
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_Address(pglcd_, GLCD_L2_HALF_Right, y_);
        GLCD_L1_Set_Address(pglcd_, GLCD_L2_HALF_Left, y_);
    }
    else
    {
        GLCD_L1_Set_Address(pglcd_, hlf_, y_);
    }
    while(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy);
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        GLCD_L1_Set_DispStartLine(pglcd_, GLCD_L2_HALF_Right, z_);
        GLCD_L1_Set_DispStartLine(pglcd_, GLCD_L2_HALF_Left, z_);
    }
    else
    {
        GLCD_L1_Set_DispStartLine(pglcd_, hlf_, z_);
    }

    return HAL_OK;
}


// Complex Write Functions
HAL_StatusTypeDef GLCD_L2_TrnsprntWriteByte(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, GLCD_L2_DispColor_TypeDef clr_)
{
    uint8_t tmp_prev_data = GLCD_L2_ReadByte(pglcd_, hlf_);

    if(clr_ == GLCD_L2_DispColor_Black)
    {
        return GLCD_L2_WriteByte(pglcd_, hlf_, data_ | tmp_prev_data);
    }
    else
    {
        return GLCD_L2_WriteByte(pglcd_, hlf_, data_ & tmp_prev_data);
    }

    return HAL_ERROR;
}


HAL_StatusTypeDef GLCD_L2_TrnsprntWriteByteXY(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, GLCD_L2_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_)
{
    uint8_t tmp_prev_data = GLCD_L2_ReadByteXY(pglcd_, hlf_, x_, y_);

    if(clr_ == GLCD_L2_DispColor_Black)
    {
        return GLCD_L2_WriteByteXY(pglcd_, hlf_, data_ | tmp_prev_data, x_, y_);
    }
    else
    {
        return GLCD_L2_WriteByteXY(pglcd_, hlf_, data_ & tmp_prev_data, x_, y_);
    }

    return HAL_ERROR;
}


HAL_StatusTypeDef GLCD_L2_TrnsprntWriteByteXYZ(GLCD_L0_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, GLCD_L2_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    uint8_t tmp_prev_data = GLCD_L2_ReadByteXYZ(pglcd_, hlf_, x_, y_, z_);

    if(clr_ == GLCD_L2_DispColor_Black)
    {
        return GLCD_L2_WriteByteXYZ(pglcd_, hlf_, data_ | tmp_prev_data, x_, y_, z_);
    }
    else
    {
        return GLCD_L2_WriteByteXYZ(pglcd_, hlf_, data_ & tmp_prev_data, x_, y_, z_);
    }

    return HAL_ERROR;
}



