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
GLCD_L2_DispStatOnOff_TypeDef GLCD_L2_IsDispOnOff(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_L2_StatusMask_OnOff, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_L2_StatusMask_OnOff, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left));
        return (GLCD_L2_DispStatOnOff_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_L2_DispStatOnOff_TypeDef)Extract_Bit(GLCD_L2_StatusMask_OnOff, GLCD_L1_Read_Status(pglcd_, hlf_));
}


GLCD_L2_DispStatBusy_TypeDef GLCD_L2_IsDispBusy(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
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
GLCD_L2_DispStatReset_TypeDef GLCD_L2_IsDispReset(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_L2_StatusMask_Reset, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_L2_StatusMask_Reset, GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left));
        return (GLCD_L2_DispStatReset_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_L2_DispStatReset_TypeDef)Extract_Bit(GLCD_L2_StatusMask_Reset, GLCD_L1_Read_Status(pglcd_, hlf_));
}


uint8_t GLCD_L2_ReadDispStatus(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    if(hlf_ == GLCD_L2_HALF_Both)
    {
        return GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Right) & GLCD_L1_Read_Status(pglcd_, GLCD_L2_HALF_Left);
    }
    return GLCD_L1_Read_Status(pglcd_, hlf_);
}


// Initialization
HAL_StatusTypeDef GLCD_L2_Init(GLCD_L1_TypeDef* pglcd_)
{
    // GPIO Initializations
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // ~ RS
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->pglcd0->RS_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->pglcd0->RS_Port, &GPIO_InitStruct);

    // ~ R/W
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->pglcd0->RW_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->pglcd0->RW_Port, &GPIO_InitStruct);

    // ~ CS1, CS2
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->pglcd0->CS1_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->pglcd0->CS1_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin   = pglcd_->pglcd0->CS2_Pin;
    HAL_GPIO_Init(pglcd_->pglcd0->CS2_Port, &GPIO_InitStruct);

    // ~ EN
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->pglcd0->EN_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->pglcd0->EN_Port, &GPIO_InitStruct);

    // ~ RST
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_->pglcd0->RST_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_->pglcd0->RST_Port, &GPIO_InitStruct);

    // ~ DBs
    if(pglcd_->pglcd0->Mode == GLCD_L0_Mode_Write)
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
        GPIO_InitStruct.Pin   = pglcd_->pglcd0->DB_Pins[i];
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(pglcd_->pglcd0->DB_Ports[i], &GPIO_InitStruct);
    }

    // Perform a Reset procedure
    GLCD_L1_StartReset(pglcd_);
    GLCD_L1_Delay(GLCD_L0_T_RS* 10);
    GLCD_L1_StopReset(pglcd_);
    while(GLCD_L2_IsDispReset(pglcd_, GLCD_L2_HALF_Both) == GLCD_L2_DispStatReset_InReset);
    
    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_FullInit(GLCD_L1_TypeDef* pglcd_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_)
{
    GLCD_L2_Init(pglcd_);
    GLCD_L2_SetWholeDispColor(pglcd_, GLCD_L2_HALF_Both, init_whole_dsp_clr_);
    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_OnOff(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, GLCD_L1_Disp_OnOff_TypeDef on_off_)
{
    if( hlf_ == GLCD_L2_HALF_Right )
    {
        return GLCD_L1_Disp_OnOff(pglcd_, GLCD_L0_HALF_Right, on_off_);
    }
    else if( hlf_ == GLCD_L2_HALF_Left )
    {
        return GLCD_L1_Disp_OnOff(pglcd_, GLCD_L0_HALF_Left, on_off_);
    }
    else
    {
        if( GLCD_L1_Disp_OnOff(pglcd_, GLCD_L0_HALF_Right, on_off_) == HAL_OK )
        {
            return GLCD_L1_Disp_OnOff(pglcd_, GLCD_L0_HALF_Left, on_off_);
        }
    }

    
    return HAL_ERROR;
}



// Set Whole Display Color
HAL_StatusTypeDef GLCD_L2_SetWholeDispColor(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, GLCD_L2_DispColor_TypeDef init_whole_dsp_clr_)
{
    if(GLCD_L2_GotoXY(pglcd_, hlf_, 0, 0) == HAL_BUSY){return HAL_BUSY;}

    if(hlf_ == GLCD_L2_HALF_Both)
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_L2_GotoX(pglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Right, i, j);
                HAL_StatusTypeDef tmp_state = GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Right, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Right, (uint8_t)init_whole_dsp_clr_* 0xFF) != HAL_OK );
                    }
                }
                GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Left, i, j);
                tmp_state = GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Left, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Left, (uint8_t)init_whole_dsp_clr_* 0xFF) != HAL_OK );
                    }
                }
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L2_GotoXY(pglcd_, hlf_, i, j);
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
    }
    
    
    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_ClearDisplay(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    return GLCD_L2_SetWholeDispColor(pglcd_, GLCD_L2_HALF_Both, GLCD_L2_DispColor_White);
}


// Write Data
HAL_StatusTypeDef GLCD_L2_WriteByte(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_)
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


HAL_StatusTypeDef GLCD_L2_WriteByteXY(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_)
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


