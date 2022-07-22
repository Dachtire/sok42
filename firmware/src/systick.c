/*!
    \file    systick.c
    \brief   the systick configuration file

    \version 2019-06-05, V1.0.0, firmware for GD32VF103
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

#include "gd32vf103.h"
#include "systick.h"
//#include "main.h"

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;

    /* get current timer value */
    uint64_t tmp = get_timer_value();
    do{
        start_mtime = get_timer_value();
    }while(start_mtime == tmp);

    /* continue counting until the delay time is reached */
    do{
        delta_mtime = get_timer_value() - start_mtime;
    }while(delta_mtime < (SystemCoreClock / 4000 * count));
}

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
//    eclic_global_interrupt_enable();

//    frequence = x Hz
//    mtimecmp = 1 / x second.
    #ifdef KB_CTL_ADC
        *(uint64_t*)(TIMER_CTRL_ADDR + TIMER_MTIMECMP) = TIMER_FREQ / 10000;
    #else
    //    1ms
    //    *(uint64_t*)(TIMER_CTRL_ADDR + TIMER_MTIMECMP) = TIMER_FREQ / 1000;

        //    1/8 ms
        *(uint64_t*)(TIMER_CTRL_ADDR + TIMER_MTIMECMP) = TIMER_FREQ / 8000;
    #endif
    //    eclic_set_nlbits(ECLIC_GROUP_LEVEL2_PRIO2);
        eclic_irq_enable(CLIC_INT_TMR, 3, 3);
        *(uint64_t*)(TIMER_CTRL_ADDR + TIMER_MTIME) = 0;

}