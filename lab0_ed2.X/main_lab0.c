/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: juego de carreras 
Hardware: PIC16F887, 
    
Creado: 8 de julio de 2021    
Descripcion: juego de carreras con dos botones como repaso
------------------------------------------------------------------------------*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <xc.h>
#include <stdint.h>

/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
char antirrebote1;

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();
    while(1)
    {
        
    }
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //CONFIGURACION DE ENTRDAS ANALOGICAS
    ANSEL=0;                //sin entradas analógicas
    ANSELH=0;               //sin entradas analogicas
    
    //CONFIGURACION DE IN-OUT DE PUERTOS
    TRISA=0;                //PortA como salida para displays 7seg
    TRISBbits.TRISB0=1;     //RB0 como entrada para boton 1, inicio
    TRISBbits.TRISB1=1;     //RB0 como entrada para boton 2, jugador 1
    TRISBbits.TRISB2=1;     //RB0 como entrada para boton 3, jugador 2
    TRISC=0;                //PortC como salida para leds jugador 1
    TRISD=0;                //PortD como salida para leds jugador 2
    
    PORTA=0x00;             //se limpia PortA
    PORTB=0x00;             //se limpia PortB
    PORTC=0x00;             //se limpia PortC
    PORTD=0x00;             //se limpia PortD
   
    //CONFIGURACION DE RELOJ
    OSCCONbits.IRCF = 0b110; //Fosc 4MHz
    OSCCONbits.SCS = 1;      //configuracion de reloj interno
    
    //CONFIGURACION DEL TIMER0
    OPTION_REGbits.T0CS = 0;    //configuracion del reloj interno
    OPTION_REGbits.PSA = 0;     //configuracion de preescaler
    OPTION_REGbits.PS2=1;       //PS2 1, preescaler 111
    OPTION_REGbits.PS1=1;       //PS1 1, preescaler 111
    OPTION_REGbits.PS0=1;       //PS0 1, preescaler 111
    TMR0 = 237;                 //valor inicial del timer0
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilitan las interrupciones globales
    INTCONbits.T0IE=1;          //enable bit de int timer0
    INTCONbits.TMR0IF=0;        //se apaga la bandera de int timer0
    INTCONbits.TMR0IE=1;        // enable bit de IntOnCHangePortB
    INTCONbits.RBIF=0;          // se apaga la bandera de IntOnChangeB  
    
    //CONFIGURACION DE INT ON CHANGE PORTB
    IOCBbits.IOCB0=1; //se abilita IntOnChangePortB, pin0
    IOCBbits.IOCB1=1; //se abilita IntOnChangePortB, pin1
    
    
    return;
}

/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/

