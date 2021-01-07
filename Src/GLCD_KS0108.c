#include "GLCD_KS0108.h"

//TODO: Inline
#define     WHICH_HALF_BUFFER(cs_, pglcd1_)    (cs_ == GLCD_LL_CS_1 ? pglcd1_->pbuffer_right : pglcd1_->pbuffer_left)
#define     FIND_BUFF_DATA_INDEX(buff_)       ((buff_->y) + (buff_->x *(GLCD_LL_Y_MAX +1)))

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
GLCD_Status_TypeDef GLCD_Driver_Ins_Disp_OnOff(GLCD_Handle_TypeDef* phglcd_, GLCD_LL_ChipSelect_TypeDef cs_,  GLCD_Driver_DispStatOnOff_TypeDef on_off_)
{
    return GLCD_LL_Write(phglcd_->pglcd_ll, (uint8_t)(GLCD_Driver_InsBase_DispOnOff | ((~on_off_) & GLCD_Driver_InsMask_DispOnOff)), GLCD_LL_FrameType_Instruction, cs_);
}


GLCD_Status_TypeDef GLCD_Driver_Ins_Set_Address(GLCD_Handle_TypeDef* phglcd_, GLCD_LL_ChipSelect_TypeDef cs_,  uint8_t y_addr_)
{
    if( phglcd_->buffered )
    {
        // Check if it is already on the Display
        if(WHICH_HALF_BUFFER(cs_, phglcd_)->y == y_addr_) {return GLCD_OK;}

        WHICH_HALF_BUFFER(cs_, phglcd_)->y = y_addr_;

        if( phglcd_->synchronized )
        {
            return GLCD_LL_Write(phglcd_->pglcd_ll, (uint8_t)(GLCD_Driver_InsBase_SetAdddress | (y_addr_ & GLCD_Driver_InsMask_SetAdddress)) , GLCD_LL_FrameType_Instruction, cs_);
        }
    }
    else
    {
        return GLCD_LL_Write(phglcd_->pglcd_ll, (uint8_t)(GLCD_Driver_InsBase_SetAdddress | (y_addr_ & GLCD_Driver_InsMask_SetAdddress)) , GLCD_LL_FrameType_Instruction, cs_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_Driver_Ins_Set_Page(GLCD_Handle_TypeDef* phglcd_, GLCD_LL_ChipSelect_TypeDef cs_,  uint8_t page_)
{
    if( phglcd_->buffered )
    {
        // Check if it is already on the Display
        if(WHICH_HALF_BUFFER(cs_, phglcd_)->x == page_) {return GLCD_OK;}

        WHICH_HALF_BUFFER(cs_, phglcd_)->x = page_;

        if( phglcd_->synchronized )
        {
            return GLCD_LL_Write(phglcd_->pglcd_ll, (uint8_t)(GLCD_Driver_InsBase_SetPage | (page_ & GLCD_Driver_InsMask_SetPage)), GLCD_LL_FrameType_Instruction, cs_);
        }
    }
    else
    {
        return GLCD_LL_Write(phglcd_->pglcd_ll, (uint8_t)(GLCD_Driver_InsBase_SetPage | (page_ & GLCD_Driver_InsMask_SetPage)), GLCD_LL_FrameType_Instruction, cs_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_Driver_Ins_Set_DispStartLine(GLCD_Handle_TypeDef* phglcd_, GLCD_LL_ChipSelect_TypeDef cs_,  uint8_t dsp_strt_ln_)
{
    if( phglcd_->buffered )
    {
        // Check if it is already on the Display
        if(WHICH_HALF_BUFFER(cs_, phglcd_)->z == dsp_strt_ln_) {return GLCD_OK;}

        WHICH_HALF_BUFFER(cs_, phglcd_)->z = dsp_strt_ln_;

        if( phglcd_->synchronized )
        {
            return GLCD_LL_Write(phglcd_->pglcd_ll, (uint8_t)(GLCD_Driver_InsBase_SetDispStartLine | (dsp_strt_ln_ & GLCD_Driver_InsMask_SetDispStartLine)), GLCD_LL_FrameType_Instruction, cs_);
        }
    }
    else
    {
        return GLCD_LL_Write(phglcd_->pglcd_ll, (uint8_t)(GLCD_Driver_InsBase_SetDispStartLine | (dsp_strt_ln_ & GLCD_Driver_InsMask_SetDispStartLine)), GLCD_LL_FrameType_Instruction, cs_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_Driver_Ins_Write_DispData(GLCD_Handle_TypeDef* phglcd_, GLCD_LL_ChipSelect_TypeDef cs_, uint8_t data_)
{
    if( phglcd_->buffered )
    {
        GLCD_LL_Buffer_TypeDef* tmp_buff = WHICH_HALF_BUFFER(cs_, phglcd_);
        
        // Check if it is already on the Display
        if(tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] == data_) {return GLCD_OK;}
        
        tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] = data_;

        if( phglcd_->synchronized )
        {
            return GLCD_LL_Write(phglcd_->pglcd_ll, data_, GLCD_LL_FrameType_Data, cs_);
        }
    }
    else
    {
        return GLCD_LL_Write(phglcd_->pglcd_ll, data_, GLCD_LL_FrameType_Data, cs_);
    }

    return GLCD_ERROR;
}


uint8_t GLCD_Driver_Ins_Read_Status(GLCD_Handle_TypeDef* phglcd_, GLCD_LL_ChipSelect_TypeDef cs_)
{
    return GLCD_LL_Read(phglcd_->pglcd_ll, GLCD_LL_FrameType_Instruction, cs_);
}


uint8_t GLCD_Driver_Ins_Read_DispData(GLCD_Handle_TypeDef* phglcd_, GLCD_LL_ChipSelect_TypeDef cs_)
{
    if( phglcd_->buffered )
    {
        GLCD_LL_Buffer_TypeDef* tmp_buff = WHICH_HALF_BUFFER(cs_, phglcd_);
        
        if( phglcd_->synchronized )
        {
            return tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)];
        }
        
        tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)] = GLCD_LL_Read(phglcd_->pglcd_ll, GLCD_LL_FrameType_Data, cs_);
        return tmp_buff->data[FIND_BUFF_DATA_INDEX(tmp_buff)];
    }
    
    return GLCD_LL_Read(phglcd_->pglcd_ll, GLCD_LL_FrameType_Data, cs_);
}

// Status Checks
GLCD_Driver_DispStatOnOff_TypeDef GLCD_Driver_IsDispOnOff(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_)
{
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_Driver_StatusMask_OnOff, GLCD_Driver_Ins_Read_Status(phglcd_, GLCD_Driver_DispHalf_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_Driver_StatusMask_OnOff, GLCD_Driver_Ins_Read_Status(phglcd_, GLCD_Driver_DispHalf_Left));
        return (GLCD_Driver_DispStatOnOff_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_Driver_DispStatOnOff_TypeDef)Extract_Bit(GLCD_Driver_StatusMask_OnOff, GLCD_Driver_Ins_Read_Status(phglcd_, hlf_));
}


GLCD_Driver_DispStatBusy_TypeDef GLCD_Driver_IsDispBusy(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_)
{
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        uint8_t tmp_r = Extract_Bit(GLCD_Driver_StatusMask_Busy, GLCD_Driver_Ins_Read_Status(phglcd_, GLCD_Driver_DispHalf_Right));
        uint8_t tmp_l = Extract_Bit(GLCD_Driver_StatusMask_Busy, GLCD_Driver_Ins_Read_Status(phglcd_, GLCD_Driver_DispHalf_Left));
        return (GLCD_Driver_DispStatBusy_TypeDef)(tmp_r & tmp_l);
    }
    return (GLCD_Driver_DispStatBusy_TypeDef)Extract_Bit(GLCD_Driver_StatusMask_Busy, GLCD_Driver_Ins_Read_Status(phglcd_, hlf_));
}



// GLCD_Driver_DispStatReset_TypeDef GLCD_Driver_IsDispReset(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_)
// {
//     if(hlf_ == GLCD_Driver_DispHalf_Both)
//     {
//         uint8_t tmp_r = Extract_Bit(GLCD_Driver_StatusMask_Reset, GLCD_Driver_Ins_Read_Status(phglcd_, GLCD_Driver_DispHalf_Right));
//         uint8_t tmp_l = Extract_Bit(GLCD_Driver_StatusMask_Reset, GLCD_Driver_Ins_Read_Status(phglcd_, GLCD_Driver_DispHalf_Left));
//         return (GLCD_Driver_DispStatReset_TypeDef)(tmp_r & tmp_l);
//     }
//     return (GLCD_Driver_DispStatReset_TypeDef)Extract_Bit(GLCD_Driver_StatusMask_Reset, GLCD_Driver_Ins_Read_Status(phglcd_, hlf_));
// }



// Initialization
GLCD_Status_TypeDef GLCD_Driver_OnOff(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, GLCD_Driver_DispStatOnOff_TypeDef on_off_)
{
    if( hlf_ != GLCD_Driver_DispHalf_Both )
    {
        return GLCD_Driver_Ins_Disp_OnOff(phglcd_, (GLCD_LL_ChipSelect_TypeDef)hlf_, on_off_);
    }
    else
    {
        if( GLCD_Driver_Ins_Disp_OnOff(phglcd_, GLCD_LL_CS_1, on_off_) == GLCD_OK )
        {
            return GLCD_Driver_Ins_Disp_OnOff(phglcd_, GLCD_LL_CS_2, on_off_);
        }
    }

    
    return GLCD_ERROR;
}



// Set Whole Display Color
GLCD_Status_TypeDef GLCD_Driver_SetWholeDispColor(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, GLCD_Driver_DispColor_TypeDef init_whole_dsp_clr_)
{
    if(GLCD_Driver_GotoXY(phglcd_, hlf_, 0, 0) == GLCD_BUSY){return GLCD_BUSY;}

    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        for (uint8_t i = 0; i <= GLCD_LL_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_Driver_GotoX(phglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_LL_Y_MAX; j++)
            {
                GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Right, i, j);
                GLCD_Status_TypeDef tmp_state = GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Right, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Right, (uint8_t)init_whole_dsp_clr_* 0xFF) != GLCD_OK );
                    }
                }
                GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Left, i, j);
                tmp_state = GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Left, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Left, (uint8_t)init_whole_dsp_clr_* 0xFF) != GLCD_OK );
                    }
                }
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= GLCD_LL_X_MAX; i++)
        {
            for (uint8_t j = 0; j <= GLCD_LL_Y_MAX; j++)
            {
                GLCD_Driver_GotoXY(phglcd_, hlf_, i, j);
                GLCD_Status_TypeDef tmp_state = GLCD_Driver_WriteByte(phglcd_, hlf_, (uint8_t)init_whole_dsp_clr_* 0xFF);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_WriteByte(phglcd_, hlf_, (uint8_t)init_whole_dsp_clr_* 0xFF) != GLCD_OK );
                    }
                }
            }
        }
    }
    
    
    return GLCD_OK;
}



