/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC4 - GPIO Pins example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2020-2021, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cybsp.h"

/*******************************************************************************
* Macros
********************************************************************************/
#define CYBSP_REFERENCE_GPIO_PORT       GPIO_PRT2
#define CYBSP_REFERENCE_GPIO_NUM        0
#define CYBSP_REFERENCE_GPIO_HSIOM      ioss_0_port_2_pin_0_HSIOM

#ifndef ioss_0_port_2_pin_0_HSIOM
#define ioss_0_port_2_pin_0_HSIOM       HSIOM_SEL_GPIO
#endif

#define USER_BUTTON_INTR_PRIORITY       3
#define INTERRUPT_FLAG_SET              1
#define INTERRUPT_FLAG_CLEAR            0
#define LED_DELAY_MS                    (1000u)

/* This code example assumes that the Device Configurator will automatically
 * configure all GPIO pins of the device. To see how PDL drivers are used
 * to manually configure GPIO pins, set the PDL_PIN_CONFIGURATION #define to 1,
 * otherwise leave it set to 0.
 */
#define PDL_PIN_CONFIGURATION           0

/*******************************************************************************
* Global Variables
********************************************************************************/

uint32_t interruptFlag = INTERRUPT_FLAG_CLEAR;

#if PDL_PIN_CONFIGURATION
/* Structure used to initialize the User Button GPIO using PDL configuration. */
const cy_stc_gpio_pin_config_t CYBSP_USER_BTN_init_struct =
{
    .outVal = 1,                        /* Pin output state */
    .driveMode = CY_GPIO_DM_PULLUP,     /* Drive mode */
    .hsiom = CYBSP_USER_BTN_HSIOM,      /* HSIOM selection */
    .intEdge = CY_GPIO_INTR_RISING,     /* Interrupt Edge type */
    .vtrip = CY_GPIO_VTRIP_CMOS,        /* Input buffer voltage trip type */
    .slewRate = CY_GPIO_SLEW_FAST,      /* Output buffer slew rate */
    .vregEn = 0UL,                      /* SIO pair output buffer mode */
    .ibufMode = 0UL,                    /* SIO pair input buffer mode */
    .vtripSel = 0UL,                    /* SIO pair input buffer trip point */
    .vrefSel = 0UL,                     /* SIO pair reference voltage for input
                                           buffer trip point */
    .vohSel = 0UL,                      /* SIO pair regulated voltage output
                                           level */
};

/* This structure is used to initialize a full GPIO Port using PDL 
 * configuration */
const cy_stc_gpio_prt_config_t PORT4_init_struct =
{
    .dr         = 0x00000000u,  /* Initial output data for the IO pins in
                                   the port */
    .intrCfg    = 0x00000000u,  /* Port interrupt edge detection
                                   configuration */
    .pc         = 0x0EEEEEEEu,  /* Port drive modes and input buffer enable
                                   configuration */
    .pc2        = 0x00000000u,  /* Port input buffer configuration */
    .sio        = 0x00000000u,  /* Port SIO configuration */
    .selActive  = 0x00000000u,  /* HSIOM selection for port pins */
};
#endif

/* User Button interrupt configuration structure */
const cy_stc_sysint_t user_button_intr_config = {
    .intrSrc = CYBSP_USER_BTN_IRQ,                  /* Source of interrupt
                                                       signal */
    .intrPriority = USER_BUTTON_INTR_PRIORITY       /* Interrupt priority */
};

