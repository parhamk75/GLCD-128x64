#include "GLCD_L1.h"

#define     WHICH_HALF_BUFFER(hlf_, pglcd1_)    (hlf_ == GLCD_L0_HALF_Right ? pglcd1_->pbuffer_right : pglcd1_->pbuffer_left)
#define     FIND_BUFF_DATA_INDEX(buff_)       ((buff_->y) + (buff_->x *(GLCD_L0_Y_MAX +1)))

// GLCD 128x64 Instruction Set
HAL_StatusTypeDef GLCD_L1_Disp_OnOff(GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  GLCD_L1_Disp_OnOff_TypeDef on_off_)
{
    return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_DispOnOff | (on_off_ & GLCD_L1_InsMask_DispOnOff)), GLCD_L0_FrameType_Instruction, hlf_);
}


HAL_StatusTypeDef GLCD_L1_Set_Address(GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t y_addr_)
{
    if( pglcd_->buffered )
    {
        WHICH_HALF_BUFFER(hlf_, pglcd_)->y = y_addr_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetAdddress | (y_addr_ & GLCD_L1_InsMask_SetAdddress)) , GLCD_L0_FrameType_Instruction, hlf_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetAdddress | (y_addr_ & GLCD_L1_InsMask_SetAdddress)) , GLCD_L0_FrameType_Instruction, hlf_);
    }
}


HAL_StatusTypeDef GLCD_L1_Set_Page(GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t page_)
{
    if( pglcd_->buffered )
    {
        WHICH_HALF_BUFFER(hlf_, pglcd_)->x = page_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetPage | (page_ & GLCD_L1_InsMask_SetPage)), GLCD_L0_FrameType_Instruction, hlf_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetPage | (page_ & GLCD_L1_InsMask_SetPage)), GLCD_L0_FrameType_Instruction, hlf_);
    }
}


HAL_StatusTypeDef GLCD_L1_Set_DispStartLine(GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_,  uint8_t dsp_strt_ln_)
{
    if( pglcd_->buffered )
    {
        WHICH_HALF_BUFFER(hlf_, pglcd_)->z = dsp_strt_ln_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetDispStartLine | (dsp_strt_ln_ & GLCD_L1_InsMask_SetDispStartLine)), GLCD_L0_FrameType_Instruction, hlf_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetDispStartLine | (dsp_strt_ln_ & GLCD_L1_InsMask_SetDispStartLine)), GLCD_L0_FrameType_Instruction, hlf_);
    }
}


HAL_StatusTypeDef GLCD_L1_Write_DispData(GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_, uint8_t data_)
{
    if( pglcd_->buffered )
    {
        GLCD_L1_Buffer_TypeDef* tmp_buff = WHICH_HALF_BUFFER(hlf_, pglcd_);
        tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] = data_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, data_, GLCD_L0_FrameType_Data, hlf_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, data_, GLCD_L0_FrameType_Data, hlf_);
    }
}


uint8_t GLCD_L1_Read_Status(GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_)
{
    return GLCD_L0_Read(pglcd_->pglcd0, GLCD_L0_FrameType_Instruction, hlf_);
}


uint8_t GLCD_L1_Read_DispData(GLCD_L1_TypeDef* pglcd_, GLCD_L0_HALF_TypeDef hlf_)
{
    if( pglcd_->buffered )
    {
        GLCD_L1_Buffer_TypeDef* tmp_buff = WHICH_HALF_BUFFER(hlf_, pglcd_);

        if( pglcd_->synchronized )
        {
            tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] = GLCD_L0_Read(pglcd_->pglcd0, GLCD_L0_FrameType_Data, hlf_);
        }
        
        return tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)];
    }
    else
    {
        return GLCD_L0_Read(pglcd_->pglcd0, GLCD_L0_FrameType_Data, hlf_);
    }
}


// GLCD Tools (Wrappers for L0 Functions)
HAL_StatusTypeDef GLCD_L1_Delay(uint16_t td_10xns_)
{
    return GLCD_L0_Delay(td_10xns_);
}


HAL_StatusTypeDef GLCD_L1_StartReset(GLCD_L1_TypeDef* pglcd_)
{
    return GLCD_L0_StartReset(pglcd_->pglcd0);
}


HAL_StatusTypeDef GLCD_L1_StopReset(GLCD_L1_TypeDef* pglcd_)
{
    return GLCD_L0_StopReset(pglcd_->pglcd0);
}


GPIO_PinState GLCD_L1_CheckReset(GLCD_L1_TypeDef* pglcd_)
{
    return GLCD_L0_CheckReset(pglcd_->pglcd0);
}

