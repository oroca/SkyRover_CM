//----------------------------------------------------------------------------
//    프로그램명 : N5110 그래픽 LCD 관련 함수
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_N5110.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_N5110_LOCAL  

#include "Hw_N5110.h"
#include "Hw_N5110_Font.h"
//#include <stdarg.h >



#define HW_N5110_RESET_ON()			CLR_BIT(REG_GPIOB_ODR, 10)
#define HW_N5110_RESET_OFF()		SET_BIT(REG_GPIOB_ODR, 10)

#define HW_N5110_SEL_DATA()			SET_BIT(REG_GPIOB_ODR, 11)
#define HW_N5110_SEL_CMD()			CLR_BIT(REG_GPIOB_ODR, 11)


//----- 내부 함수
void Hw_N5110_Reset(void);

u16 Hw_N5110_WriteCmd( u8 CmdData );
u16 Hw_N5110_WriteByte( u8 ByteData );
u16 Hw_N5110_WriteChar( u8 CharData );
u16 Hw_N5110_WriteStr( char *pStr );
u16 Hw_N5110_Goto( u8 x, u8 y );


//----- 내부 변수


void test_( char *format, ... )
{
	char Str[200];
	
	Lb_sprintf( Str, "Cnt2 : %d",100 );
	
	Lb_printf( Str );
}

/*---------------------------------------------------------------------------
     TITLE   : Hw_5110_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110_Init(void)
{
	u8 i;
	char Str[100];
	
	Hw_SPI_Init();	
	
	//-- RESET PB.10
	//
	REG_GPIOB_CRH &= ~(0x0F << ( 8));	// Clear
	REG_GPIOB_CRH |=  (0x03 << ( 8));	// MODE, PB.10 Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x00 << (10));	// CNF,  PB.10 General purpose output push-pul	

	//-- DNC PB.11
	//
	REG_GPIOB_CRH &= ~(0x0F << (12));	// Clear
	REG_GPIOB_CRH |=  (0x03 << (12));	// MODE, PB.11 Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x00 << (14));	// CNF,  PB.11 General purpose output push-pul	


	HW_N5110_RESET_OFF();
	HW_N5110_SEL_CMD();
	
	Hw_N5110_Reset();
	
	Hw_N5110_WriteCmd(0x21);	// H = 1
	Hw_N5110_WriteCmd(0xC6);	// Write Vop
	Hw_N5110_WriteCmd(0x06);	// Set Temperature Coefficient
	Hw_N5110_WriteCmd(0x13);	// Set Bias System
	Hw_N5110_WriteCmd(0x20);	// H = 0
	Hw_N5110_WriteCmd(0x0C);	// Set display configuration
								//  - normal mode							
								
	Hw_N5110_Clear();
		
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_5110_Reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_N5110_Reset(void)
{
	HW_N5110_RESET_ON();
	Hw_Timer_Delay_ms(10);
	HW_N5110_RESET_OFF();
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_5110_Reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110_WriteCmd( u8 CmdData )
{
	u16 Ret;
	
	HW_N5110_SEL_CMD();

	Ret = Hw_SPI_Write8Bit( HW_SPI_CH2, CmdData );
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110_WriteByte
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110_WriteByte( u8 ByteData )
{
	u16 Ret;
	
	HW_N5110_SEL_DATA();
	
	Ret = Hw_SPI_Write8Bit( HW_SPI_CH2, ByteData );
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110_Goto
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110_Goto( u8 x, u8 y )
{
	u16 Ret = ERR_NONE;
	
	Ret |= Hw_N5110_WriteCmd( 0x40|y );
	Ret |= Hw_N5110_WriteCmd( 0x80|x );
		
	return Ret;
}






/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110_WriteChar
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110_WriteChar( u8 CharData )
{
	u16 Ret = ERR_NONE;
	u16 i;
	u8  Ch;
	
	for( i=0; i<6; i++ )
	{
		Ch = Hw_N5110_Font6_8[CharData-32][i];
		Ret |= Hw_N5110_WriteByte(Ch);
	}	
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110_Clear
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110_Clear( void )
{
	u16 Ret;
	u16 i;
	
	for( i=0; i<6*84; i++ )
	{
		Hw_N5110_WriteByte(0);
	}
	
	return Ret;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110_WriteStr
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110_WriteStr( char *pStr )
{
	u16 Ret = ERR_NONE;
		
	while( *pStr != '\0' )
	{
		Ret |= Hw_N5110_WriteChar( *pStr );
		pStr++;
	}
	
	// 일단 현재 버그때문에 마지막에 추가 데이터를 전송하는데 
	// 버그가 수정되면 삭제 필요 13.9.21
	Hw_N5110_WriteByte(0x00);
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_N5110_Print
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_N5110_Print( u8 x_str, u8 y_str,  char *format, ... )
{
	u16 Ret = ERR_NONE;
	char Str[200];
	Lb_va_list ap;

	Lb_va_start( ap, format );
		
	Lb_vsprintf(Str, format, ap);	
		
	Ret |= Hw_N5110_Goto( x_str*6, y_str );	

	Ret |= Hw_N5110_WriteStr( Str );	

	Lb_va_end(ap);		
	
	return Ret;
}
