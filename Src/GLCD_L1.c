#include "GLCD_L1.h"

//TODO: Inline
#define     WHICH_HALF_BUFFER(cs_, pglcd1_)    (cs_ == GLCD_L0_CS_1 ? pglcd1_->pbuffer_right : pglcd1_->pbuffer_left)
#define     FIND_BUFF_DATA_INDEX(buff_)       ((buff_->y) + (buff_->x *(GLCD_L0_Y_MAX +1)))

// Utilities
uint8_t Extract_Bit(uint8_t mask_,uint8_t data_) //TODO: Inline
{
    uint8_t tmp_1 = data_ & mask_;
    if(tmp_1 == 0)
    {
        return 0;
    }
    return 1;
}

// GLCD 128x64 Instruction Set
GLCD_Status_TypeDef GLCD_L1_Ins_Disp_OnOff(GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  GLCD_L1_DispStatOnOff_TypeDef on_off_)
{
    return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_DispOnOff | ((~on_off_) & GLCD_L1_InsMask_DispOnOff)), GLCD_L0_FrameType_Instruction, cs_);
}


GLCD_Status_TypeDef GLCD_L1_Ins_Set_Address(GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  uint8_t y_addr_)
{
    if( pglcd_->buffered )
    {
        // Check if it is already on the Display
        if(WHICH_HALF_BUFFER(cs_, pglcd_)->y == y_addr_) {return GLCD_OK;}

        WHICH_HALF_BUFFER(cs_, pglcd_)->y = y_addr_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetAdddress | (y_addr_ & GLCD_L1_InsMask_SetAdddress)) , GLCD_L0_FrameType_Instruction, cs_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetAdddress | (y_addr_ & GLCD_L1_InsMask_SetAdddress)) , GLCD_L0_FrameType_Instruction, cs_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_L1_Ins_Set_Page(GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  uint8_t page_)
{
    if( pglcd_->buffered )
    {
        // Check if it is already on the Display
        if(WHICH_HALF_BUFFER(cs_, pglcd_)->x == page_) {return GLCD_OK;}

        WHICH_HALF_BUFFER(cs_, pglcd_)->x = page_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetPage | (page_ & GLCD_L1_InsMask_SetPage)), GLCD_L0_FrameType_Instruction, cs_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetPage | (page_ & GLCD_L1_InsMask_SetPage)), GLCD_L0_FrameType_Instruction, cs_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_L1_Ins_Set_DispStartLine(GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_,  uint8_t dsp_strt_ln_)
{
    if( pglcd_->buffered )
    {
        // Check if it is already on the Display
        if(WHICH_HALF_BUFFER(cs_, pglcd_)->z == dsp_strt_ln_) {return GLCD_OK;}

        WHICH_HALF_BUFFER(cs_, pglcd_)->z = dsp_strt_ln_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetDispStartLine | (dsp_strt_ln_ & GLCD_L1_InsMask_SetDispStartLine)), GLCD_L0_FrameType_Instruction, cs_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, (uint8_t)(GLCD_L1_InsBase_SetDispStartLine | (dsp_strt_ln_ & GLCD_L1_InsMask_SetDispStartLine)), GLCD_L0_FrameType_Instruction, cs_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_L1_Ins_Write_DispData(GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_, uint8_t data_)
{
    if( pglcd_->buffered )
    {
        GLCD_L0_Buffer_TypeDef* tmp_buff = WHICH_HALF_BUFFER(cs_, pglcd_);
        
        // Check if it is already on the Display
        if(tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] == data_) {return GLCD_OK;}
        
        tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] = data_;

        if( pglcd_->synchronized )
        {
            return GLCD_L0_Write(pglcd_->pglcd0, data_, GLCD_L0_FrameType_Data, cs_);
        }
    }
    else
    {
        return GLCD_L0_Write(pglcd_->pglcd0, data_, GLCD_L0_FrameType_Data, cs_);
    }

    return GLCD_ERROR;
}


uint8_t GLCD_L1_Ins_Read_Status(GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_)
{
    return GLCD_L0_Read(pglcd_->pglcd0, GLCD_L0_FrameType_Instruction, cs_);
}