// Write Data
GLCD_Status_TypeDef GLCD_Driver_WriteByte(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t data_)
{
    if(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Write_DispData(phglcd_, GLCD_Driver_DispHalf_Left, data_);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Right) == GLCD_Driver_DispStatBusy_Busy);
        GLCD_Driver_Ins_Write_DispData(phglcd_, GLCD_Driver_DispHalf_Right, data_);

        return GLCD_OK;
    }
    GLCD_Driver_Ins_Write_DispData(phglcd_, hlf_, data_);

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_Driver_WriteByteXY(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_)
{
    if(GLCD_Driver_GotoXY(phglcd_, hlf_, x_, y_) == GLCD_BUSY){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Write_DispData(phglcd_, GLCD_Driver_DispHalf_Left, data_);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Right) == GLCD_Driver_DispStatBusy_Busy);
        GLCD_Driver_Ins_Write_DispData(phglcd_, GLCD_Driver_DispHalf_Right, data_);

        return GLCD_OK;
    }
    GLCD_Driver_Ins_Write_DispData(phglcd_, hlf_, data_);

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_Driver_WriteByteXYZ(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t data_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    if(GLCD_Driver_GotoXYZ(phglcd_, hlf_, x_, y_, z_) == GLCD_BUSY){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Write_DispData(phglcd_, GLCD_Driver_DispHalf_Left, data_);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Right) == GLCD_Driver_DispStatBusy_Busy);
        GLCD_Driver_Ins_Write_DispData(phglcd_, GLCD_Driver_DispHalf_Right, data_);

        return GLCD_OK;
    }
    GLCD_Driver_Ins_Write_DispData(phglcd_, hlf_, data_);

    return GLCD_OK;
}