HAL_StatusTypeDef GLCD_L2_WriteByteXYZ(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_, uint8_t z_)
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
uint8_t GLCD_L2_ReadByte(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or HAL_BUSY!!!
    // if(GLCD_L2_IsDispBusy(pglcd_, hlf_) == GLCD_L2_DispStatBusy_Busy){return HAL_BUSY;}
    
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


uint8_t GLCD_L2_ReadByteXY(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or HAL_BUSY!!!
    // if(GLCD_L2_GotoXY(pglcd_, hlf_, x_, y_) == HAL_BUSY){return HAL_BUSY;}
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


uint8_t GLCD_L2_ReadByteXYZ(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or HAL_BUSY!!!
    // if(GLCD_L2_GotoXYZ(pglcd_, hlf_, x_, y_, z_) == HAL_BUSY){return HAL_BUSY;}
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

HAL_StatusTypeDef GLCD_L2_GotoX(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_)
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


HAL_StatusTypeDef GLCD_L2_GotoY(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t y_)
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


HAL_StatusTypeDef GLCD_L2_GotoZ(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t z_)
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


HAL_StatusTypeDef GLCD_L2_GotoXY(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_)
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


HAL_StatusTypeDef GLCD_L2_GotoXYZ(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
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
HAL_StatusTypeDef GLCD_L2_TrnsprntWriteByte(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, GLCD_L2_DispColor_TypeDef clr_)
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


HAL_StatusTypeDef GLCD_L2_TrnsprntWriteByteXY(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, GLCD_L2_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_)
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


HAL_StatusTypeDef GLCD_L2_TrnsprntWriteByteXYZ(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_, uint8_t data_, GLCD_L2_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t z_)
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


// Buffering Tools
HAL_StatusTypeDef GLCD_L2_SyncBuff_WriteToDisp(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    // Save Buffering status and turn it off
    bool tmp_bffrd_flg = pglcd_->buffered;
    pglcd_->buffered = false;

    if(GLCD_L2_GotoXY(pglcd_, hlf_, 0, 0) == HAL_BUSY)
    {
        // Restore buffering status
        pglcd_->buffered = tmp_bffrd_flg;
        
        return HAL_BUSY;
    }
    uint8_t tmp_data = 0x00;

    if(hlf_ == GLCD_L2_HALF_Both)
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_L2_GotoX(pglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Right, i, j);
                tmp_data = pglcd_->pbuffer_right->data[j +(i *(GLCD_L0_Y_MAX+1))];
                HAL_StatusTypeDef tmp_state = GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Right, tmp_data);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Right, tmp_data) != HAL_OK );
                    }
                }
                GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Left, i, j);
                tmp_data = pglcd_->pbuffer_left->data[j +(i *(GLCD_L0_Y_MAX+1))];
                tmp_state = GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Left, tmp_data);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_WriteByte(pglcd_, GLCD_L2_HALF_Left, tmp_data) != HAL_OK );
                    }
                }
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L2_GotoXY(pglcd_, hlf_, i, j);
                if(hlf_ == GLCD_L2_HALF_Right)
                {
                    tmp_data = pglcd_->pbuffer_right->data[j +(i *(GLCD_L0_Y_MAX+1))];
                }
                else
                {
                    tmp_data = pglcd_->pbuffer_left->data[j +(i *(GLCD_L0_Y_MAX+1))];
                }
                HAL_StatusTypeDef tmp_state = GLCD_L2_WriteByte(pglcd_, hlf_, tmp_data);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_WriteByte(pglcd_, hlf_, tmp_data) != HAL_OK );
                    }
                }
            }
        }
    }
    
    // Restore buffering status
    pglcd_->buffered = tmp_bffrd_flg;

    return HAL_OK;
}


HAL_StatusTypeDef GLCD_L2_SyncBuff_ReadFromDisp(GLCD_L1_TypeDef* pglcd_, GLCD_L2_HALF_TypeDef hlf_)
{
    // Save Buffering status and turn it off
    bool tmp_bffrd_flg = pglcd_->buffered;
    bool tmp_sync_flg  = pglcd_->synchronized;
    pglcd_->buffered     = true;
    pglcd_->synchronized = false;

    if(GLCD_L2_GotoXY(pglcd_, hlf_, 0, 0) == HAL_BUSY)
    {
        // Restore buffering status
        pglcd_->buffered     = tmp_bffrd_flg;
        pglcd_->synchronized = tmp_sync_flg;
        
        return HAL_BUSY;
    }

    if(hlf_ == GLCD_L2_HALF_Both)
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_L2_GotoX(pglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Right, i, j);
                GLCD_L2_ReadByte(pglcd_, GLCD_L2_HALF_Right);
                
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or HAL_BUSY!!!
                HAL_StatusTypeDef tmp_state = GLCD_L2_ReadByte(pglcd_, GLCD_L2_HALF_Right);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_ReadByte(pglcd_, GLCD_L2_HALF_Right) != HAL_OK );
                    }
                }
                */
                GLCD_L2_GotoXY(pglcd_, GLCD_L2_HALF_Left, i, j);
                GLCD_L2_ReadByte(pglcd_, GLCD_L2_HALF_Left);
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or HAL_BUSY!!!
                HAL_StatusTypeDef tmp_state = GLCD_L2_ReadByte(pglcd_, GLCD_L2_HALF_Left);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_ReadByte(pglcd_, GLCD_L2_HALF_Left) != HAL_OK );
                    }
                }
                */
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L2_GotoXY(pglcd_, hlf_, i, j);
                GLCD_L2_ReadByte(pglcd_, hlf_);
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or HAL_BUSY!!!
                HAL_StatusTypeDef tmp_state = GLCD_L2_ReadByte(pglcd_, hlf_);
                if( tmp_state != HAL_OK )
                {
                    if( tmp_state == HAL_BUSY )
                    {
                        while(GLCD_L2_ReadByte(pglcd_, hlf_); != HAL_OK );
                    }
                }
                */
            }
        }
    }
    
    // Restore buffering status
    pglcd_->buffered     = tmp_bffrd_flg;
    pglcd_->synchronized = tmp_sync_flg;

    return HAL_OK;
}



