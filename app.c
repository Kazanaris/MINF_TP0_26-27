/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "Mc32DriverLcd.h"
#include "Mc32Delays.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    //Initialisation du lcd et de l'ADC
    lcd_init();
    BSP_InitADC10();
    
    
    appData.state = APP_STATE_INIT;
    
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /*Variable pour le 1er passage dans l'APP_STATE_SERVICE*/
    static uint8_t AppStateServiceFirstTime = ZERO;
    /*Variable pour appeler la fonction chenillard toutes les 200ms*/
    static uint8_t CountCallChenillard200ms = ZERO;
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /*Affichage du message sur le LCD*/
            lcd_bl_on();
            printf_lcd("Tp0 Led+AD <2026>");
            lcd_gotoxy(1, 2);
            printf_lcd("Kirian Gruber");
            /*Allumage de toutes les LEDs*/
            TurnOnAllLeds();
            /*Demarrage du timer 1*/
            DRV_TMR0_Start();
            /*La pin RB6 est mise à l'état haut.
             *Coupler à l'appel de fonction PLIB_PORTS_PinClear au début de 
             *l'APP_STATE_SERVICE_TASKS, cela permet de vérifier si les 3s
             *demandée avant l'activation de l'APP_STATE_SERVICE_TASKS sont 
             *bien respectées.*/
            PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_6);
            /*Update de l'état de appData, passage en mode wait*/
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            /*La pin RB6 est mise à l'état bas.*/
            PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_6);
            /*Verification si 1er passage dans APP_STATE_SERVICE_TASKS*/
            if(AppStateServiceFirstTime == ZERO)
            {
                /*Eteindre toutes les LEDs*/
                TurnOffAllLeds();
                /**/
                AppStateServiceFirstTime = ONE;
            }
            
            /*Lecture des ADC*/
            appData.adcRes = BSP_ReadAllADC();
            /*Deplacement à la 3ème ligne de la 1ère colonne du LCD*/
            lcd_gotoxy(1, 3);
            /*Affichage des valeurs des potentiomètres sur le LCD*/
            printf_lcd("Ch0 %4d / Ch1 %4d", appData.adcRes.Chan0, appData.adcRes.Chan1);
            
            /*Temporisation pour faire progresser le chenillard toutes les 200ms*/
            if(CountCallChenillard200ms == ZERO)
            {
                CountCallChenillard200ms++;
            }
            else
            {
                /*Appel de la fonction chenillard*/
                ChenillardLEDs();
                CountCallChenillard200ms = ZERO;
            }
            
            /*Update de l'état de appData, passage en mode wait*/
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }

        /* TODO: implement your application state machine.*/
        case APP_STATE_WAIT:
        {
            /*Rien n'est fait ici*/
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

/*******************************************************************************
  Function:
    void APP_TMR0_Callback(void)

  Remarks:
    See prototype in app.h.
 */

void APP_TMR0_Callback(void)
{
    APP_UpdateState(APP_STATE_SERVICE_TASKS);
}

/*******************************************************************************
  Function:
    void APP_UpdateState(APP_STATES newState)

  Remarks:
    See prototype in app.h.
 */

void APP_UpdateState(APP_STATES newState)
{
    appData.state = newState;
}

/*******************************************************************************
  Function:
    void TurnOnAllLeds(void)

  Remarks:
    See prototype in app.h.
 */

void TurnOnAllLeds(void)
{
    BSP_LEDOn(BSP_LED_0); 
    BSP_LEDOn(BSP_LED_1); 
    BSP_LEDOn(BSP_LED_2); 
    BSP_LEDOn(BSP_LED_3); 
    BSP_LEDOn(BSP_LED_4); 
    BSP_LEDOn(BSP_LED_5); 
    BSP_LEDOn(BSP_LED_6); 
    BSP_LEDOn(BSP_LED_7);   
}

/*******************************************************************************
  Function:
    void TurnOffAllLeds(void)

  Remarks:
    See prototype in app.h.
 */

void TurnOffAllLeds(void)
{
    BSP_LEDOff(BSP_LED_0); 
    BSP_LEDOff(BSP_LED_1); 
    BSP_LEDOff(BSP_LED_2); 
    BSP_LEDOff(BSP_LED_3); 
    BSP_LEDOff(BSP_LED_4); 
    BSP_LEDOff(BSP_LED_5); 
    BSP_LEDOff(BSP_LED_6); 
    BSP_LEDOff(BSP_LED_7);  
}

/*******************************************************************************
  Function:
    void ChenillardLEDs(void)

  Remarks:
    See prototype in app.h.
 */

void ChenillardLEDs(void)
{
    /*Variable nous permettant de savoir quel LED doit s'allumer*/
    static uint8_t ChenillardStatus = ZERO;
    /*Variable nous permettant de savoir dans quel sens 
     *doit aller le chnillard (de LED0 à LED7 ou de LED7 à LED0*/
    static uint8_t ChenillardHasReachLed7 = ZERO;
    /*Switch case nous permettant de réaliser le chenillard*/
    switch (ChenillardStatus)
    {
        case 0:
            /*On éteint la LED 1 (sert quand le chenillard va 
             *de la LED7 à la LED0)*/
            BSP_LEDOff(BSP_LED_1);
            /*La LED0 s'allume*/
            BSP_LEDOn(BSP_LED_0);
            /*Indique que le chenillard doit aller de la LED0 à la LED7*/
            ChenillardHasReachLed7 = ZERO;
            break;
            
        case 1:
            /*On éteint la LED 0 (sert quand le chenillard va 
             *de la LED0 à la LED7)*/
            BSP_LEDOff(BSP_LED_0);
            /*On éteint la LED 2 (sert quand le chenillard va 
             *de la LED7 à la LED0)*/
            BSP_LEDOff(BSP_LED_2);
            /*La LED1 s'allume*/
            BSP_LEDOn(BSP_LED_1);
            break;
            
        case 2:
            /*On éteint la LED 1 (sert quand le chenillard va 
             *de la LED0 à la LED7)*/
            BSP_LEDOff(BSP_LED_1);
            /*On éteint la LED 3 (sert quand le chenillard va 
             *de la LED7 à la LED0)*/
            BSP_LEDOff(BSP_LED_3);
            /*La LED2 s'allume*/
            BSP_LEDOn(BSP_LED_2);
            break;
            
        case 3:
            /*On éteint la LED 2 (sert quand le chenillard va 
             *de la LED0 à la LED7)*/
            BSP_LEDOff(BSP_LED_2);
            /*On éteint la LED 4 (sert quand le chenillard va 
             *de la LED7 à la LED0)*/
            BSP_LEDOff(BSP_LED_4);
            /*La LED3 s'allume*/
            BSP_LEDOn(BSP_LED_3);
            break;
            
        case 4:
            /*On éteint la LED 3 (sert quand le chenillard va 
             *de la LED0 à la LED7)*/
            BSP_LEDOff(BSP_LED_3);
            /*On éteint la LED 5 (sert quand le chenillard va 
             *de la LED7 à la LED0)*/
            BSP_LEDOff(BSP_LED_5);
            /*La LED4 s'allume*/
            BSP_LEDOn(BSP_LED_4);
            break;
            
        case 5:
            /*On éteint la LED 4 (sert quand le chenillard va 
             *de la LED0 à la LED7)*/
            BSP_LEDOff(BSP_LED_4);
            /*On éteint la LED 6 (sert quand le chenillard va 
             *de la LED7 à la LED0)*/
            BSP_LEDOff(BSP_LED_6);
            /*La LED5 s'allume*/
            BSP_LEDOn(BSP_LED_5);
            break;
            
        case 6:
            /*On éteint la LED 5 (sert quand le chenillard va 
             *de la LED0 à la LED7)*/
            BSP_LEDOff(BSP_LED_5);
            /*On éteint la LED 7 (sert quand le chenillard va 
             *de la LED7 à la LED0)*/
            BSP_LEDOff(BSP_LED_7);
            /*La LED6 s'allume*/
            BSP_LEDOn(BSP_LED_6);
            break;
            
        case 7:
            /*On éteint la LED 6 (sert quand le chenillard va 
             *de la LED0 à la LED7)*/
            BSP_LEDOff(BSP_LED_6);
            /*La LED7 s'allume*/
            BSP_LEDOn(BSP_LED_7);
            /*Indique que le chenillard doit aller de la LED7 à la LED0*/
            ChenillardHasReachLed7 = ONE;
            break;
            
        default:
            break;
            
         
    }
    /*Ce if nous permet de faire que le chenillard revient en arrière au lieu
     *de reboucler*/
    if(ChenillardHasReachLed7 == ZERO)
    {
        ChenillardStatus++;
    }
    else
    {
        ChenillardStatus--;  
    }
}


/*******************************************************************************
 End of File
 */