uint8_t GLCD_L1_Ins_Read_DispData(GLCD_HandleTypeDef* pglcd_, GLCD_L0_ChipSelect_TypeDef cs_)
{
    if( pglcd_->buffered )
    {
        GLCD_L0_Buffer_TypeDef* tmp_buff = WHICH_HALF_BUFFER(cs_, pglcd_);
        
        if( pglcd_->synchronized )
        {
            return tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)];
        }
        
        tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] = GLCD_L0_Read(pglcd_->pglcd0, GLCD_L0_FrameType_Data, cs_);
        return tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)];
    }
    
    return GLCD_L0_Read(pglcd_->pglcd0, GLCD_L0_FrameType_Data, cs_);
}

// Status Checks
GLCD_L1_DispStatOnOff_TypeDef GLCD_L1_IsDispOnOff(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_)
{
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_L1_StatusMask_OnOff, GLCD_L1_Ins_Read_Status(pglcd_, GLCD_L1_DispHalf_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_L1_StatusMask_OnOff, GLCD_L1_Ins_Read_Status(pglcd_, GLCD_L1_DispHalf_Left));
        return (GLCD_L1_DispStatOnOff_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_L1_DispStatOnOff_TypeDef)Extract_Bit(GLCD_L1_StatusMask_OnOff, GLCD_L1_Ins_Read_Status(pglcd_, hlf_));
}


GLCD_L1_DispStatBusy_TypeDef GLCD_L1_IsDispBusy(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_)
{
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_L1_StatusMask_Busy, GLCD_L1_Ins_Read_Status(pglcd_, GLCD_L1_DispHalf_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_L1_StatusMask_Busy, GLCD_L1_Ins_Read_Status(pglcd_, GLCD_L1_DispHalf_Left));
        return (GLCD_L1_DispStatBusy_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_L1_DispStatBusy_TypeDef)Extract_Bit(GLCD_L1_StatusMask_Busy, GLCD_L1_Ins_Read_Status(pglcd_, hlf_));
}



// GLCD_L1_DispStatReset_TypeDef GLCD_L1_IsDispReset(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_)
// {
//     if(hlf_ == GLCD_L1_DispHalf_Both)
//     {
//         uint8_t tmp_r = Extract_Bit(GLCD_L1_StatusMask_Reset, GLCD_L1_Ins_Read_Status(pglcd_, GLCD_L1_DispHalf_Right));
//         uint8_t tmp_l = Extract_Bit(GLCD_L1_StatusMask_Reset, GLCD_L1_Ins_Read_Status(pglcd_, GLCD_L1_DispHalf_Left));
//         return (GLCD_L1_DispStatReset_TypeDef)(tmp_r & tmp_l);
//     }
//     return (GLCD_L1_DispStatReset_TypeDef)Extract_Bit(GLCD_L1_StatusMask_Reset, GLCD_L1_Ins_Read_Status(pglcd_, hlf_));
// }



// Initialization
GLCD_Status_TypeDef GLCD_L1_OnOff(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, GLCD_L1_DispStatOnOff_TypeDef on_off_)
{
    if( hlf_ != GLCD_L1_DispHalf_Both )
    {
        return GLCD_L1_Ins_Disp_OnOff(pglcd_, (GLCD_L0_ChipSelect_TypeDef)hlf_, on_off_);
    }
    else
    {
        if( GLCD_L1_Ins_Disp_OnOff(pglcd_, GLCD_L0_CS_1, on_off_) == GLCD_OK )
        {
            return GLCD_L1_Ins_Disp_OnOff(pglcd_, GLCD_L0_CS_2, on_off_);
        }
    }

    
    return GLCD_ERROR;
}



// Set Whole Display Color
GLCD_Status_TypeDef GLCD_L1_SetWholeDispColor(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, GLCD_L1_DispColor_TypeDef init_whole_dsp_clr_)
{
    if(GLCD_L1_GotoXY(pglcd_, hlf_, 0, 0) == GLCD_BUSY){return GLCD_BUSY;}

    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_L1_GotoX(pglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Right, i, j);
                GLCD_Status_TypeDef tmp_state = GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Right, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Right, (uint8_t)init_whole_dsp_clr_* 0xFF) != GLCD_OK );
                    }
                }
                GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Left, i, j);
                tmp_state = GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Left, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Left, (uint8_t)init_whole_dsp_clr_* 0xFF) != GLCD_OK );
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
                GLCD_L1_GotoXY(pglcd_, hlf_, i, j);
                GLCD_Status_TypeDef tmp_state = GLCD_L1_WriteByte(pglcd_, hlf_, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_WriteByte(pglcd_, hlf_, (uint8_t)init_whole_dsp_clr_* 0xFF) != GLCD_OK );
                    }
                }
            }
        }
    }
    
    
    return GLCD_OK;
}