// Read Data
uint8_t GLCD_Driver_ReadByte(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
    // if(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy){return GLCD_BUSY;}
    
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Left);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Left) == GLCD_Driver_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Left);
        GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Right);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Right) == GLCD_Driver_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Right);

        return tmp_l & tmp_r;
    }
    GLCD_Driver_Ins_Read_DispData(phglcd_, hlf_);    // Dummy read request (Reason in datasheet!)
    while(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy);
    
    return GLCD_Driver_Ins_Read_DispData(phglcd_, hlf_);
}


uint8_t GLCD_Driver_ReadByteXY(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
    // if(GLCD_Driver_GotoXY(phglcd_, hlf_, x_, y_) == GLCD_BUSY){return GLCD_BUSY;}
    GLCD_Driver_GotoXY(phglcd_, hlf_, x_, y_);       // 2B Deleted
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Left);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Left) == GLCD_Driver_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Left);
        GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Right);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Right) == GLCD_Driver_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Right);

        return tmp_l & tmp_r;
    }
    GLCD_Driver_Ins_Read_DispData(phglcd_, hlf_);
    while(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy);
    
    return GLCD_Driver_Ins_Read_DispData(phglcd_, hlf_);
}


uint8_t GLCD_Driver_ReadByteXYZ(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    // FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
    // if(GLCD_Driver_GotoXYZ(phglcd_, hlf_, x_, y_, z_) == GLCD_BUSY){return GLCD_BUSY;}
    GLCD_Driver_GotoXYZ(phglcd_, hlf_, x_, y_, z_);       // 2B Deleted
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Left);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Left) == GLCD_Driver_DispStatBusy_Busy);
        uint8_t tmp_l = GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Left);
        GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Right);
        while(GLCD_Driver_IsDispBusy(phglcd_, GLCD_Driver_DispHalf_Right) == GLCD_Driver_DispStatBusy_Busy);
        uint8_t tmp_r = GLCD_Driver_Ins_Read_DispData(phglcd_, GLCD_Driver_DispHalf_Right);

        return tmp_l & tmp_r;
    }
    
    GLCD_Driver_Ins_Read_DispData(phglcd_, hlf_);
    while(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy);
    
    return GLCD_Driver_Ins_Read_DispData(phglcd_, hlf_);
}


