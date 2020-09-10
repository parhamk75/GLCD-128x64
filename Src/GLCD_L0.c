#include "GLCD_L0.h"



HAL_StatusTypeDef GLCD_L0_Delay(uint16_t td_10xns_)
{

    for (uint16_t i = td_10xns_; i > 0; i--)
    {
        for(uint8_t j = GLCD_L0_MPU_10NS_CNST; j > 1; j--);
    }
    
    return HAL_OK;
}

HAL_StatusTypeDef GLCD_L0_Write(GLCD_L0_TypeDef* pglcd_, uint8_t DBs_, uint8_t is_instrctn_)
{
    // Re-Configurate DB Pins
    // EN -> Low (For Address Set-Up)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    // RW -> Low
    HAL_GPIO_WritePin(pglcd_->RW_Port, pglcd_->RW_Pin, GPIO_PIN_RESET);
    // RS -> Set
    HAL_GPIO_WritePin(pglcd_->RS_Port, pglcd_->RS_Pin, (GPIO_PinState)is_instrctn_);
    // CS1, CS2 -> Low
    HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_RESET);
    // DBs -> Set
    for(uint8_t i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(pglcd_->DB_Ports[i], pglcd_->DB_Pins[i], (GPIO_PinState)((DBs_>>i)%2));
    }
    // EN -> High (Prepare for a Falling Edge)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_SET);
    // EN -> Low (Make a Falling Edge)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    // EN, RW, CS1, CS2 -> High (Terminate the Job)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_->RW_Port, pglcd_->RW_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_SET);
    // RS -> Reset
    HAL_GPIO_TogglePin(pglcd_->RS_Port, pglcd_->RS_Pin);
    
    return HAL_OK;
}

uint8_t GLCD_L0_Read(GLCD_L0_TypeDef* pglcd_, uint8_t DBs_, uint8_t is_instrctn_)
{
    // Re-Configurate DB Pins
    // EN -> Low (For Address Set-Up) [I Think this is unneccessary and It can be just a high EN Signal from the brginning]
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    // RW -> High
    HAL_GPIO_WritePin(pglcd_->RW_Port, pglcd_->RW_Pin, GPIO_PIN_SET);
    // RS -> Set
    HAL_GPIO_WritePin(pglcd_->RS_Port, pglcd_->RS_Pin, (GPIO_PinState)is_instrctn_);
    // CS1, CS2 -> Low
    HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_RESET);
    // EN -> High (To make the GLCD latch it's data on DBs)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_SET);
    // DBs -> Read
    uint8_t tmp_val = 0x00;
    for(uint8_t i = 0; i < 8; i++)
    {
        tmp_val |= HAL_GPIO_ReadPin(pglcd_->DB_Ports[i], pglcd_->DB_Pins[i]) << i;
    }
    // EN, RW -> Low (Terminate the Job)
    HAL_GPIO_WritePin(pglcd_->EN_Port, pglcd_->EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pglcd_->RW_Port, pglcd_->RW_Pin, GPIO_PIN_RESET);
    // CS1, CS2 -> High (Terminate the Job)
    HAL_GPIO_WritePin(pglcd_->CS1_Port, pglcd_->CS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pglcd_->CS2_Port, pglcd_->CS2_Pin, GPIO_PIN_SET);
    // RS -> Reset
    HAL_GPIO_TogglePin(pglcd_->RS_Port, pglcd_->RS_Pin);
    
    return tmp_val;
}


HAL_StatusTypeDef   GLCD_L0_StartReset(GLCD_L0_TypeDef* pglcd_)
{
    HAL_GPIO_WritePin(pglcd_->RST_Port, pglcd_->RST_Pin, GPIO_PIN_RESET);
    return HAL_OK;
}

HAL_StatusTypeDef   GLCD_L0_StopReset(GLCD_L0_TypeDef* pglcd_)
{
    HAL_GPIO_WritePin(pglcd_->RST_Port, pglcd_->RST_Pin, GPIO_PIN_RESET);
    return HAL_OK;
}
