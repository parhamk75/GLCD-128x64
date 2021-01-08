#include "GLCD_LL_STM32.h"

// Utilities
typedef enum{
    IO_DIR_INPUT    = GPIO_MODE_INPUT,
    IO_DIR_OUTPUT   = GPIO_MODE_OUTPUT_PP
}IO_Dir_TypeDef;


void Change_IO_Dir(GPIO_TypeDef* port_, uint16_t pin_, IO_Dir_TypeDef io_dir_)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(io_dir_ == IO_DIR_INPUT)
    {
        GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    }
    else
    {
        GPIO_InitStruct.Pull  = GPIO_NOPULL;        
    }
    
    GPIO_InitStruct.Mode  = io_dir_;
    GPIO_InitStruct.Pin   = pin_;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(port_, &GPIO_InitStruct);

}


// GLCD_LL Funcs
GLCD_Status_TypeDef GLCD_LL_Delay(uint16_t td_10xns_)
{

    for (uint16_t i = td_10xns_; i > 0; i--)
    {
        for(uint8_t j = GLCD_LL_MPU_10NS_CNST; j > 1; j--)
        {
            __asm__ __volatile__("");
        }
    }
    
    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_LL_Write(GLCD_LL_TypeDef* pglcd_ll_, uint8_t DBs_, GLCD_LL_FrameType_TypeDef frm_typ_, GLCD_LL_ChipSelect_TypeDef cs_)
{
    // Re-Configurate DB Pins
    if (pglcd_ll_->Mode == GLCD_LL_Mode_Read)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            Change_IO_Dir(pglcd_ll_->DB_Ports[i], pglcd_ll_->DB_Pins[i], IO_DIR_OUTPUT);
        }
        pglcd_ll_->Mode = GLCD_LL_Mode_Write;
    }    
    // EN -> Low (For Address Set-Up)
    HAL_GPIO_WritePin(pglcd_ll_->EN_Port, pglcd_ll_->EN_Pin, GPIO_PIN_RESET);
    // RW -> Low
    HAL_GPIO_WritePin(pglcd_ll_->RW_Port, pglcd_ll_->RW_Pin, GPIO_PIN_RESET);    
    // CS1/CS2 -> Low
    if( cs_ == GLCD_LL_CS_1 )
    {
        HAL_GPIO_WritePin(pglcd_ll_->CS1_Port, pglcd_ll_->CS1_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(pglcd_ll_->CS2_Port, pglcd_ll_->CS2_Pin, GPIO_PIN_RESET);
    }
    // RS -> Set
    HAL_GPIO_WritePin(pglcd_ll_->RS_Port, pglcd_ll_->RS_Pin, (GPIO_PinState)frm_typ_);
    // DBs -> Set
    for(uint8_t i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(pglcd_ll_->DB_Ports[i], pglcd_ll_->DB_Pins[i], (GPIO_PinState)((DBs_>>i)%2));
    }
    // EN -> High (Prepare for a Falling Edge)
    GLCD_LL_Delay(GLCD_LL_T_WL); // including a GLCD_LL_Delay(GLCD_LL_T_ASU); in itself!
    HAL_GPIO_WritePin(pglcd_ll_->EN_Port, pglcd_ll_->EN_Pin, GPIO_PIN_SET);
    // EN -> Low (Make a Falling Edge)
    GLCD_LL_Delay(GLCD_LL_T_WH); // including a GLCD_LL_Delay(GLCD_LL_T_DSU); in itself!
    HAL_GPIO_WritePin(pglcd_ll_->EN_Port, pglcd_ll_->EN_Pin, GPIO_PIN_RESET);
    // EN, RW, CS1, CS2 -> High (Terminate the Job)
    GLCD_LL_Delay(GLCD_LL_T_DHW);
    HAL_GPIO_WritePin(pglcd_ll_->RW_Port, pglcd_ll_->RW_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_ll_->CS1_Port, pglcd_ll_->CS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_ll_->CS2_Port, pglcd_ll_->CS2_Pin, GPIO_PIN_SET);
    GLCD_LL_Delay(GLCD_LL_T_WL/2); // Just for ensure (for EN signal Min Cycle Time)
    HAL_GPIO_WritePin(pglcd_ll_->EN_Port, pglcd_ll_->EN_Pin, GPIO_PIN_SET);
    // RS -> Low (to prevent confusion with data write)
    HAL_GPIO_WritePin(pglcd_ll_->RS_Port, pglcd_ll_->RS_Pin, GPIO_PIN_RESET);
    
    return GLCD_OK;
}


