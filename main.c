/******************************************************************************
 * File Name:   main.c
 *
 * Description: This is the source code for the PSoC4 - GPIO Pins example
 *              for ModusToolbox.
 *
 * Related Document: See README.md
 *
 *
 ******************************************************************************
 * (c) 2020-2026, Infineon Technologies AG, or an affiliate of Infineon
 * Technologies AG. All rights reserved.
 * This software, associated documentation and materials ("Software") is
 * owned by Infineon Technologies AG or one of its affiliates ("Infineon")
 * and is protected by and subject to worldwide patent protection, worldwide
 * copyright laws, and international treaty provisions. Therefore, you may use
 * this Software only as provided in the license agreement accompanying the
 * software package from which you obtained this Software. If no license
 * agreement applies, then any use, reproduction, modification, translation, or
 * compilation of this Software is prohibited without the express written
 * permission of Infineon.
 *
 * Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
 * IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
 * THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
 * SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
 * Infineon reserves the right to make changes to the Software without notice.
 * You are responsible for properly designing, programming, and testing the
 * functionality and safety of your intended application of the Software, as
 * well as complying with any legal requirements related to its use. Infineon
 * does not guarantee that the Software will be free from intrusion, data theft
 * or loss, or other breaches ("Security Breaches"), and Infineon shall have
 * no liability arising out of any Security Breaches. Unless otherwise
 * explicitly approved by Infineon, the Software may not be used in any
 * application where a failure of the Product or any consequences of the use
 * thereof can reasonably be expected to result in personal injury.
 *****************************************************************************/

/******************************************************************************
 * Include header files
 *****************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"

/******************************************************************************
 * Macros
 *****************************************************************************/
/* For PSOC4 HVMS family */
#if defined  COMPONENT_PSOC4HVMS128K || COMPONENT_PSOC4HVMS64K
#define CYBSP_REFERENCE_GPIO_PORT       GPIO_PRT0
#define CYBSP_REFERENCE_GPIO_NUM        2
#define CYBSP_REFERENCE_GPIO_HSIOM      ioss_0_port_0_pin_2_HSIOM

#define CYBSP_ACCESSED_GPIO_PORT        GPIO_PRT4

#ifndef ioss_0_port_0_pin_2_HSIOM
#define ioss_0_port_0_pin_2_HSIOM       HSIOM_SEL_GPIO
#endif

#define PRT_NUM 0

/* For PSOC4 HVPA family */
#elif defined  COMPONENT_PSOC4HVPA144K || COMPONENT_PSOC4HVPASPM10
#define CYBSP_REFERENCE_GPIO_PORT       GPIO_PRT0
#define CYBSP_REFERENCE_GPIO_NUM        4
#define CYBSP_REFERENCE_GPIO_HSIOM      ioss_0_port_0_pin_4_HSIOM

#define CYBSP_ACCESSED_GPIO_PORT        GPIO_PRT2

#ifndef ioss_0_port_0_pin_4_HSIOM
#define ioss_0_port_0_pin_4_HSIOM       HSIOM_SEL_GPIO
#endif

#define PRT_NUM 0

/* For other PSOC4 family */
#else
#define CYBSP_REFERENCE_GPIO_PORT       GPIO_PRT2
#define CYBSP_REFERENCE_GPIO_NUM        0
#define CYBSP_REFERENCE_GPIO_HSIOM      ioss_0_port_2_pin_0_HSIOM

#define CYBSP_ACCESSED_GPIO_PORT        GPIO_PRT4

#ifndef ioss_0_port_2_pin_0_HSIOM
#define ioss_0_port_2_pin_0_HSIOM       HSIOM_SEL_GPIO
#endif

#define PRT_NUM 2

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

/******************************************************************************
 * Global Variables
 *****************************************************************************/
uint32_t g_interruptFlag = INTERRUPT_FLAG_CLEAR;

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
};

/* This structure is used to initialize a full GPIO Port using PDL 
 * configuration */
