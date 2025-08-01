/******************************************************************************
* Copyright (C) 2017, Xiaohua Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Xiaohua Semiconductor Co.,Ltd ("XHSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with XHSC
* components. This software is licensed by XHSC to be adapted only
* for use in systems utilizing XHSC components. XHSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. XHSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* XHSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* XHSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file main.c
**
** A detailed description is available at
** @link Sample Group Some description @endlink
**
**   - 2018-03-09  1.0  Lux First version
**
******************************************************************************/

/******************************************************************************
* Include files
******************************************************************************/
#include "ddl.h"
#include "hdiv.h"
/******************************************************************************
* Local pre-processor symbols/macros ('#define')                            
******************************************************************************/

/******************************************************************************
* Global variable definitions (declared in header file with 'extern')
******************************************************************************/

/******************************************************************************
* Local type definitions ('typedef')                                         
******************************************************************************/

/******************************************************************************
* Local function prototypes ('static')
******************************************************************************/

/******************************************************************************
* Local variable definitions ('static')                                      *
******************************************************************************/

static void Error_Handle()
{
  while(1);
}

/******************************************************************************
* Local pre-processor symbols/macros ('#define')                             
******************************************************************************/

/*****************************************************************************
* Function implementation - global ('extern') and local ('static')
******************************************************************************/
/**
******************************************************************************
** \brief  Main function of project
**
** \return uint32_t return value, if needed
**
******************************************************************************/

int32_t main(void)
{	
  uint32_t  u32Qutient = 0; 
  uint32_t  u32Remainder = 0;
  en_result_t enRc = Ok;
  uint32_t Dividend = 0x000F4240;
  uint16_t Divisor = 0x0000619B;
  stc_div_unsigned_result_t stcDivResult;
  
  DDL_ZERO_STRUCT(stcDivResult);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralDiv,TRUE);
  
  enRc = Hdiv_Unsigned(Dividend,Divisor,&stcDivResult);
  
  if(Ok == enRc)
  {
    u32Qutient = stcDivResult.Quotient;
    u32Remainder = stcDivResult.Remainder;
  }
  else
  {
    Error_Handle();
  }
  u32Qutient = u32Qutient;
  u32Remainder = u32Remainder;
  
  while(1);
}

/******************************************************************************
* EOF (not truncated)
******************************************************************************/


