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



 


EXT_HW_N5110_DEF void N5110_Init(void);

EXT_HW_N5110_DEF uint16_t  N5110_Clear( void );
EXT_HW_N5110_DEF uint16_t  N5110_Print( uint8_t x_str, uint8_t y_str,  char *format, ... );
EXT_HW_N5110_DEF uint16_t  N5110_WriteChar( uint8_t CharData );


#endif