const cy_stc_gpio_prt_config_t ACCESSED_PORT_init_struct =
{
    .dr         = 0x00000000u,  /* Initial output data for the IO pins in
                                   the port */
    .intrCfg    = 0x00000000u,  /* Port interrupt edge detection
                                   configuration */
    .pc         = 0x00DB6DB6u,  /* Port drive modes and input buffer enable
                                   configuration */
    .pc2        = 0x000000FFu,  /* Port input buffer configuration */
    .selActive  = 0x00000000u,  /* HSIOM selection for port pins */
};
#endif

/* User Button interrupt configuration structure */
const cy_stc_sysint_t user_button_intr_config = {
    .intrSrc = CYBSP_USER_BTN_IRQ,                  /* Source of interrupt
                                                       signal */
    .intrPriority = USER_BUTTON_INTR_PRIORITY       /* Interrupt priority */
};

/******************************************************************************
 * Function Name: user_button_interrupt_handler
 ******************************************************************************
 *
 * Summary:
 *  This function is executed when User Button interrupt is triggered.
 *
 *****************************************************************************/
void user_button_interrupt_handler()
{
    /* Clears the triggered pin interrupt */
    Cy_GPIO_ClearInterrupt(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_NUM);
    NVIC_ClearPendingIRQ(user_button_intr_config.intrSrc);

    /* Set interrupt flag */
    g_interruptFlag = INTERRUPT_FLAG_SET;
}