// Set Address Functions (Goto)

GLCD_Status_TypeDef GLCD_Driver_GotoX(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t x_)
{
    if(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_Page(phglcd_, GLCD_Driver_DispHalf_Right, x_);
        GLCD_Driver_Ins_Set_Page(phglcd_, GLCD_Driver_DispHalf_Left, x_);
    }
    else
    {
        GLCD_Driver_Ins_Set_Page(phglcd_, hlf_, x_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_Driver_GotoY(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t y_)
{
    if(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_Address(phglcd_, GLCD_Driver_DispHalf_Right, y_);
        GLCD_Driver_Ins_Set_Address(phglcd_, GLCD_Driver_DispHalf_Left, y_);
    }
    else
    {
        GLCD_Driver_Ins_Set_Address(phglcd_, hlf_, y_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_Driver_GotoZ(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t z_)
{
    if(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_DispStartLine(phglcd_, GLCD_Driver_DispHalf_Right, z_);
        GLCD_Driver_Ins_Set_DispStartLine(phglcd_, GLCD_Driver_DispHalf_Left, z_);
    }
    else
    {
        GLCD_Driver_Ins_Set_DispStartLine(phglcd_, hlf_, z_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_Driver_GotoXY(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_)
{
    if(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_Page(phglcd_, GLCD_Driver_DispHalf_Right, x_);
        GLCD_Driver_Ins_Set_Page(phglcd_, GLCD_Driver_DispHalf_Left, x_);
    }
    else
    {
        GLCD_Driver_Ins_Set_Page(phglcd_, hlf_, x_);
    }
    while(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy);
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_Address(phglcd_, GLCD_Driver_DispHalf_Right, y_);
        GLCD_Driver_Ins_Set_Address(phglcd_, GLCD_Driver_DispHalf_Left, y_);
    }
    else
    {
        GLCD_Driver_Ins_Set_Address(phglcd_, hlf_, y_);
    }

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_Driver_GotoXYZ(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    if(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy){return GLCD_BUSY;}
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_Page(phglcd_, GLCD_Driver_DispHalf_Right, x_);
        GLCD_Driver_Ins_Set_Page(phglcd_, GLCD_Driver_DispHalf_Left, x_);
    }
    else
    {
        GLCD_Driver_Ins_Set_Page(phglcd_, hlf_, x_);
    }
    while(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy);
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_Address(phglcd_, GLCD_Driver_DispHalf_Right, y_);
        GLCD_Driver_Ins_Set_Address(phglcd_, GLCD_Driver_DispHalf_Left, y_);
    }
    else
    {
        GLCD_Driver_Ins_Set_Address(phglcd_, hlf_, y_);
    }
    while(GLCD_Driver_IsDispBusy(phglcd_, hlf_) == GLCD_Driver_DispStatBusy_Busy);
    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        GLCD_Driver_Ins_Set_DispStartLine(phglcd_, GLCD_Driver_DispHalf_Right, z_);
        GLCD_Driver_Ins_Set_DispStartLine(phglcd_, GLCD_Driver_DispHalf_Left, z_);
    }
    else
    {
        GLCD_Driver_Ins_Set_DispStartLine(phglcd_, hlf_, z_);
    }

    return GLCD_OK;
}


// Complex Write Functions
GLCD_Status_TypeDef GLCD_Driver_TrnsprntWriteByte(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_Driver_DispColor_TypeDef clr_)
{
    uint8_t tmp_prev_data = GLCD_Driver_ReadByte(phglcd_, hlf_);

    if(clr_ == GLCD_Driver_DispColor_Black)
    {
        return GLCD_Driver_WriteByte(phglcd_, hlf_, data_ | tmp_prev_data);
    }
    else
    {
        return GLCD_Driver_WriteByte(phglcd_, hlf_, data_ & tmp_prev_data);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_Driver_TrnsprntWriteByteXY(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_Driver_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_)
{
    uint8_t tmp_prev_data = GLCD_Driver_ReadByteXY(phglcd_, hlf_, x_, y_);

    if(clr_ == GLCD_Driver_DispColor_Black)
    {
        return GLCD_Driver_WriteByteXY(phglcd_, hlf_, data_ | tmp_prev_data, x_, y_);
    }
    else
    {
        return GLCD_Driver_WriteByteXY(phglcd_, hlf_, data_ & tmp_prev_data, x_, y_);
    }

    return GLCD_ERROR;
}


GLCD_Status_TypeDef GLCD_Driver_TrnsprntWriteByteXYZ(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_, uint8_t data_, GLCD_Driver_DispColor_TypeDef clr_, uint8_t x_, uint8_t y_, uint8_t z_)
{
    uint8_t tmp_prev_data = GLCD_Driver_ReadByteXYZ(phglcd_, hlf_, x_, y_, z_);

    if(clr_ == GLCD_Driver_DispColor_Black)
    {
        return GLCD_Driver_WriteByteXYZ(phglcd_, hlf_, data_ | tmp_prev_data, x_, y_, z_);
    }
    else
    {
        return GLCD_Driver_WriteByteXYZ(phglcd_, hlf_, data_ & tmp_prev_data, x_, y_, z_);
    }

    return GLCD_ERROR;
}


// Buffering Tools
GLCD_Status_TypeDef GLCD_Driver_SyncBuff_WriteToDisp(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_)
{
    // Save Buffering status and turn it off
    bool tmp_bffrd_flg = phglcd_->buffered;
    phglcd_->buffered = false;

    if(GLCD_Driver_GotoXY(phglcd_, hlf_, 0, 0) == GLCD_BUSY)
    {
        // Restore buffering status
        phglcd_->buffered = tmp_bffrd_flg;
        
        return GLCD_BUSY;
    }
    uint8_t tmp_data = 0x00;

    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        for (uint8_t i = 0; i <= GLCD_LL_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_Driver_GotoX(phglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_LL_Y_MAX; j++)
            {
                GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Right, i, j);
                tmp_data = phglcd_->pbuffer_right->data[j +(i *(GLCD_LL_Y_MAX+1))];
                GLCD_Status_TypeDef tmp_state = GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Right, tmp_data);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Right, tmp_data) != GLCD_OK );
                    }
                }
                GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Left, i, j);
                tmp_data = phglcd_->pbuffer_left->data[j +(i *(GLCD_LL_Y_MAX+1))];
                tmp_state = GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Left, tmp_data);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_WriteByte(phglcd_, GLCD_Driver_DispHalf_Left, tmp_data) != GLCD_OK );
                    }
                }
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= GLCD_LL_X_MAX; i++)
        {
            for (uint8_t j = 0; j <= GLCD_LL_Y_MAX; j++)
            {
                GLCD_Driver_GotoXY(phglcd_, hlf_, i, j);
                if(hlf_ == GLCD_Driver_DispHalf_Right)
                {
                    tmp_data = phglcd_->pbuffer_right->data[j +(i *(GLCD_LL_Y_MAX+1))];
                }
                else
                {
                    tmp_data = phglcd_->pbuffer_left->data[j +(i *(GLCD_LL_Y_MAX+1))];
                }
                GLCD_Status_TypeDef tmp_state = GLCD_Driver_WriteByte(phglcd_, hlf_, tmp_data);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_WriteByte(phglcd_, hlf_, tmp_data) != GLCD_OK );
                    }
                }
            }
        }
    }
    
    // Restore buffering status
    phglcd_->buffered = tmp_bffrd_flg;

    return GLCD_OK;
}