/*******************************************************************************
* Function Name: user_button_interrupt_handler
********************************************************************************
*
* Summary:
*  This function is executed when User Button interrupt is triggered.
*
*******************************************************************************/
void user_button_interrupt_handler()
{
    /* Clears the triggered pin interrupt */
    Cy_GPIO_ClearInterrupt(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM);
    NVIC_ClearPendingIRQ(user_button_intr_config.intrSrc);

    /* Set interrupt flag */
    interruptFlag = INTERRUPT_FLAG_SET;
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  The main function performs the following actions:
*   1. Initializes GPIO pins
*   2. Reads value from Reference pin when User button is pressed.
*   3. Writes value read from the Reference pin to User LED pin.
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Configuration tools automatically generate GPIO configuration code and
     * execute it as part of the device boot process. The following GPIO
     * configuration methods are typically only used with manual PDL GPIO
     * configuration. They may also be used at run time to dynamically
     * reconfigure GPIO pins independent of how the initial configuration
     * was performed.
     */

    /* Most IO pins only require their basic parameters to be set and can use
     * default values for all other settings. This allows use of a simplified
     * initialization function. Cy_GPIO_Pin_FastInit() only supports
     * parameterized configuration of drive mode, output logic level, and HSIOM
     * multiplexer setting. All other configuration settings are untouched. Very
     * useful at run time to dynamically change a pin's configuration.
     * For example, configure a pin to strong drive mode to write data, and then
     * reconfigure as high impedance to read data.
     */
    Cy_GPIO_Pin_FastInit(CYBSP_REFERENCE_GPIO_PORT, CYBSP_REFERENCE_GPIO_NUM, CY_GPIO_DM_HIGHZ, 0, CYBSP_REFERENCE_GPIO_HSIOM);

#if PDL_PIN_CONFIGURATION
    /* The method to configure all attributes of a single pin is to use the
     * Cy_GPIO_Pin_Init() function and configuration structure. While easy to
     * use, it generates larger code than other methods.
     */
    Cy_GPIO_Pin_Init(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM, &CYBSP_USER_BTN_init_struct);

    /* The most code efficient method to configure all attributes for a full
     * port of pins is to use the Cy_GPIO_Port_Init() API function and
     * configuration structure. It packs all the configuration data into direct
     * register writes for the whole port. Its limitation is that it must
     * configure all pins in a port and the user must calculate the combined
     * register values for all pins or copy them from a configuration tool. 
     * This is the method used by automated configuration tools.
     */
    Cy_GPIO_Port_Init(GPIO_PRT4, &PORT4_init_struct);
#else
    /* Individual pin configuration settings can also be changed at run time
     * using supplied driver API functions. An example of some of these
     * functions are provided below.
     */
    Cy_GPIO_SetHSIOM(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM, CYBSP_USER_BTN_HSIOM);
    Cy_GPIO_SetDrivemode(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM, CY_GPIO_DM_PULLUP);
    Cy_GPIO_SetVtrip(CYBSP_USER_BTN_PORT, CY_GPIO_VTRIP_CMOS);
    Cy_GPIO_SetSlewRate(CYBSP_USER_BTN_PORT, CY_GPIO_SLEW_FAST);
    /* Pin Interrupts */
    /* Configure GPIO pin to generate interrupts */
    Cy_GPIO_SetInterruptEdge(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM, CY_GPIO_INTR_RISING);
#endif

    /* Initialize and enable GPIO interrupt */
    result = Cy_SysInt_Init(&user_button_intr_config, user_button_interrupt_handler);
    if(result != CY_SYSINT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    NVIC_ClearPendingIRQ(user_button_intr_config.intrSrc);
    NVIC_EnableIRQ(user_button_intr_config.intrSrc);

    /* Variable to store the value of the Reference pin */
    uint32_t pinReadValue = 0;
    /* Variable to store the value read from the port */
    uint32_t portReadValue = 0;
    /* Variable to store the port number of the Reference pin */
    uint32_t portNumber = 2;

    /* Pin input read methods */

    /* The following code performs the same read from a GPIO using the different
     * read methods available. Please choose the most appropriate method for your
     * specific use case. All Read() functions are thread and multi-core safe.
     */

    /* Pin read using #defines provided by configuration tool pin name is not
     * shown as the pin is not enabled in the configuration tool.
     */

    /* Pin read with user defined custom #define pin name. This is the preferred
     * method for direct PDL use without a configuration tool. #defines
     * are typically placed in .h file but included here for example simplicity
     * and clarity
     */
    pinReadValue = Cy_GPIO_Read(CYBSP_REFERENCE_GPIO_PORT, CYBSP_REFERENCE_GPIO_NUM);

    /* Pin read using default device pin name #defines */
    pinReadValue = Cy_GPIO_Read(P2_0_PORT, P2_0_NUM);

    /* Pin read using default port register name #defines and pin number */
    pinReadValue = Cy_GPIO_Read(GPIO_PRT2, 0);

    /* Pin read using port and pin numbers. Useful for algorithmically generated
     * port and pin numbers. Cy_GPIO_PortToAddr() is a helper function
     * that converts the port number into the required port register base
     * address.
     */
    pinReadValue = Cy_GPIO_Read(Cy_GPIO_PortToAddr(portNumber), 0);

    /* Direct port IN register read with mask and shift of desired pin data */
    pinReadValue = (GPIO_PRT2->PS >> P2_0_NUM) & CY_GPIO_PS_MASK;

    /* This code example uses the pin read with user defined custom #define
     * pin name from here on for simplicity.
     */

    /* Similarly write operations can be performed using different pin write
     * methods. For simplicity, this code example uses the pin write using
     * #defines provided by configuration tool pin name. This is the preferred
     * method for use with configuration tools. Cy_GPIO_Write() API is best used
     * when the desired pin state is not already known and is determined at run
     * time. The Write API uses atomic operations that directly affect only the
     * selected pin without using read-modify-write operations. The Write API is
     * therefore thread and multi-core safe.
     */
    Cy_GPIO_Write(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM, pinReadValue);

    Cy_SysLib_Delay(LED_DELAY_MS);

    /* Pin output methods to directly Set, Clear, and Invert pin output state */

    /* These register writes are atomic operations that directly affect the
     * selected pin without using read-modify-write operations. They are 
     * therefore thread and multi-core safe. These are the most efficient
     * output methods when the desired pin state is already known at compile
     * time. The same argument variations as demonstrated with the
     * Cy_GPIO_Read() API can be used.
     */
    Cy_GPIO_Clr(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
    Cy_SysLib_Delay(LED_DELAY_MS);
    Cy_GPIO_Set(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
    Cy_SysLib_Delay(LED_DELAY_MS);
    Cy_GPIO_Inv(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM);
    Cy_SysLib_Delay(LED_DELAY_MS);

    for(;;)
    {
        /* Simultaneous Port Pin access */

        /* Direct register access is used to interface with multiple pins in
         * one port at the same time. May not be thread or multi-core safe due
         * to possible read-modify-write operations. All pins in a Port under
         * direct register control should only be accessed by a single CPU core.
         */
        portReadValue = GPIO_PRT4->DR;
        portReadValue++;
        GPIO_PRT4->DR = portReadValue;

        if(interruptFlag == INTERRUPT_FLAG_SET)
        {
            /* If interrupt occurs i.e., if button is pressed, read the input
             * value from the Reference pin and write the value to the User LED.
             */
            pinReadValue = Cy_GPIO_Read(CYBSP_REFERENCE_GPIO_PORT, CYBSP_REFERENCE_GPIO_NUM);
            Cy_GPIO_Write(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM, pinReadValue);
            Cy_SysLib_Delay(LED_DELAY_MS);
            /* Clear interrupt flag */
            interruptFlag = INTERRUPT_FLAG_CLEAR;
        }
        else
        {
            Cy_SysLib_Delay(LED_DELAY_MS);
        }
    }
}

/* [] END OF FILE */
