#include <lpc17xx.h>
#include <stdint.h>
/*

#define LPC_APB0_BASE         (0x40000000)
#define LPC_APB1_BASE         (0x40080000)
#define LPC_UART0_BASE        (LPC_APB0_BASE + 0x0C000)
#define LPC_UART0             ((LPC_UART_TypeDef      *) LPC_UART0_BASE    )
#define LPC_PINCON_BASE       (LPC_APB0_BASE + 0x2C000)
#define LPC_PINCON            ((LPC_PINCON_TypeDef    *) LPC_PINCON_BASE   )
#define LPC_SC_BASE           (LPC_APB1_BASE + 0x7C000)
#define LPC_SC                ((LPC_SC_TypeDef        *) LPC_SC_BASE       )


typedef struct
{
  union {
			uint8_t  RBR;
			uint8_t  THR;
			uint8_t  DLL;
      uint32_t RESERVED0;
  };
  union {
		uint8_t  DLM;
		uint32_t IER;
  };
  union {
		uint32_t IIR;
    uint8_t  FCR;
  };
		uint8_t  LCR;
    uint8_t  RESERVED1[7];
		uint8_t  LSR;
    uint8_t  RESERVED2[7];
		uint8_t  SCR;
    uint8_t  RESERVED3[3];
		uint32_t ACR;
		uint8_t  ICR;
		uint8_t  RESERVED4[3];
		uint8_t  FDR;
		uint8_t  RESERVED5[7];
		uint8_t  TER;
		uint8_t RESERVED6[3];
} LPC_UART_TypeDef;

typedef struct
{
  
   uint32_t PCON;
   uint32_t PCONP;
               
  uint32_t PCLKSEL0;
  uint32_t PCLKSEL1;
                
 } LPC_SC_TypeDef;

/*------------- Pin Connect Block (PINCON) -----------------------------------*/
/** @brief Pin Connect Block (PINCON) register structure definition */
/*
typedef struct
{
   uint32_t PINSEL0;
   uint32_t PINSEL1;
  
 } LPC_PINCON_TypeDef;
*/

#define SBIT_WordLenght    0x00u
#define SBIT_DLAB          0x07u
#define SBIT_FIFO          0x00u
#define SBIT_RxFIFO        0x01u
#define SBIT_TxFIFO        0x02u

#define SBIT_RDR           0x00u
#define SBIT_THRE          0x05u



/* Function to initialize the UART0 at specifief baud rate */
void uart_init(uint32_t baudrate)
{
    uint32_t var_UartPclk_u32,var_Pclk_u32,var_RegValue_u32;

    LPC_PINCON->PINSEL0 &= ~0x000000F0;
    LPC_PINCON->PINSEL0 |= 0x00000050;            // Enable TxD0 P0.2 and p0.3 

    LPC_UART0->FCR = (1<<SBIT_FIFO) | (1<<SBIT_RxFIFO) | (1<<SBIT_TxFIFO); // Enable FIFO and reset Rx/Tx FIFO buffers    
    LPC_UART0->LCR = (0x03<<SBIT_WordLenght) | (1<<SBIT_DLAB);             // 8bit data, 1Stop bit, No parity


    /** Baud Rate Calculation :
       PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
       Bit6,Bit7 contains the Uart Clock(ie.UART_PCLK) information.
       The UART_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
       (Refer data sheet for more info)
       UART_PCLK    PCLK
         0x00       SystemFreq/4        
         0x01       SystemFreq
         0x02       SystemFreq/2
         0x03       SystemFreq/8   
     **/

    var_UartPclk_u32 = (LPC_SC->PCLKSEL0 >> 6) & 0x03;

    switch( var_UartPclk_u32 )
    {
    case 0x00:
        var_Pclk_u32 = SystemCoreClock/4;
        break;
    case 0x01:
        var_Pclk_u32 = SystemCoreClock;
        break; 
    case 0x02:
        var_Pclk_u32 = SystemCoreClock/2;
        break; 
    case 0x03:
        var_Pclk_u32 = SystemCoreClock/8;
        break;
    }


    var_RegValue_u32 = ( var_Pclk_u32 / (16 * baudrate )); 
    LPC_UART0->DLL =  var_RegValue_u32 & 0xFF;
    LPC_UART0->DLM = (var_RegValue_u32 >> 0x08) & 0xFF;
    LPC_UART0->LCR&=~(SBIT_DLAB);  // Clear DLAB after setting DLL,DLM
}


/* Function to transmit a char */
void uart_TxChar(char ch)
{
    while(!(LPC_UART0->LSR&SBIT_THRE)); // Wait for Previous transmission
    LPC_UART0->THR=ch;                                  // Load the data to be transmitted
}


/* Function to Receive a char */
char uart_RxChar()
{
    char ch;
// while((U0LSR&(1<<0))==0);	
    while(!(LPC_UART0->LSR&SBIT_RDR));  // Wait till the data is received
    ch = LPC_UART0->RBR;                                // Read received data    
    return ch;
}

/*
Fdiv = ( pclk / 16 ) / baudrate ; /*baud rate */

// LPC_UART0->DLM = Fdiv / 256; LPC_UART0->DLL = Fdiv % 256;


