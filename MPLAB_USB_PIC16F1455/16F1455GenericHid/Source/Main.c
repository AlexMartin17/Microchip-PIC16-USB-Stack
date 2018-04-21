#include <stdio.h>
#include <stdlib.h>
#include <pic16f1455.h>
#include <xc.h>
#include "Usb.h"

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is Digital Input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = NOCLKDIV // CPU System Clock Selection Bit (CPU system clock divided by 6)
#pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// Local Defines
#define CommandGetStatus    0x80
#define CommandToggleLed    0x81
#define Button              RC2
#define Led                 RC3

// Local Variables
char ButtonStatus;  // This is to hold last status of the button so that we only report if it changes

// Interrupt
void interrupt ISRCode()
{
    if (UsbInterrupt) ProcessUSBTransactions();
}

static void InitializeSystem(void)
{
    ANSELA =        0x00;
    ANSELC =        0x00;
    OSCTUNE =       0x00;
    OSCCON =        0xFC;           // 16MHz HFINTOSC with 3x PLL enabled (48MHz operation)
    ACTCON =        0x90;           // Enable active clock tuning with USB
    OPTION_REG =    0xC3;           // Set prescaler to 256

    TRISC =         0b00000100;     // Set RC3 as output except RC2 for Button
    LATC =          0b00000000;     // Clear Port C Latches;

    ButtonStatus = 0;
}

static void EnableInterrupts(void)
{
    UIE = 0x4B;             // Transaction complete, Strat Of Frame, Error, Reset
    INTCONbits.PEIE = 1;    // Peripheral Interrupt enable
    INTCONbits.GIE = 1;     // Global Interrupt Enable
    PIE2bits.USBIE = 1;     // Enable Usb Global Interrupt
}


void PrepareTxBuffer(void)
{
    char i;

    HIDTxBuffer[0] = 0x80;      // Ststus
    HIDTxBuffer[1] = Button;    // Button Status
    HIDTxBuffer[2] = Led;       // Led Status

    // Fill The Rest Of Buffer with 0
    for(i = 3 ; i < HidReportByteCount; i++)
    {
        HIDTxBuffer[i] = 0x00;
    }
}

void ProcessIncommingData(void)
{
    if(HIDRxBuffer[0] == CommandGetStatus)
    {
        // Report status
        PrepareTxBuffer();
    }

    if(HIDRxBuffer[0] == CommandToggleLed)
    {
        // Toggle Led
        if(Led) Led = 0; else Led = 1;

        // Report status
        PrepareTxBuffer();
    }

    // PrepareTxBuffer Can be Moved Here and first If Discarded
    // But I Left it like this for better Picture.
}

static void CheckUsb(void)
{
    if(IsUsbDataAvaialble(HidInterfaceNumber) > 0 )
    {
        ProcessIncommingData();
        ReArmInterface(HidInterfaceNumber);
        HIDSend(HidInterfaceNumber);
    }
}

void ProcessIO(void)
{
    // Check USB for incomming Commands
    if (IsUsbReady) CheckUsb();

    // Check Status Of the Button
    if (Button == ButtonStatus ) return;

    // If Button Status Chenged - Report
    PrepareTxBuffer();
    HIDSend(HidInterfaceNumber);

    // Save New Button Status
    ButtonStatus = Button;
}

void main(void)
{
    InitializeSystem();
    InitializeUSB();
    EnableUSBModule();
    EnableInterrupts();

    while(1) { ProcessIO(); }
}