/******************************************************************************
 * Function Name: main
 ******************************************************************************
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
 *****************************************************************************/
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
     * (excluding PSOC4 HVPA-144K which lacks sufficient ports)
     */
    #ifndef COMPONENT_PSOC4HVPA144K
	Cy_GPIO_Port_Init(CYBSP_ACCESSED_GPIO_PORT, &ACCESSED_PORT_init_struct);
    #endif
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

    /* Variable to store the value read from the port
     * (excluding PSOC4 HVPA-144K which lacks sufficient ports)
     */
    #ifndef COMPONENT_PSOC4HVPA144K
    uint32_t portReadValue = 0;
    #endif

    /* Variable to store the port number of the Reference pin */
    uint32_t portNumber = PRT_NUM;

    /**************************************************************************
     * SECTION: GPIO READ METHODS
     * 5 different types of pin input read methods are demonstrated below.
     *
     * - The following code performs the same read from a GPIO using different
     *   read methods available. Choose the most appropriate for your use case.
     * - All Read() functions are thread and multi-core safe.
     * - Pin read using #defines provided by configuration tool pin name is
     *   not shown as the pin is not enabled in the configuration tool.
     *************************************************************************/

    {
        /* 1. Pin read with user defined custom #define pin name
         *    This is the preferred method for direct PDL use without
         *    a configuration tool.
         *    #defines are typically placed in .h file but included here for
         *    example simplicity and clarity with HW abstraction.
         */
        pinReadValue = Cy_GPIO_Read(CYBSP_REFERENCE_GPIO_PORT, CYBSP_REFERENCE_GPIO_NUM);
    }

    {
        /* 2. Pin read using default device pin name #defines */
        #if defined COMPONENT_PSOC4HVMS128K || COMPONENT_PSOC4HVMS64K
        pinReadValue = Cy_GPIO_Read(P0_2_PORT, P0_2_NUM);
        #elif defined COMPONENT_PSOC4HVPA144K || COMPONENT_PSOC4HVPASPM10
        pinReadValue = Cy_GPIO_Read(P0_4_PORT, P0_4_NUM);
        #else
        pinReadValue = Cy_GPIO_Read(P2_0_PORT, P2_0_NUM);
        #endif
    }

    {
        /* 3. Pin read using default port register name #defines and pin number */
        #if defined COMPONENT_PSOC4HVMS128K || COMPONENT_PSOC4HVMS64K
        pinReadValue = Cy_GPIO_Read(GPIO_PRT0, 2);
        #elif defined COMPONENT_PSOC4HVPA144K || COMPONENT_PSOC4HVPASPM10
        pinReadValue = Cy_GPIO_Read(GPIO_PRT0, 4);
        #else
        pinReadValue = Cy_GPIO_Read(GPIO_PRT2, 0);
        #endif
    }

    {
        /* 4. Pin read using port and pin numbers
         *    Useful for algorithmically generated port and pin numbers.
         *    Cy_GPIO_PortToAddr() is a helper function that converts
         *    the port number into the required port register base address.
         */
        #if defined COMPONENT_PSOC4HVMS128K || COMPONENT_PSOC4HVMS64K
        pinReadValue = Cy_GPIO_Read(Cy_GPIO_PortToAddr(portNumber), 2);
        #elif defined COMPONENT_PSOC4HVPA144K || COMPONENT_PSOC4HVPASPM10
        pinReadValue = Cy_GPIO_Read(Cy_GPIO_PortToAddr(portNumber), 4);
        #else
        pinReadValue = Cy_GPIO_Read(Cy_GPIO_PortToAddr(portNumber), 0);
        #endif
    }

    {
        /* 5. Pin read using direct port register access
         *    May not be thread or multi-core safe due to possible 
         *    read-modify-write operations.
         *    Use only when you are certain that no other code will access
         *    the same port pins.
         */
        #if defined COMPONENT_PSOC4HVMS128K || COMPONENT_PSOC4HVMS64K
        pinReadValue = (GPIO_PRT0->PS >> 2) & CY_GPIO_PS_MASK;
        #elif defined COMPONENT_PSOC4HVPA144K || COMPONENT_PSOC4HVPASPM10
        pinReadValue = (GPIO_PRT0->PS >> 4) & CY_GPIO_PS_MASK;
        #else
        pinReadValue = (GPIO_PRT2->PS >> 0) & CY_GPIO_PS_MASK;
        #endif
    }

    /* This code example uses the pin read with user defined custom #define
     * pin name from here on for simplicity.
     */

    /**************************************************************************
     * SECTION: GPIO WRITE METHODS
     *
     * - Write operations can be performed using different pin write methods.
     * - Cy_GPIO_Write() API is best used when the desired pin state is not
     *   already known and is determined at run time.
     * - The Write API uses atomic operations that directly affect only the
     *   selected pin without using read-modify-write operations.
     * - The Write API is therefore thread and multi-core safe.
     *************************************************************************/
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

    /**************************************************************************
     * SECTION: MAIN LOOP - SIMULTANEOUS PORT ACCESS & INTERRUPT HANDLING
     *************************************************************************/
    for(;;)
    {
        /* Simultaneous Port Pin access (Binary Counter Demo)
         * 
         * Direct register access is used to interface with multiple pins in
         * one port at the same time. May not be thread or multi-core safe due
         * to possible read-modify-write operations. All pins in a Port under
         * direct register control should only be accessed by a single CPU core.
         * (excluding PSOC4 HVPA-144K which lacks sufficient ports)
         */
        #ifndef COMPONENT_PSOC4HVPA144K
        portReadValue = CYBSP_ACCESSED_GPIO_PORT->DR;
        portReadValue++;
        CYBSP_ACCESSED_GPIO_PORT->DR = portReadValue;
        #endif

        /* Interrupt-driven LED Control */
        if(g_interruptFlag == INTERRUPT_FLAG_SET)
        {
            /* If interrupt occurs i.e., if button is pressed, read the input
             * value from the Reference pin and write the value to the User LED.
             */
            pinReadValue = Cy_GPIO_Read(CYBSP_REFERENCE_GPIO_PORT, CYBSP_REFERENCE_GPIO_NUM);

            Cy_GPIO_Write(CYBSP_USER_LED_PORT, CYBSP_USER_LED_NUM, pinReadValue);

            Cy_SysLib_Delay(LED_DELAY_MS);
            /* Clear interrupt flag */
            g_interruptFlag = INTERRUPT_FLAG_CLEAR;
        }
        else
        {
            Cy_SysLib_Delay(LED_DELAY_MS);
        }
    }
}

/* [] END OF FILE */