// Write Data
GLCD_Status_TypeDef GLCD_L1_WriteByte(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_)
{
    if(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Write_DispData(pglcd_, GLCD_L1_DispHalf_Left, data_);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Right) == GLCD_L1_DispStatBusy_Busy);
        GLCD_L1_Ins_Write_DispData(pglcd_, GLCD_L1_DispHalf_Right, data_);

        return GLCD_OK;
    }
    GLCD_L1_Ins_Write_DispData(pglcd_, hlf_, data_);

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_L1_WriteByteXY(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_)
{
    if(GLCD_L1_GotoXY(pglcd_, hlf_, x_, y_) == GLCD_BUSY){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Write_DispData(pglcd_, GLCD_L1_DispHalf_Left, data_);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Right) == GLCD_L1_DispStatBusy_Busy);
        GLCD_L1_Ins_Write_DispData(pglcd_, GLCD_L1_DispHalf_Right, data_);

        return GLCD_OK;
    }
    GLCD_L1_Ins_Write_DispData(pglcd_, hlf_, data_);

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_L1_WriteByteXYZ(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    if(GLCD_L1_GotoXYZ(pglcd_, hlf_, x_, y_, z_) == GLCD_BUSY){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Write_DispData(pglcd_, GLCD_L1_DispHalf_Left, data_);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Right) == GLCD_L1_DispStatBusy_Busy);
        GLCD_L1_Ins_Write_DispData(pglcd_, GLCD_L1_DispHalf_Right, data_);

        return GLCD_OK;
    }
    GLCD_L1_Ins_Write_DispData(pglcd_, hlf_, data_);

    return GLCD_OK;
}



// Read Data
uint8_t GLCD_L1_ReadByte(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
    // if(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy){return GLCD_BUSY;}
    
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Left);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Left) == GLCD_L1_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Left);
        GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Right);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Right) == GLCD_L1_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Right);

        return tmp_l & tmp_r;
    }
    GLCD_L1_Ins_Read_DispData(pglcd_, hlf_);    // Dummy read request (Reason in datasheet!)
    while(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy);
    
    return GLCD_L1_Ins_Read_DispData(pglcd_, hlf_);
}


uint8_t GLCD_L1_ReadByteXY(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
    // if(GLCD_L1_GotoXY(pglcd_, hlf_, x_, y_) == GLCD_BUSY){return GLCD_BUSY;}
    GLCD_L1_GotoXY(pglcd_, hlf_, x_, y_);       // 2B Deleted
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Left);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Left) == GLCD_L1_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Left);
        GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Right);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Right) == GLCD_L1_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Right);

        return tmp_l & tmp_r;
    }
    GLCD_L1_Ins_Read_DispData(pglcd_, hlf_);
    while(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy);
    
    return GLCD_L1_Ins_Read_DispData(pglcd_, hlf_);
}


uint8_t GLCD_L1_ReadByteXYZ(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
    // if(GLCD_L1_GotoXYZ(pglcd_, hlf_, x_, y_, z_) == GLCD_BUSY){return GLCD_BUSY;}
    GLCD_L1_GotoXYZ(pglcd_, hlf_, x_, y_, z_);       // 2B Deleted
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Left);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Left) == GLCD_L1_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Left);
        GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Right);
        while(GLCD_L1_IsDispBusy(pglcd_, GLCD_L1_DispHalf_Right) == GLCD_L1_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_L1_Ins_Read_DispData(pglcd_, GLCD_L1_DispHalf_Right);

        return tmp_l & tmp_r;
    }
    
    GLCD_L1_Ins_Read_DispData(pglcd_, hlf_);
    while(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy);
    
    return GLCD_L1_Ins_Read_DispData(pglcd_, hlf_);
}


// Set Address Functions (Goto)