uint8_t GLCD_LL_Read(GLCD_LL_TypeDef* pglcd_ll_, GLCD_LL_FrameType_TypeDef frm_typ_, GLCD_LL_ChipSelect_TypeDef cs_)
{
    // Re-Configurate DB Pins
    if (pglcd_ll_->Mode == GLCD_LL_Mode_Write)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            Change_IO_Dir(pglcd_ll_->DB_Ports[i], pglcd_ll_->DB_Pins[i], IO_DIR_INPUT);
        }
        pglcd_ll_->Mode = GLCD_LL_Mode_Read;
    }
    // EN -> Low (For Address Set-Up) [I Think this is unneccessary and It can be just a high EN Signal from the brginning]
    HAL_GPIO_WritePin(pglcd_ll_->EN_Port, pglcd_ll_->EN_Pin, GPIO_PIN_RESET);
    // RW -> High
    HAL_GPIO_WritePin(pglcd_ll_->RW_Port, pglcd_ll_->RW_Pin, GPIO_PIN_SET);
    // CS1/CS2 -> Low
    if( cs_ == GLCD_LL_CS_1 )
    {
        HAL_GPIO_WritePin(pglcd_ll_->CS1_Port, pglcd_ll_->CS1_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(pglcd_ll_->CS2_Port, pglcd_ll_->CS2_Pin, GPIO_PIN_RESET);
    }
    // RS -> Set
    HAL_GPIO_WritePin(pglcd_ll_->RS_Port, pglcd_ll_->RS_Pin, (GPIO_PinState)frm_typ_);
    // EN -> High (To make the GLCD latch it's data on DBs)
    GLCD_LL_Delay(GLCD_LL_T_WL); // including a GLCD_LL_Delay(GLCD_LL_T_ASU); in itself!
    HAL_GPIO_WritePin(pglcd_ll_->EN_Port, pglcd_ll_->EN_Pin, GPIO_PIN_SET);
    // DBs -> Read
    GLCD_LL_Delay(GLCD_LL_T_D);
    uint8_t tmp_val = 0x00;
    for(uint8_t i = 0; i < 8; i++)
    {
        tmp_val |= HAL_GPIO_ReadPin(pglcd_ll_->DB_Ports[i], pglcd_ll_->DB_Pins[i]) << i;
    }
    // EN, RW -> Low (Terminate the Job)
    GLCD_LL_Delay(GLCD_LL_T_WH);
    HAL_GPIO_WritePin(pglcd_ll_->EN_Port, pglcd_ll_->EN_Pin, GPIO_PIN_RESET);
    // CS1, CS2 -> High (Terminate the Job) (RW -> Remains High to prevent accidental data write)
    GLCD_LL_Delay(GLCD_LL_T_AH);
    HAL_GPIO_WritePin(pglcd_ll_->CS1_Port, pglcd_ll_->CS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_ll_->CS2_Port, pglcd_ll_->CS2_Pin, GPIO_PIN_SET);
    // RS -> Low (to prevent accidental data write)
    HAL_GPIO_WritePin(pglcd_ll_->RS_Port, pglcd_ll_->RS_Pin, GPIO_PIN_RESET);
    
    return tmp_val;
}


GLCD_Status_TypeDef GLCD_LL_StartReset(GLCD_LL_TypeDef* pglcd_ll_)
{
    HAL_GPIO_WritePin(pglcd_ll_->RST_Port, pglcd_ll_->RST_Pin, GPIO_PIN_RESET);
    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_LL_StopReset(GLCD_LL_TypeDef* pglcd_ll_)
{
    HAL_GPIO_WritePin(pglcd_ll_->RST_Port, pglcd_ll_->RST_Pin, GPIO_PIN_SET);
    return GLCD_OK;
}


GPIO_PinState GLCD_LL_CheckReset(GLCD_LL_TypeDef* pglcd_ll_)
{
    return HAL_GPIO_ReadPin(pglcd_ll_->DB_Ports[4], pglcd_ll_->DB_Pins[4]);
}


GLCD_Status_TypeDef GLCD_LL_Init(GLCD_LL_TypeDef* pglcd_ll_)
{
    // GPIO Initializations
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // ~ RS
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_ll_->RS_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_ll_->RS_Port, &GPIO_InitStruct);

    // ~ R/W
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_ll_->RW_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_ll_->RW_Port, &GPIO_InitStruct);

    // ~ CS1, CS2
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_ll_->CS1_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_ll_->CS1_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin   = pglcd_ll_->CS2_Pin;
    HAL_GPIO_Init(pglcd_ll_->CS2_Port, &GPIO_InitStruct);

    // ~ EN
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_ll_->EN_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_ll_->EN_Port, &GPIO_InitStruct);

    // ~ RST
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = pglcd_ll_->RST_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(pglcd_ll_->RST_Port, &GPIO_InitStruct);

    // ~ DBs
    if(pglcd_ll_->Mode == GLCD_LL_Mode_Write)
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
        GPIO_InitStruct.Pin   = pglcd_ll_->DB_Pins[i];
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(pglcd_ll_->DB_Ports[i], &GPIO_InitStruct);
    }

    // Perform a Reset procedure
    GLCD_LL_StartReset(pglcd_ll_);
    GLCD_LL_Delay(GLCD_LL_T_RS* 10);
    GLCD_LL_StopReset(pglcd_ll_);
    while(GLCD_LL_CheckReset(pglcd_ll_));
    // while(GLCD_L1_IsDispReset(pglcd_ll_, GLCD_L1_DispHalf_Both) == GLCD_L1_DispStatReset_InReset);
    
    return GLCD_OK;
}


