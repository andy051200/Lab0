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
void semaforo_inicio(void);
void semaforo_apagado(void);
void cuenta_p1(void);
void cuenta_p2(void);
void p1_gana (void);
void p2_gana (void);

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
unsigned char antirrebote1; //variable de control para antirrebote 1
unsigned char antirrebote2; //variable de control para antirrebote 2
unsigned char antirrebote3; //variable de control para antirrebote 3
unsigned char jugador1=0;     //variable de control para jugador1
unsigned char jugador2=0;     //variable de control para jugador2

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //interrupciones del timer0
    if (T0IF)
    {
    }
    
    //interrupciones del timer1
    if (PIR1bits.TMR1IF)
    {
    }
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();
    while(1)
    {
        //--------------PARTE PARA BOTON DE INICIO DE JUEGO
        //antirrebota para boton 1, inicio de juego 
        if (PORTBbits.RB0 ==0)
        {
            antirrebote1 = 1;
        }
        
        if (PORTBbits.RB0 == 1 && antirrebote1 == 1)
        {  
            
            semaforo_inicio();      //se llama funcion para semaforo inicial
       
        }
        else
        {
            semaforo_apagado();     //se llama funcion para semaforo apagado
        }
        
        //--------------PARTE PARA BOTON DE JUGADOR 1
        //antirrebota para boton 2, jugador 1
        if (PORTBbits.RB1 ==0)
        {
            antirrebote2 = 1;
        }
        
        if (PORTBbits.RB1 == 1 && antirrebote2 == 1)
        {
            //PORTC++;
            jugador1++;
            cuenta_p1();
        }
        
        //--------------PARTE PARA BOTON DE JUGADOR 2
        //antirrebota para boton 3, jugador 2
        if (PORTBbits.RB2 ==0)
        {
            antirrebote3 = 1;
        }
        
        if (PORTBbits.RB2 == 1 && antirrebote3 == 1)
        {
            //PORTD++;
            jugador2++;
            cuenta_p2();
        }
        
        //--------------PARTE PARA MULTIPLEXADA DE DISPLAYS
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
    TRISE=0;                //PortE como salida para leds semaforo de inicio
    
    PORTA=0;             //se limpia PortA
    PORTB=0;             //se limpia PortB
    PORTC=0;             //se limpia PortC
    PORTD=0;             //se limpia PortD
    PORTE=0;             //se limpia PortE*/
   
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
    
    //WEAK PULL UPs PORTB
    OPTION_REGbits.nRBPU = 0;   // enable Individual pull-ups
    WPUBbits.WPUB0 = 1;         // enable Pull-Up de RB0 
    WPUBbits.WPUB1 = 1;         // enable Pull-Up de RB1 
    WPUBbits.WPUB2 = 1;         // enable Pull-Up de RB2 
    
    //CONFIGURACION DEL TIMER1
    T1CONbits.T1CKPS1 = 1;          // preescaler 11, 1:8
    T1CONbits.T1CKPS0 = 1;          // preescaler 11, 1:8
    T1CONbits.T1OSCEN = 1;          // Timer1 Oscillator Enable 
    T1CONbits.T1SYNC = 1;           // External Clock Input Synchronization
    T1CONbits.TMR1CS = 0;           // Clock Source Select bit, Internal (FOSC/4)
    T1CONbits.TMR1ON = 1;           // bit 0 enables timer
    TMR1H = 10;                 // preset for timer1 MSB register
    TMR1L = 105;                // preset for timer1 LSB register
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilitan las interrupciones globales
    INTCONbits.T0IE=1;          //enable bit de int timer0
    INTCONbits.TMR0IF=0;        //se apaga la bandera de int timer0
    INTCONbits.TMR0IE=1;        // enable bit de IntOnCHangePortB
    INTCONbits.RBIF=0;          // se apaga la bandera de IntOnChangeB  
    PIE1bits.TMR1IE=1;          // se enciende enable de Int Timer1
    PIR1bits.TMR1IF=0;          //se apaga bandera Timer1
    return;
}

/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
//funcion para indicador de semaforo de inicio
void semaforo_inicio()
{
    for (int semaforo=1; semaforo<6;semaforo++)
    {
        switch(semaforo)
        {
            case(1):
                PORTEbits.RE0=0;        //RE0 apagado
                PORTEbits.RE1=0;        //RE1 apagado
                PORTEbits.RE2=0;        //RE2 apagado
                __delay_ms(500);
                break;
                
            case(2):
                PORTEbits.RE0=1;        //RE0 apagado
                PORTEbits.RE1=0;        //RE1 apagado
                PORTEbits.RE2=0;        //RE2 apagado
                __delay_ms(500);
                break;
                
            case(3):
                PORTEbits.RE0=1;        //RE0 apagado
                PORTEbits.RE1=1;        //RE1 apagado
                PORTEbits.RE2=0;        //RE2 apagado
                __delay_ms(500);
                break;
                
            case(4):
                PORTEbits.RE0=1;        //RE0 apagado
                PORTEbits.RE1=1;        //RE1 apagado
                PORTEbits.RE2=1;        //RE2 apagado
                __delay_ms(500);
                break;
            
            case(5):
                PORTEbits.RE0=0;        //RE0 apagado
                PORTEbits.RE1=0;        //RE1 apagado
                PORTEbits.RE2=0;        //RE2 apagado
                __delay_ms(500);
                break;
        }
    }
}

//funcion mientras no se precione el boton 1
void semaforo_apagado()
{
    PORTEbits.RE0=0;        //RE0 apagado
    PORTEbits.RE1=0;        //RE1 apagado
    PORTEbits.RE2=0;        //RE2 apagado
}

