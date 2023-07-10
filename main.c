#include "main.h"
#include<stdio.h>
int main()
{
    char ch,a[]="\n\rExploreEmbedded";
    int i;

    SystemInit();
    uart_init(9600);  // Initialize the UART0 for 9600 baud rate

    uart_TxChar('h'); //Transmit "hello" char by char
    uart_TxChar('e');
    uart_TxChar('l');
    uart_TxChar('l');
    uart_TxChar('o');


    for(i=0;a[i];i++)  //transmit a predefined string
        uart_TxChar(a[i]);


    while(1)
    {
        //Finally receive a char and transmit it infinitely
        ch = uart_RxChar(); 
        uart_TxChar(ch);
    }       


    SystemInit();

    /* Initialize All the Four UARTs with different Baud rate */
    UART0_Init(9600);   
    UART1_Init(19200);   
    UART2_Init(38400);   
    UART3_Init(115200);  

    while(1)
    {
        /*UARTx_Printf, where suffix "x" specifies the UART channel(0-3)*/
        UART0_Printf("Welcome to LPC1768 UART Programming on channel Zero at 9600 baud\n\r"); 
        UART1_Printf("Welcome to LPC1768 UART Programming on channel One at 19200 baud\n\r"); 
        UART2_Printf("Welcome to LPC1768 UART Programming on channel Two at 38400 baud\n\r"); 
        UART3_Printf("Welcome to LPC1768 UART Programming on channel Three at 115200 baud\n\r"); 
    }
}