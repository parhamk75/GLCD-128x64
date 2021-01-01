#include "GLCD_L0.h"

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


// GLCD_L0 Funcs
GLCD_Status_TypeDef GLCD_L0_Delay(uint16_t td_10xns_)
{

    for (uint16_t i = td_10xns_; i > 0; i--)
    {
        for(uint8_t j = GLCD_L0_MPU_10NS_CNST; j > 1; j--)
        {
            __asm__ __volatile__("");
        }
    }
    
    return GLCD_OK;
}

GLCD_Status_TypeDef GLCD_L0_Write(GLCD_L0_TypeDef* pglcd_, uint8_t DBs_, GLCD_L0_FrameType_TypeDef frm_typ_, GLCD_L0_ChipSelect_TypeDef cs_)
{
    // Re-Configurate DB Pins
    if (pglcd_->Mode == GLCD_L0_Mode_Read)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            Change_IO_Dir(pglcd_->DB_Ports[i], pglcd_->DB_Pins[i], IO_DIR_OUTPUT);
        }
        pglcd_->Mode = GLCD_L0_Mode_Write;
    }    
    // EN -> Low (For Address Set-Up)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    // RW -> Low
    HAL_GPIO_WritePin(pglcd_->RW_Port, pglcd_->RW_Pin, GPIO_PIN_RESET);    
    // CS1/CS2 -> Low
    if( cs_ == GLCD_L0_CS_1 )
    {
        HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_RESET);
    }
    // RS -> Set
    HAL_GPIO_WritePin(pglcd_->RS_Port, pglcd_->RS_Pin, (GPIO_PinState)frm_typ_);
    // DBs -> Set
    for(uint8_t i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(pglcd_->DB_Ports[i], pglcd_->DB_Pins[i], (GPIO_PinState)((DBs_>>i)%2));
    }
    // EN -> High (Prepare for a Falling Edge)
    GLCD_L0_Delay(GLCD_L0_T_WL); // including a GLCD_L0_Delay(GLCD_L0_T_ASU); in itself!
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_SET);
    // EN -> Low (Make a Falling Edge)
    GLCD_L0_Delay(GLCD_L0_T_WH); // including a GLCD_L0_Delay(GLCD_L0_T_DSU); in itself!
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    // EN, RW, CS1, CS2 -> High (Terminate the Job)
    GLCD_L0_Delay(GLCD_L0_T_DHW);
    HAL_GPIO_WritePin(pglcd_->RW_Port, pglcd_->RW_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_SET);
    GLCD_L0_Delay(GLCD_L0_T_WL/2); // Just for ensure (for EN signal Min Cycle Time)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_SET);
    // RS -> Low (to prevent confusion with data write)
    HAL_GPIO_WritePin(pglcd_->RS_Port, pglcd_->RS_Pin, GPIO_PIN_RESET);
    
    return GLCD_OK;
}


uint8_t GLCD_L0_Read(GLCD_L0_TypeDef* pglcd_, GLCD_L0_FrameType_TypeDef frm_typ_, GLCD_L0_ChipSelect_TypeDef cs_)
{
    // Re-Configurate DB Pins
    if (pglcd_->Mode == GLCD_L0_Mode_Write)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            Change_IO_Dir(pglcd_->DB_Ports[i], pglcd_->DB_Pins[i], IO_DIR_INPUT);
        }
        pglcd_->Mode = GLCD_L0_Mode_Read;
    }
    // EN -> Low (For Address Set-Up) [I Think this is unneccessary and It can be just a high EN Signal from the brginning]
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    // RW -> High
    HAL_GPIO_WritePin(pglcd_->RW_Port, pglcd_->RW_Pin, GPIO_PIN_SET);
    // CS1/CS2 -> Low
    if( cs_ == GLCD_L0_CS_1 )
    {
        HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_RESET);
    }
    // RS -> Set
    HAL_GPIO_WritePin(pglcd_->RS_Port, pglcd_->RS_Pin, (GPIO_PinState)frm_typ_);
    // EN -> High (To make the GLCD latch it's data on DBs)
    GLCD_L0_Delay(GLCD_L0_T_WL); // including a GLCD_L0_Delay(GLCD_L0_T_ASU); in itself!
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_SET);
    // DBs -> Read
    GLCD_L0_Delay(GLCD_L0_T_D);
    uint8_t tmp_val = 0x00;
    for(uint8_t i = 0; i < 8; i++)
    {
        tmp_val |= HAL_GPIO_ReadPin(pglcd_->DB_Ports[i], pglcd_->DB_Pins[i]) << i;
    }
    // EN, RW -> Low (Terminate the Job)
    GLCD_L0_Delay(GLCD_L0_T_WH);
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    // CS1, CS2 -> High (Terminate the Job) (RW -> Remains High to prevent accidental data write)
    GLCD_L0_Delay(GLCD_L0_T_AH);
    HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_SET);
    // RS -> Low (to prevent accidental data write)
    HAL_GPIO_WritePin(pglcd_->RS_Port, pglcd_->RS_Pin, GPIO_PIN_RESET);
    
    return tmp_val;
}


GLCD_Status_TypeDef GLCD_L0_StartReset(GLCD_L0_TypeDef* pglcd_)
{
    HAL_GPIO_WritePin(pglcd_->RST_Port, pglcd_->RST_Pin, GPIO_PIN_RESET);
    return GLCD_OK;
}

GLCD_Status_TypeDef GLCD_L0_StopReset(GLCD_L0_TypeDef* pglcd_)
{
    HAL_GPIO_WritePin(pglcd_->RST_Port, pglcd_->RST_Pin, GPIO_PIN_SET);
    return GLCD_OK;
}

GPIO_PinState GLCD_L0_CheckReset(GLCD_L0_TypeDef* pglcd_)
{
    return HAL_GPIO_ReadPin(pglcd_->DB_Ports[4], pglcd_->DB_Pins[4]);
}

GLCD_Status_TypeDef GLCD_L1_Init(GLCD_HandleTypeDef* pglcd_)
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
    GLCD_L0_StartReset(pglcd_->pglcd0);
    GLCD_L0_Delay(GLCD_L0_T_RS* 10);
    GLCD_L0_StopReset(pglcd_->pglcd0);
    while(GLCD_L0_CheckReset(pglcd_->pglcd0));
    // while(GLCD_L1_IsDispReset(pglcd_, GLCD_L1_DispHalf_Both) == GLCD_L1_DispStatReset_InReset);
    
    return GLCD_OK;
}