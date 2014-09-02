//----------------------------------------------------------------------------
//    프로그램명 : N5110 그래픽 LCD 관련 함수 헤더
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_N5110.h
//----------------------------------------------------------------------------
                       
#ifndef __HW_N5110_H__
#define __HW_N5110_H__ 




#ifdef   HW_N5110_LOCAL
#define  EXT_HW_N5110_DEF 
#else
#define  EXT_HW_N5110_DEF     extern
#endif


#include "Hw.h"
#include "Hw_SPI.h"
 


EXT_HW_N5110_DEF void Hw_N5110_Init(void);

EXT_HW_N5110_DEF u16  Hw_N5110_Clear( void );
EXT_HW_N5110_DEF u16  Hw_N5110_Print( u8 x_str, u8 y_str,  char *format, ... );


#endif