GLCD_Status_TypeDef GLCD_L1_GotoX(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_)
{
    if(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_Page(pglcd_, GLCD_L1_DispHalf_Right, x_);
        GLCD_L1_Ins_Set_Page(pglcd_, GLCD_L1_DispHalf_Left, x_);
    }
    else
    {
        GLCD_L1_Ins_Set_Page(pglcd_, hlf_, x_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_L1_GotoY(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t y_)
{
    if(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_Address(pglcd_, GLCD_L1_DispHalf_Right, y_);
        GLCD_L1_Ins_Set_Address(pglcd_, GLCD_L1_DispHalf_Left, y_);
    }
    else
    {
        GLCD_L1_Ins_Set_Address(pglcd_, hlf_, y_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_L1_GotoZ(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t z_)
{
    if(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_DispStartLine(pglcd_, GLCD_L1_DispHalf_Right, z_);
        GLCD_L1_Ins_Set_DispStartLine(pglcd_, GLCD_L1_DispHalf_Left, z_);
    }
    else
    {
        GLCD_L1_Ins_Set_DispStartLine(pglcd_, hlf_, z_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_L1_GotoXY(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_)
{
    if(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_Page(pglcd_, GLCD_L1_DispHalf_Right, x_);
        GLCD_L1_Ins_Set_Page(pglcd_, GLCD_L1_DispHalf_Left, x_);
    }
    else
    {
        GLCD_L1_Ins_Set_Page(pglcd_, hlf_, x_);
    }
    while(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy);
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_Address(pglcd_, GLCD_L1_DispHalf_Right, y_);
        GLCD_L1_Ins_Set_Address(pglcd_, GLCD_L1_DispHalf_Left, y_);
    }
    else
    {
        GLCD_L1_Ins_Set_Address(pglcd_, hlf_, y_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_L1_GotoXYZ(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    if(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_Page(pglcd_, GLCD_L1_DispHalf_Right, x_);
        GLCD_L1_Ins_Set_Page(pglcd_, GLCD_L1_DispHalf_Left, x_);
    }
    else
    {
        GLCD_L1_Ins_Set_Page(pglcd_, hlf_, x_);
    }
    while(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy);
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_Address(pglcd_, GLCD_L1_DispHalf_Right, y_);
        GLCD_L1_Ins_Set_Address(pglcd_, GLCD_L1_DispHalf_Left, y_);
    }
    else
    {
        GLCD_L1_Ins_Set_Address(pglcd_, hlf_, y_);
    }
    while(GLCD_L1_IsDispBusy(pglcd_, hlf_) == GLCD_L1_DispStatBusy_Busy);
    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        GLCD_L1_Ins_Set_DispStartLine(pglcd_, GLCD_L1_DispHalf_Right, z_);
        GLCD_L1_Ins_Set_DispStartLine(pglcd_, GLCD_L1_DispHalf_Left, z_);
    }
    else
    {
        GLCD_L1_Ins_Set_DispStartLine(pglcd_, hlf_, z_);
    }

    return GLCD_OK;
}


// Complex Write Functions
GLCD_Status_TypeDef GLCD_L1_TrnsprntWriteByte(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_)
{
    uint8_t tmp_prev_data = GLCD_L1_ReadByte(pglcd_, hlf_);

    if(clr_ == GLCD_L1_DispColor_Black)
    {
        return GLCD_L1_WriteByte(pglcd_, hlf_, data_ | tmp_prev_data);
    }
    else
    {
        return GLCD_L1_WriteByte(pglcd_, hlf_, data_ & tmp_prev_data);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_L1_TrnsprntWriteByteXY(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_)
{
    uint8_t tmp_prev_data = GLCD_L1_ReadByteXY(pglcd_, hlf_, x_, y_);

    if(clr_ == GLCD_L1_DispColor_Black)
    {
        return GLCD_L1_WriteByteXY(pglcd_, hlf_, data_ | tmp_prev_data, x_, y_);
    }
    else
    {
        return GLCD_L1_WriteByteXY(pglcd_, hlf_, data_ & tmp_prev_data, x_, y_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_L1_TrnsprntWriteByteXYZ(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_L1_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    uint8_t tmp_prev_data = GLCD_L1_ReadByteXYZ(pglcd_, hlf_, x_, y_, z_);

    if(clr_ == GLCD_L1_DispColor_Black)
    {
        return GLCD_L1_WriteByteXYZ(pglcd_, hlf_, data_ | tmp_prev_data, x_, y_, z_);
    }
    else
    {
        return GLCD_L1_WriteByteXYZ(pglcd_, hlf_, data_ & tmp_prev_data, x_, y_, z_);
    }

    return GLCD_ERROR;
}


// Buffering Tools
GLCD_Status_TypeDef GLCD_L1_SyncBuff_WriteToDisp(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_)
{
    // Save Buffering status and turn it off
    bool tmp_bffrd_flg = pglcd_->buffered;
    pglcd_->buffered = false;

    if(GLCD_L1_GotoXY(pglcd_, hlf_, 0, 0) == GLCD_BUSY)
    {
        // Restore buffering status
        pglcd_->buffered = tmp_bffrd_flg;
        
        return GLCD_BUSY;
    }
    uint8_t tmp_data = 0x00;

    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_L1_GotoX(pglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Right, i, j);
                tmp_data = pglcd_->pbuffer_right->data[j +(i *(GLCD_L0_Y_MAX+1))];
                GLCD_Status_TypeDef tmp_state = GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Right, tmp_data);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Right, tmp_data) != GLCD_OK );
                    }
                }
                GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Left, i, j);
                tmp_data = pglcd_->pbuffer_left->data[j +(i *(GLCD_L0_Y_MAX+1))];
                tmp_state = GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Left, tmp_data);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_WriteByte(pglcd_, GLCD_L1_DispHalf_Left, tmp_data) != GLCD_OK );
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
                GLCD_L1_GotoXY(pglcd_, hlf_, i, j);
                if(hlf_ == GLCD_L1_DispHalf_Right)
                {
                    tmp_data = pglcd_->pbuffer_right->data[j +(i *(GLCD_L0_Y_MAX+1))];
                }
                else
                {
                    tmp_data = pglcd_->pbuffer_left->data[j +(i *(GLCD_L0_Y_MAX+1))];
                }
                GLCD_Status_TypeDef tmp_state = GLCD_L1_WriteByte(pglcd_, hlf_, tmp_data);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_WriteByte(pglcd_, hlf_, tmp_data) != GLCD_OK );
                    }
                }
            }
        }
    }
    
    // Restore buffering status
    pglcd_->buffered = tmp_bffrd_flg;

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_L1_SyncBuff_ReadFromDisp(GLCD_HandleTypeDef* pglcd_, GLCD_L1_DispHalf_TypeDef hlf_)
{
    // Save Buffering status and turn it off
    bool tmp_bffrd_flg = pglcd_->buffered;
    bool tmp_sync_flg  = pglcd_->synchronized;
    pglcd_->buffered     = true;
    pglcd_->synchronized = false;

    if(GLCD_L1_GotoXY(pglcd_, hlf_, 0, 0) == GLCD_BUSY)
    {
        // Restore buffering status
        pglcd_->buffered     = tmp_bffrd_flg;
        pglcd_->synchronized = tmp_sync_flg;
        
        return GLCD_BUSY;
    }

    if(hlf_ == GLCD_L1_DispHalf_Both)
    {
        for (uint8_t i = 0; i <= GLCD_L0_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_L1_GotoX(pglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_L0_Y_MAX; j++)
            {
                GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Right, i, j);
                GLCD_L1_ReadByte(pglcd_, GLCD_L1_DispHalf_Right);
                
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
                GLCD_Status_TypeDef tmp_state = GLCD_L1_ReadByte(pglcd_, GLCD_L1_DispHalf_Right);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_ReadByte(pglcd_, GLCD_L1_DispHalf_Right) != GLCD_OK );
                    }
                }
                */
                GLCD_L1_GotoXY(pglcd_, GLCD_L1_DispHalf_Left, i, j);
                GLCD_L1_ReadByte(pglcd_, GLCD_L1_DispHalf_Left);
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
                GLCD_Status_TypeDef tmp_state = GLCD_L1_ReadByte(pglcd_, GLCD_L1_DispHalf_Left);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_ReadByte(pglcd_, GLCD_L1_DispHalf_Left) != GLCD_OK );
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
                GLCD_L1_GotoXY(pglcd_, hlf_, i, j);
                GLCD_L1_ReadByte(pglcd_, hlf_);
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
                GLCD_Status_TypeDef tmp_state = GLCD_L1_ReadByte(pglcd_, hlf_);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_L1_ReadByte(pglcd_, hlf_); != GLCD_OK );
                    }
                }
                */
            }
        }
    }
    
    // Restore buffering status
    pglcd_->buffered     = tmp_bffrd_flg;
    pglcd_->synchronized = tmp_sync_flg;

    return GLCD_OK;
}