//funcion para llevar el contador de decada del jugador 1
void cuenta_p1()
{
    switch(jugador1)
    {
        case(1):
            PORTCbits.RC0=1;
            PORTCbits.RC1=0;
            PORTCbits.RC2=0;
            PORTCbits.RC3=0;
            PORTCbits.RC4=0;
            PORTCbits.RC5=0;
            PORTCbits.RC6=0;
            PORTCbits.RC7=0;
            break;
                    
        case(2):
            PORTCbits.RC0=1;
            PORTCbits.RC1=1;
            PORTCbits.RC2=0;        
            PORTCbits.RC3=0;
            PORTCbits.RC4=0;
            PORTCbits.RC5=0;
            PORTCbits.RC6=0;
            PORTCbits.RC7=0;
            break;
                    
        case(3):
            PORTCbits.RC0=1;
            PORTCbits.RC1=1;
            PORTCbits.RC2=1;
            PORTCbits.RC3=0;
            PORTCbits.RC4=0;
            PORTCbits.RC5=0;
            PORTCbits.RC6=0;
            PORTCbits.RC7=0;
            break;
                    
        case(4):
            PORTCbits.RC0=1;
            PORTCbits.RC1=1;
            PORTCbits.RC2=1;
            PORTCbits.RC3=1;
            PORTCbits.RC4=0;
            PORTCbits.RC5=0;
            PORTCbits.RC6=0;
            PORTCbits.RC7=0;
            break;
                    
        case(5):
            PORTCbits.RC0=1;
            PORTCbits.RC1=1;
            PORTCbits.RC2=1;
            PORTCbits.RC3=1;         
            PORTCbits.RC4=1;         
            PORTCbits.RC5=0; 
            PORTCbits.RC6=0; 
            PORTCbits.RC7=0;
            break;
                    
        case(6):
            PORTCbits.RC0=1; 
            PORTCbits.RC1=1;
            PORTCbits.RC2=1; 
            PORTCbits.RC3=1;
            PORTCbits.RC4=1;
            PORTCbits.RC5=1;
            PORTCbits.RC6=0;
            PORTCbits.RC7=0;
            break;
                    
        case(7):
            PORTCbits.RC0=1;
            PORTCbits.RC1=1;
            PORTCbits.RC2=1;
            PORTCbits.RC3=1;
            PORTCbits.RC4=1;
            PORTCbits.RC5=1;
            PORTCbits.RC6=1;
            PORTCbits.RC7=0;
            break;
                    
        case(8):
            PORTCbits.RC0=1;
            PORTCbits.RC1=1;
            PORTCbits.RC2=1;
            PORTCbits.RC3=1;
            PORTCbits.RC4=1;
            PORTCbits.RC5=1;
            PORTCbits.RC6=1;
            PORTCbits.RC7=1;
            break;
                    
        case(9):
            jugador1=0;
            PORTCbits.RC0=0;
            PORTCbits.RC1=0;
            PORTCbits.RC2=0;
            PORTCbits.RC3=0;
            PORTCbits.RC4=0;
            PORTCbits.RC5=0;
            PORTCbits.RC6=0;
            PORTCbits.RC7=0;
            break;
    }
}

//funcion para llevar el contador de decada del jugador 1
void cuenta_p2()
{
    switch(jugador2)
    {
        case(1):
            PORTDbits.RD0=1;
            PORTDbits.RD1=0;
            PORTDbits.RD2=0;
            PORTDbits.RD3=0;
            PORTDbits.RD4=0;
            PORTDbits.RD5=0;
            PORTDbits.RD6=0;
            PORTDbits.RD7=0;
            break;
                    
        case(2):
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
            PORTDbits.RD2=0;
            PORTDbits.RD3=0;
            PORTDbits.RD4=0;
            PORTDbits.RD5=0;
            PORTDbits.RD6=0;
            PORTDbits.RD7=0;
            break;
                    
        case(3):
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            PORTDbits.RD3=0;
            PORTDbits.RD4=0;
            PORTDbits.RD5=0;
            PORTDbits.RD6=0;
            PORTDbits.RD7=0;
            break;
                    
        case(4):
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            PORTDbits.RD3=1;
            PORTDbits.RD4=0;
            PORTDbits.RD5=0;
            PORTDbits.RD6=0;
            PORTDbits.RD7=0;
            break;
                    
        case(5):
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            PORTDbits.RD3=1;
            PORTDbits.RD4=1;
            PORTDbits.RD5=0;
            PORTDbits.RD6=0;
            PORTDbits.RD7=0;
            break;
                    
        case(6):
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            PORTDbits.RD3=1;
            PORTDbits.RD4=1;
            PORTDbits.RD5=1;
            PORTDbits.RD6=0;
            PORTDbits.RD7=0;
            break;
                    
        case(7):
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            PORTDbits.RD3=1;
            PORTDbits.RD4=1;
            PORTDbits.RD5=1;
            PORTDbits.RD6=1;
            PORTDbits.RD7=0;
            break;
                    
        case(8):
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            PORTDbits.RD3=1;
            PORTDbits.RD4=1;
            PORTDbits.RD5=1;
            PORTDbits.RD6=1;
            PORTDbits.RD7=1;
            break;
                    
        case(9):
            jugador2=0;
            PORTDbits.RD0=0;
            PORTDbits.RD1=0;
            PORTDbits.RD2=0;
            PORTDbits.RD3=0;
            PORTDbits.RD4=0;
            PORTDbits.RD5=0;
            PORTDbits.RD6=0;
            PORTDbits.RD7=0;
            break;   
    }
}

//funcion para desplegar ganador p1 en 7seg
void p1_gana ()
{
}

//funcion para desplegar ganador p2 en 7seg
void p2_gana ()
{
}
