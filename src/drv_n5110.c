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
//    파일명     : drv_n5110.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_N5110_LOCAL  

#include "board.h"
#include "drv_n5110.h"
#include "drv_n5110_font.h"
//#include <stdarg.h >




#define N5110_RESET_ON()			GPIO_ResetBits(GPIOC, GPIO_Pin_14)
#define N5110_RESET_OFF()			GPIO_SetBits  (GPIOC, GPIO_Pin_14)

#define N5110_SEL_DATA()			GPIO_SetBits  (GPIOC, GPIO_Pin_15)
#define N5110_SEL_CMD()				GPIO_ResetBits(GPIOC, GPIO_Pin_15)


//----- 내부 함수
void N5110_Reset(void);

uint16_t N5110_WriteCmd( uint8_t CmdData );
uint16_t N5110_WriteByte( uint8_t ByteData );
uint16_t N5110_WriteChar( uint8_t CharData );
uint16_t N5110_WriteStr( char *pStr );
uint16_t N5110_Goto( uint8_t x, uint8_t y );


//----- 내부 변수


void test_( char *format, ... )
{
	char Str[200];
	
	//Lb_sprintf( Str, "Cnt2 : %d",100 );
	
	//Lb_printf( Str );
}

/*---------------------------------------------------------------------------
     TITLE   : N5110_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void N5110_Init(void)
{
	uint8_t i;
	char 	Str[100];
	
	GPIO_InitTypeDef GPIO_InitStructure;



	//-- RESET PC.14
	//
	//REG_GPIOB_CRH &= ~(0x0F << ( 8));	// Clear
	//REG_GPIOB_CRH |=  (0x03 << ( 8));	// MODE, PB.10 Output mode, max speed 50Mhz
	//REG_GPIOB_CRH |=  (0x00 << (10));	// CNF,  PB.10 General purpose output push-pul	

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	//-- DNC PC.15
	//
	//REG_GPIOB_CRH &= ~(0x0F << (12));	// Clear
	//REG_GPIOB_CRH |=  (0x03 << (12));	// MODE, PB.11 Output mode, max speed 50Mhz
	//REG_GPIOB_CRH |=  (0x00 << (14));	// CNF,  PB.11 General purpose output push-pul	

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	spiInit();

	spiSelect(false);


	N5110_RESET_OFF();
	N5110_SEL_CMD();
	
	N5110_Reset();
	
	N5110_WriteCmd(0x21);	// H = 1
	N5110_WriteCmd(0xC6);	// Write Vop
	N5110_WriteCmd(0x06);	// Set Temperature Coefficient
	N5110_WriteCmd(0x13);	// Set Bias System
	N5110_WriteCmd(0x20);	// H = 0
	N5110_WriteCmd(0x0C);	// Set display configuration
							//  - normal mode							
								
	N5110_Clear();
		
}





/*---------------------------------------------------------------------------
     TITLE   : N5110_Reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void N5110_Reset(void)
{
	N5110_RESET_ON();
	delay(10);
	N5110_RESET_OFF();
}





/*---------------------------------------------------------------------------
     TITLE   : N5110_WriteCmd
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint16_t N5110_WriteCmd( uint8_t CmdData )
{
	uint16_t Ret;

	N5110_SEL_CMD();

	//Ret = Hw_SPI_Write8Bit( HW_SPI_CH2, CmdData );
	
	spiSelect(true);
	spiTransferByte(CmdData);
	spiSelect(false);

	Ret = 0;

	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : N5110_WriteByte
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint16_t N5110_WriteByte( uint8_t ByteData )
{
	uint16_t Ret;
	
	N5110_SEL_DATA();
	
	//Ret = Hw_SPI_Write8Bit( HW_SPI_CH2, ByteData );

	spiSelect(true);
	spiTransferByte(ByteData);
	spiSelect(false);

	Ret = 0;
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : N5110_Goto
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint16_t N5110_Goto( uint8_t x, uint8_t y )
{
	uint16_t Ret = 0;
	
	Ret |= N5110_WriteCmd( 0x40|y );
	Ret |= N5110_WriteCmd( 0x80|x );
		
	return Ret;
}






/*---------------------------------------------------------------------------
     TITLE   : N5110_WriteChar
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint16_t N5110_WriteChar( uint8_t CharData )
{
	uint16_t Ret = 0;
	uint16_t i;
	uint8_t  Ch;
	
	for( i=0; i<6; i++ )
	{
		Ch = Hw_N5110_Font6_8[CharData-32][i];
		Ret |= N5110_WriteByte(Ch);
	}	
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : N5110_Clear
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint16_t N5110_Clear( void )
{
	uint16_t Ret = 0;
	uint16_t i;
	
	for( i=0; i<6*84; i++ )
	{
		N5110_WriteByte(0);
	}
	
	return Ret;
}




/*---------------------------------------------------------------------------
     TITLE   : N5110_WriteStr
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint16_t N5110_WriteStr( char *pStr )
{
	u16 Ret = 0;
		
	while( *pStr != '\0' )
	{
		Ret |= N5110_WriteChar( *pStr );
		pStr++;
	}
	
	// 일단 현재 버그때문에 마지막에 추가 데이터를 전송하는데 
	// 버그가 수정되면 삭제 필요 13.9.21
	N5110_WriteByte(0x00);
	
	return Ret;
}




static void N5110_Putc(void *p, char c)
{
    N5110_WriteChar( c );
}


/*---------------------------------------------------------------------------
     TITLE   : N5110_Print
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint16_t N5110_Print( uint8_t x_str, uint8_t y_str,  char *format, ... )
{
	uint16_t Ret = 0;
	
	va_list va;

	va_start(va, format);
		
	Ret |= N5110_Goto( x_str*6, y_str );	

	tfp_format(NULL, N5110_Putc, format, va);	

    
    va_end(va);	

	return Ret;
}