GLCD_Status_TypeDef GLCD_Driver_SyncBuff_ReadFromDisp(GLCD_Handle_TypeDef* phglcd_, GLCD_Driver_DispHalf_TypeDef hlf_)
{
    // Save Buffering status and turn it off
    bool tmp_bffrd_flg = phglcd_->buffered;
    bool tmp_sync_flg  = phglcd_->synchronized;
    phglcd_->buffered     = true;
    phglcd_->synchronized = false;

    if(GLCD_Driver_GotoXY(phglcd_, hlf_, 0, 0) == GLCD_BUSY)
    {
        // Restore buffering status
        phglcd_->buffered     = tmp_bffrd_flg;
        phglcd_->synchronized = tmp_sync_flg;
        
        return GLCD_BUSY;
    }

    if(hlf_ == GLCD_Driver_DispHalf_Both)
    {
        for (uint8_t i = 0; i <= GLCD_LL_X_MAX; i++)
        {
            // UPDATE:
            // This is the right place for setting the address 
            //  (Actually there is no need for setting the y address before 
            //  each write due to its automatic increment in GLCD registers)
            // --> GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Right, i, 0); 
            // It is even logically better to just set the X because Y will
            //  have an overflow and reset to 0 itself!
            // --> GLCD_Driver_GotoX(phglcd_, hlf_, i);
            // As it can be seen, there is also another redundancy:
            //  writing procedure can be done by calling the writebyte func with
            //  "Half_Both" mode! but here it is called seperately for each half
            //  This is because of the unknown [as far] problem that causes
            //  unwanted increase in Y address after some write calls!

            for (uint8_t j = 0; j <= GLCD_LL_Y_MAX; j++)
            {
                GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Right, i, j);
                GLCD_Driver_ReadByte(phglcd_, GLCD_Driver_DispHalf_Right);
                
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
                GLCD_Status_TypeDef tmp_state = GLCD_Driver_ReadByte(phglcd_, GLCD_Driver_DispHalf_Right);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_ReadByte(phglcd_, GLCD_Driver_DispHalf_Right) != GLCD_OK );
                    }
                }
                */
                GLCD_Driver_GotoXY(phglcd_, GLCD_Driver_DispHalf_Left, i, j);
                GLCD_Driver_ReadByte(phglcd_, GLCD_Driver_DispHalf_Left);
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
                GLCD_Status_TypeDef tmp_state = GLCD_Driver_ReadByte(phglcd_, GLCD_Driver_DispHalf_Left);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_ReadByte(phglcd_, GLCD_Driver_DispHalf_Left) != GLCD_OK );
                    }
                }
                */
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= GLCD_LL_X_MAX; i++)
        {
            for (uint8_t j = 0; j <= GLCD_LL_Y_MAX; j++)
            {
                GLCD_Driver_GotoXY(phglcd_, hlf_, i, j);
                GLCD_Driver_ReadByte(phglcd_, hlf_);
                /* FIX: This is ridiculous! return value is uint8_t! it is not distiguishable if it is a 0x02 or GLCD_BUSY!!!
                GLCD_Status_TypeDef tmp_state = GLCD_Driver_ReadByte(phglcd_, hlf_);
                if( tmp_state != GLCD_OK )
                {
                    if( tmp_state == GLCD_BUSY )
                    {
                        while(GLCD_Driver_ReadByte(phglcd_, hlf_); != GLCD_OK );
                    }
                }
                */
            }
        }
    }
    
    // Restore buffering status
    phglcd_->buffered     = tmp_bffrd_flg;
    phglcd_->synchronized = tmp_sync_flg;

    return GLCD_OK;
}
