/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "Mc32DriverAdc.h"
#include "peripheral/ports/plib_ports.h"
#include "peripheral/tmr/plib_tmr.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define PIN_TEST_W       LATBbits.LATB6      //Indication dans quel latch on vient écrire pour cette pin
#define PIN_TEST_T       TRISBbits.TRISB6    //Indication de quel tristate correspond a cette pin
#define PIN_TEST_PORT    PORT_CHANNEL_B      //indication du port du channel de la pin
#define PIN_TEST_BIT     PORTS_BIT_POS_6     //Indication de la position du bit de la pin
 
/*Définition pour éviter le hardcodage*/
#define ZERO 0
#define ONE 1
#define COUNT_TO_30_S 29
    
// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
	APP_STATE_SERVICE_TASKS,
    APP_STATE_WAIT,

	/* TODO: Define states used by the application state machine. */

} APP_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;
    /*Résultat de la lecture de l'adc*/
    S_ADCResults adcRes;
    /* TODO: Define any additional data used by the application. */

} APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */
void APP_Tasks( void );

/*******************************************************************************
  Function:
    void APP_TMR0_Callback ( void )

  Summary:
    Fonction de callback pour le timer 1

  Description:
    Cette fonction est appelée dans l'interrution du timer 1 pour mettre à jour 
 * l'APP_STATES depuis l'interruption du timer 1. 
 * Au début du programme, elle est appelée au bout de 3s puis après cela, 
 * elle est appelée toutes les 100ms.

  Precondition:
    

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_TMR0_Callback();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/
void APP_TMR0_Callback(void);

/*******************************************************************************
  Function:
    void TurnOnAllLeds ( void )

  Summary:
    Fonction servant à allumer toutes les LEDs

  Description:
    Cette fonction va allumer les LEDs de LED0 à LED7.

  Precondition:
    

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    TurnOnAllLeds();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/
void TurnOnAllLeds(void);

/*******************************************************************************
  Function:
    void TurnOffAllLeds ( void )

  Summary:
     Fonction servant à éteindre toutes les LEDs.

  Description:
    Cette fonction va éteindre les LEDs de LED0 à LED7.

  Precondition:
    

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    TurnOffAllLeds();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/
void TurnOffAllLeds(void);

/*******************************************************************************
  Function:
    void ChenillardLEDs ( void )

  Summary:
    Cette fonction est la fonction responsable de l'effectuation du chenillard.

  Description:
   Cette fonction va allumer une LED et éteindre la LED précédament allumer.
 * Le if à la fin de la fonction permet de faire en sorte que le chenillard 
 * revient en arrière au lieu de reboucler.

  Precondition:
    

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    ChenillardLEDs();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/
void ChenillardLEDs(void);

/*******************************************************************************
  Function:
    void APP_UpdateState ( APP_STATES newState )

  Summary:
    Cette fonction met à jour l'état du switch case "appData.state".

  Description:
   Cette fonction met à jour l'état du switch case à l'aide du nouvel état reçu
 * en paramètre d'entrée.
 * Elle est appelée dans la fonction APP_TMR0_Callback() ainsi qu'à la fin
 * de l'APP_STATE_INIT et l'APP_STATE_SERVICE_TASK.

  Precondition:
    

  Parameters:
    Necessite le nouvel état voulu pour le APP_STATE.

  Returns:
    None.

  Example:
    <code>
    APP_UpdateState(APP_STATE_WAIT);
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/
void APP_UpdateState(APP_STATES newState);
#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

