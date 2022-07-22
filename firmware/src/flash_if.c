/*!
    \file    flash_if.c
    \brief   USB DFU device flash interface functions

    \version 2020-08-04, V1.1.0, firmware for GD32VF103
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "flash_if.h"

/* local function prototypes ('static') */
static uint8_t  flash_if_init      (void);
static uint8_t  flash_if_deinit    (void);
static uint8_t  flash_if_erase     (uint32_t addr);
static uint8_t  flash_if_write     (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t* flash_if_read      (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t  flash_if_checkaddr (uint32_t addr);

dfu_mal_prop DFU_Flash_cb =
{
    (const uint8_t *)FLASH_IF_STRING,

    flash_if_init,
    flash_if_deinit,
    flash_if_erase,
    flash_if_write,
    flash_if_read,
    flash_if_checkaddr,
    60, /* flash erase timeout in ms */
    80  /* flash programming timeout in ms (80us * RAM Buffer size (1024 Bytes) */
};

/*!
    \brief      write option byte
    \param[in]  Mem_Add: memory address
    \param[in]  data: pointer to data
    \param[out] none
    \retval     fmc state
*/
fmc_state_enum Option_Byte_Write(uint32_t Mem_Add, uint8_t* data)
{
    fmc_state_enum status ;

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_PGERR | FMC_FLAG_WPERR  | FMC_FLAG_END);

    status = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    /* Authorize the small information block programming */
    ob_unlock();
    
    /* start erase the option byte */
    FMC_CTL |= FMC_CTL_OBER;
    FMC_CTL |= FMC_CTL_START;

    status = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    FMC_CTL &= ~FMC_CTL_OBER;
    /* set the OBPG bit */
    FMC_CTL |= FMC_CTL_OBPG;

    uint8_t index;

    /*OptionBytes always have 16Bytes*/
    for(index = 0U;index<15U;index=index+2U)
    {
        *(__IO uint16_t*)Mem_Add = data[index]&0xffU;

        Mem_Add = Mem_Add + 2U;

        status = fmc_ready_wait(FMC_TIMEOUT_COUNT);
        
    }

    /* if the program operation is completed, disable the OPTPG Bit */
    FMC_CTL &= ~FMC_CTL_OBPG;

    fmc_lock();

    return status;
}

/*!
    \brief      flash memory interface initialization routine
    \param[in]  none
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_init (void)
{
    /* unlock the internal flash */
    fmc_unlock();

    return MAL_OK;
}

/*!
    \brief      flash memory interface de-initialization routine
    \param[in]  none
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_deinit (void)
{
    /* lock the internal flash */
    fmc_lock();

    return MAL_OK;
}

/*!
    \brief      erase flash sector
    \param[in]  addr: flash address to be erased
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_erase (uint32_t addr)
{
    fmc_page_erase(addr);

    return MAL_OK;
}

/*!
    \brief      flash memory write routine
    \param[in]  buf: data buffer pointer
    \param[in]  addr: flash address to be written
    \param[in]  len: length of data to be written (in bytes)
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_write (uint8_t *buf, uint32_t addr, uint32_t len)
{
    uint32_t idx = 0U;

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* not an aligned data */
    if (len & 0x03U) {
        for (idx = len; idx < ((len & 0xFFFCU) + 4U); idx++) {
            buf[idx] = 0xFFU;
        }
    }

    /* data received are word multiple */
    for (idx = 0U; idx < len; idx += 4U) {
        fmc_word_program(addr, *(uint32_t *)(buf + idx));
        addr += 4U;
    }

    fmc_lock();

    return MAL_OK;
}

/*!
    \brief      flash memory read routine
    \param[in]  buf: data buffer pointer
    \param[in]  addr: flash address to be read from
    \param[in]  len: length of data to be read (in bytes)
    \param[out] none
    \retval     pointer to the physical address where data should be read
*/
static uint8_t *flash_if_read (uint8_t *buf, uint32_t addr, uint32_t len)
{
    return  (uint8_t *)(addr);
}

/*!
    \brief      check if the address is an allowed address for this memory
    \param[in]  addr: flash address to be checked
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_checkaddr (uint32_t addr)
{
    if ((addr >= FLASH_START_ADDR) && (addr < FLASH_END_ADDR)) {
        return MAL_OK;
    } else {
        return MAL_FAIL;
    }
}
