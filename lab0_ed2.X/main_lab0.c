/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: ganas de hacer el star tarcker pero mas automatizado
Hardware: PIC16F887, 
    
Creado: 15 de junio de 2021    
Descripcion: controlar un motor stepper para el star tracker
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
        PORTAbits.RA0=1;        //pulso para A0_0
        PORTAbits.RA1=0;        //pulso para A1_1
        PORTAbits.RA2=0;        //pulso para A2_2
        PORTAbits.RA3=0;        //pulso para A3_3
        __delay_ms(1000);
        
        PORTAbits.RA0=0;        //pulso para A0_0
        PORTAbits.RA1=1;        //pulso para A1_1
        PORTAbits.RA2=0;        //pulso para A2_2
        PORTAbits.RA3=0;        //pulso para A3_3
        __delay_ms(1000);
        
        PORTAbits.RA0=0;        //pulso para A0_0
        PORTAbits.RA1=0;        //pulso para A1_1
        PORTAbits.RA2=1;        //pulso para A2_2
        PORTAbits.RA3=0;        //pulso para A3_3
        __delay_ms(1000);
        
        PORTAbits.RA0=0;        //pulso para A0_0
        PORTAbits.RA1=0;        //pulso para A1_1
        PORTAbits.RA2=0;        //pulso para A2_2
        PORTAbits.RA3=1;        //pulso para A3_3
        __delay_ms(1000);
        
        /*PORTAbits.RA0=0;        //pulso para A0_0
        PORTAbits.RA1=0;        //pulso para A1_1
        PORTAbits.RA2=1;        //pulso para A2_2
        PORTAbits.RA3=0;        //pulso para A3_3
        __delay_ms(1000);
        
        PORTAbits.RA0=0;        //pulso para A0_0
        PORTAbits.RA1=1;        //pulso para A1_1
        PORTAbits.RA2=0;        //pulso para A2_2
        PORTAbits.RA3=0;        //pulso para A3_3
        __delay_ms(100);*/
        
        
       /* //antirrebote de botones
        if (PORTBbits.RB0 ==0)
        {
            antirrebote1 = 1;
        }
        
        if (PORTBbits.RB0 == 1 && antirrebote1 == 1)
        {   
        }      */
    }
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //CONFIGURACION DE ENTRDAS Y SALIDAS
    ANSEL=0;                //sin entradas analógicas
    ANSELH=0;               //sin entradas analogicas
    
    //CONFIGURACION DE PUERTOS
    TRISAbits.TRISA0=0;     //RA0 como salida
    TRISAbits.TRISA1=0;     //RA1 como salida
    TRISAbits.TRISA2=0;     //RA1 como salida
    TRISAbits.TRISA3=0;     //RA3 como salida
    PORTAbits.RA0=0;        //se limpia pin
    PORTAbits.RA1=0;        //se limpia pin
    PORTAbits.RA2=0;        //se limpia pin
    PORTAbits.RA3=0;        //se limpia pin
    
    //CONFIGURACION DE RELOJ
    OSCCONbits.IRCF = 0b110; //Fosc 4MHz
    OSCCONbits.SCS = 1;      //configuracion de reloj interno
    
    return;
}

/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/

