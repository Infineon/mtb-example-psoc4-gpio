# PSOC&trade; 4: GPIO pins

This example demonstrates multiple methods of configuring, reading, writing, and generating interrupts with PSOC&trade; 4 general purpose input/output (GPIO) pins. See [AN86439 - PSOC&trade; 4 - using GPIO pins](https://www.infineon.com/AN86439) for an overview of GPIO basics, configuration options, mixed-signal use, interrupts, and low-power behavior.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-psoc4-gpio)

[Provide feedback on this code example.](https://yourvoice.infineon.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzE0NzEiLCJTcGVjIE51bWJlciI6IjAwMi0zMTQ3MSIsIkRvYyBUaXRsZSI6IlBTT0MmdHJhZGU7IDQ6IEdQSU8gcGlucyIsInJpZCI6InJvaGFubWFuaXNoLm1hbGhvdHJhQGluZmluZW9uLmNvbSIsIkRvYyB2ZXJzaW9uIjoiMi43LjAiLCJEb2MgTGFuZ3VhZ2UiOiJFbmdsaXNoIiwiRG9jIERpdmlzaW9uIjoiTUNEIiwiRG9jIEJVIjoiSUNXIiwiRG9jIEZhbWlseSI6IlBTT0MifQ==)

## Requirements

- [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) v3.7 or later (tested with v3.7)
- Board support package (BSP) minimum required version: 3.1.0
- Programming language: C
- Associated parts: [PSOC&trade; 4000S, PSOC&trade; 4100S, PSOC&trade; 4100S Plus, PSOC&trade; 4500S, PSOC&trade; 4100S Max, PSOC&trade; 4000T, PSOC&trade; 4100T](https://www.infineon.com/cms/en/product/microcontroller/32-bit-psoc-arm-cortex-microcontroller/psoc-4-32-bit-arm-cortex-m0-mcu/) and [PSOC&trade; 4 HV (High Voltage)](https://www.infineon.com/cms/en/product/microcontroller/32-bit-psoc-arm-cortex-microcontroller/32-bit-psoc-4-hv-arm-cortex-m0/)

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® Embedded Compiler v14.2.1  (`GCC_ARM`) – Default value of `TOOLCHAIN`
- Arm&reg; Compiler v6.22 (`ARM`)
- IAR C/C++ Compiler v9.50.2 (`IAR`)

## Supported kits (make variable 'TARGET')

- [PSOC&trade; 4100S Max Pioneer Kit](https://www.infineon.com/CY8CKIT-041S-MAX) (`CY8CKIT-041S-MAX`) - Default value of `TARGET`
- [PSOC&trade; 4100S Plus Prototyping Kit](https://www.infineon.com/CY8CKIT-149) (`CY8CKIT-149`)
- [PSOC&trade; 4000S CAPSENSE&trade; Prototyping Kit](https://www.infineon.com/CY8CKIT-145-40XX) (`CY8CKIT-145-40XX`)
- [PSOC&trade; 4100S CAPSENSE&trade; Pioneer Kit](https://www.infineon.com/CY8CKIT-041-41XX) (`CY8CKIT-041-41XX`)
- [PSOC&trade; 4500S Pioneer Kit](https://www.infineon.com/CY8CKIT-045S) (`CY8CKIT-045S`)
- [PSOC&trade; 4000T CAPSENSE&trade; Prototyping Kit](https://www.infineon.com/CY8CPROTO-040T) (`CY8CPROTO-040T`)
- [PSOC&trade; 4000T Multi-Sense Prototyping Kit](https://www.infineon.com/CY8CPROTO-040T-MS) (`CY8CPROTO-040T-MS`)
- [PSOC&trade; 4100T Plus CAPSENSE&trade; Prototyping kit](https://www.infineon.com/CY8CPROTO-041TP) (`CY8CPROTO-041TP`)
- [PSOC&trade; 4 HVMS-128K Evaluation Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_psoc4-hvms-128k_lite/) (`KIT_PSOC4-HVMS-128K_LITE`)
- [PSOC&trade; 4 HVMS-64K Evaluation Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_psoc4-hvms-64k_lite/) (`KIT_PSOC4-HVMS-64K_LITE`)
- [PSOC&trade; 4 HVPA-144K Evaluation Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_psoc4-hvpa-144k_lite/) (`KIT_PSOC4-HVPA-144K_LITE`)
- [PSOC&trade; 4 HVPA-SPM 1.0 Evaluation Kit](https://www.infineon.com/design-resources/finder-selection-tools/evaluation-board) (`KIT_PSOC-HVPA-SPM1_LITE`)


## Hardware setup

This example uses the board's default configuration for all the kits listed above, except few kits. Make the hardware changes as mentioned below:

- To use Port 4 header pins on CY8CKIT-045S, populate R53 to R55 and R57 to R61 with 0-Ohm resistors.
- To use the User Button on CY8CPROTO-040T or CY8CPROTO-040T-MS, short J6.2 and J6.3 with the jumper shunt on the kit.

See the kit guide to ensure that the board is configured correctly.

> **Note:** Some of the PSOC&trade; 4 kits ship with KitProg2 installed. ModusToolbox&trade; requires KitProg3. Before using this code example, make sure that the board is upgraded to KitProg3. The tool and instructions are available in the [Firmware Loader](https://github.com/Infineon/Firmware-loader) GitHub repository. If you do not upgrade, you will see an error like "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".

Use jumper wire to externally connect the reference pin (P2[0]) to either logic HIGH (3V3) or logic LOW (GND).
For PSOC&trade; 4 HVMS kits, use jumper wire to externally connect the reference pin (P0[2]) to either logic HIGH (VDDD) or logic LOW (GND).
For PSOC&trade; 4 HVPA kits, use jumper wire to externally connect the reference pin (P0[0]) to either logic HIGH (VDDD) or logic LOW (GND).

Board |[CY8CKIT-041S-MAX](https://www.infineon.com/CY8CKIT-041S-MAX)|[CY8CKIT-149](https://www.infineon.com/CY8CKIT-149)|[CY8CKIT-145-40XX](https://www.infineon.com/CY8CKIT-145-40XX)|[CY8CKIT-041-41XX](https://www.infineon.com/CY8CKIT-041-41XX)|[CY8CKIT-045S](https://www.infineon.com/CY8CKIT-045S)|[CY8CPROTO-040T](https://www.infineon.com/CY8CPROTO-040T)|[CY8CPROTO-040T-MS](https://www.infineon.com/CY8CPROTO-040T-MS)|[CY8CPROTO-041TP](https://www.infineon.com/CY8CPROTO-041TP)|[KIT_PSOC4-HVMS-128K_LITE](https://www.infineon.com/cms/en/product/evaluation-boards/kit_psoc4-hvms-128k_lite/)|[KIT_PSOC4-HVMS-64K_LITE](https://www.infineon.com/cms/en/product/evaluation-boards/kit_psoc4-hvms-64k_lite/)|[KIT_PSOC4-HVPA-144K_LITE](https://www.infineon.com/cms/en/product/evaluation-boards/kit_psoc4-hvpa-144k_lite/)|[KIT_PSOC-HVPA-SPM1_LITE](https://www.infineon.com/design-resources/finder-selection-tools/evaluation-board)
-----| ------| ------ | ------ | ------ | ------ |  ------ | ------ | ------ | ------ | ------ | ------| ------
Reference pin |P2[0]| P2[0] | P2[0] | P2[0] | P2[0] |  P2[0] | P2[0] | P2[0] | P0[2] | P0[2]| P0[4] | P0[4] |
<br>

## Software setup

See the [ModusToolbox&trade; tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.
This example requires no additional software or tools.

## Using the code example

### Create the project

The ModusToolbox&trade; tools package provides the Project Creator as both a GUI tool and a command line tool.

<details><summary><b>Use Project Creator GUI</b></summary>

1. Open the Project Creator GUI tool.

   There are several ways to do this, including launching it from the dashboard or from inside the Eclipse IDE. For more details, see the [Project Creator user guide](https://www.infineon.com/ModusToolboxProjectCreator) (locally available at *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/docs/project-creator.pdf*).

2. On the **Choose Board Support Package (BSP)** page, select a kit supported by this code example. See [Supported kits](#supported-kits-make-variable-target).

   > **Note:** To use this code example for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. On the **Select Application** page:

   a. Select the **Applications(s) Root Path** and the **Target IDE**.

   > **Note:** Depending on how you open the Project Creator tool, these fields may be pre-selected for you.

   b.	Select this code example from the list by enabling its check box.

   > **Note:** You can narrow the list of displayed examples by typing in the filter box.

   c. (Optional) Change the suggested **New Application Name** and **New BSP Name**.

   d. Click **Create** to complete the application creation process.

</details>

<details><summary><b>Use Project Creator CLI</b></summary>

The 'project-creator-cli' tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the 'project-creator-cli' tool. On Windows, use the command-line 'modus-shell' program provided in the ModusToolbox&trade; installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing "modus-shell" in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The following example clones the "[mtb-example-psoc4-gpio](https://github.com/Infineon/mtb-example-psoc4-gpio)" application with the desired name "MyGPIO" configured for the *CY8CKIT-041S-MAX* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CKIT-041S-MAX --app-id mtb-example-psoc4-gpio --user-app-name MyGPIO --target-dir "C:/mtb_projects"
   ```

The 'project-creator-cli' tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the <id> field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the <id> field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

> **Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at {ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf).


</details>



### Open the project

After the project has been created, you can open it in your preferred development environment.

<details><summary><b>Eclipse IDE</b></summary>


If you opened the Project Creator tool from the included Eclipse IDE, the project will open in Eclipse automatically.

For more details, see the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>


<details><summary><b>Visual Studio (VS) Code</b></summary>

Launch VS Code manually, and then open the generated *{project-name}.code-workspace* file located in the project directory.

For more details, see the [Visual Studio Code for ModusToolbox&trade; user guide](https://www.infineon.com/MTBVSCodeUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_vscode_user_guide.pdf*).

</details>


<details><summary><b>Keil µVision</b></summary>

Double-click the generated *{project-name}.cprj* file to launch the Keil µVision IDE.

For more details, see the [Keil µVision for ModusToolbox&trade; user guide](https://www.infineon.com/MTBuVisionUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_uvision_user_guide.pdf*).

</details>


<details><summary><b>IAR Embedded Workbench</b></summary>

Open IAR Embedded Workbench manually, and create a new project. Then select the generated *{project-name}.ipcf* file located in the project directory.

For more details, see the [IAR Embedded Workbench for ModusToolbox&trade; user guide](https://www.infineon.com/MTBIARUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_iar_user_guide.pdf*).

</details>


<details><summary><b>Command line</b></summary>

If you prefer to use the CLI, open the appropriate terminal, and navigate to the project directory. On Windows, use the command-line 'modus-shell' program; on Linux and macOS, you can use any terminal application. From there, you can run various `make` commands.

For more details, see the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Connect the Reference pin to logic LOW externally using a jumper wire. For PSOC&trade; 4 HVMS kits, connect the reference pin to logic HIGH externally using a jumper wire.

2. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE.
   </details>


   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```
   </details>

4. After programming, the application starts automatically.

5. Confirm that the user LED toggles twice.

6. Connect the reference pin to logic HIGH externally using a jumper wire. Press the user button to observe the change in the state of the user LED. When the user button is pressed, the state of the reference pin is read and written to the user LED using the `Cy_GPIO_Write()` PDL API.

7. Now connect the reference pin to logic LOW. Press the user button to observe the change in the state of the user LED.

The value on port 4 pins are incremented continuously and the toggling port pins can be monitored on an oscilloscope. It is not implemented in the KIT_PSOC4-HVPA-144K_LITE.

## Debugging

You can debug the example to step through the code.


<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).

</details>


<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.
</details>


## Design and implementation

This example demonstrates the GPIO pin configuration, reading, writing, full port access, and interrupts using multiple GPIO PDL driver methods. Use the GPIO PDL driver method suitable for your application. See the comments in the *main.c* file or [AN86439 - PSOC&trade; 4 - using GPIO pins](https://www.infineon.com/AN86439) for more details on each PDL driver method.

To demonstrate individual GPIO pin access, this example reads the value from the reference pin and writes it to the user LED. The user LED toggles twice to demonstrate the set/clear function.

The user button is configured to generate an interrupt on a rising edge, which occurs on a button release. The interrupt routine reads the value from the reference pin and writes to the user LED.

A full port of GPIO pins is configured as output pins on port 4. The value on port 4 is continuously read with direct register reads, is incremented, and written back to the port. The toggling port pins can be monitored on an oscilloscope. It is not implemented in the KIT_PSOC4-HVPA-144K_LITE.

### Resources and settings

**Table 1. Application resources**

| Resource  |  Alias/object     |    Purpose     |
| :------- | :------------    | :------------ |
| GPIO (BSP)  | CYBSP_USER_LED        | User LED to show the output              |
| GPIO (BSP)  | CYBSP_USER_BUTTON     | User button to generate the interrupt   |
| GPIO (BSP)  | CYBSP_REFERENCE_PIN   | Reference pin to update the state of user LED   |
| GPIO (BSP)  | CYBSP_ACCESSED_GPIO_PORT | GPIO pins to show port access. In the KIT_PSOC4-HVPA-144K, Port 4 is not supported. |

## Related resources

Resources  | Links
-----------|----------------------------------
Application notes  | [AN79953](https://www.infineon.com/AN79953) – Getting started with PSOC&trade; 4 <br>[AN0034](https://www.infineon.com/row/public/documents/10/42/infineon-an0034-getting-started-with-psoc-4-hv-ms-mcus-in-modustoolbox-applicationnotes-en.pdf) - Getting started with PSOC&trade; 4 HV MS and PSOC&trade; 4 HV PA MCUs in ModusToolbox&trade;
Code examples  | [Using ModusToolbox&trade;](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation |[PSOC&trade; 4 datasheets](https://documentation.infineon.com/psoc4/docs/qqs1702048028479) <br>[PSOC&trade; 4 technical reference manuals](https://documentation.infineon.com/psoc4/docs/hup1702048028817) <br>[PSOC&trade; 4 HV MS 64k datasheets](https://documentation.infineon.com/psoc4atv/docs/lov1747976741007) <br>[PSOC&trade; 4 HV MS 64k reference manuals](https://documentation.infineon.com/psoc4atv/docs/sfz1747976866500) <br>[PSOC&trade; 4 HV MS 128k datasheets](https://documentation.infineon.com/psoc4atv/docs/ypt1703926222536) <br>[PSOC&trade; 4 HV MS 128k reference manuals](https://documentation.infineon.com/psoc4atv/docs/fsx1703926693671) <br>[PSOC&trade; 4 HV PA datasheets](https://documentation.infineon.com/psoc4atv/docs/rsd1669346756301) <br>[PSOC&trade; 4 HV PA reference manuals](https://documentation.infineon.com/psoc4atv/docs/vkg1670389100008)
Development kits | Select your kits from the [Evaluation board finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board) page.
Libraries on GitHub | [mtb-pdl-cat2](https://github.com/Infineon/mtb-pdl-cat2) – PSOC&trade; 4 Peripheral Driver Library (PDL)<br> [mtb-hal-cat2](https://github.com/Infineon/mtb-hal-cat2) – Hardware Abstraction Layer (HAL) library
Tools  | [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSOC&trade; Industrial/IoT MCUs, AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices, XMC&trade; Industrial MCUs, and EZ-USB&trade;/EZ-PD&trade; wired connectivity controllers. ModusToolbox&trade; incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development.

<br>


## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.

## Document history

Document title: *CE231471* - *PSOC&trade; 4: GPIO pins*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example. <br /> This version is not backward compatible with ModusToolbox&trade; software v2.3.
 2.0.0   | Major update to support ModusToolbox&trade; v3.0. <br> This version is not backward compatible with the previous versions of ModusToolbox&trade; software.
 2.1.0   | Added support for CY8CPROTO-040T and updated to support ModusToolbox&trade; v3.1.
 2.2.0   | Added support for KIT_PSOC4-HVMS-128K_LITE and KIT_PSOC4-HVMS-64K_LITE and updated to support ModusToolbox&trade; v3.2.
 2.3.0   | Added support for CY8CPROTO-040T-MS and updated to support ModusToolbox&trade; v3.4.
 2.4.0   | Added support for CY8CPROTO-041TP and updated to support ModusToolbox&trade; v3.5. <br> Changed HVMS kit name PSoC4 to PSOC4.
 2.5.0   | Added support for KIT_PSOC4-HVMS-128K_LITE-02 and KIT_PSOC4-HVMS-64K_LITE-02
 2.6.0   | Added support for KIT_PSOC4-HVPA-144K_LITE
 2.7.0   | Added support for KIT_PSOC-HVPA-SPM1_LITE and updated to support ModusToolbox&trade; v3.7
<br>

All referenced product or service names and trademarks are the property of their respective owners.



The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.

PSOC&trade;, formerly known as PSoC&trade;, is a trademark of Infineon Technologies. Any references to PSoC&trade; in this document or others shall be deemed to refer to PSOC&trade;.

---------------------------------------------------------

(c)2020 - 2026, Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
This software, associated documentation and materials ("Software") is owned by Infineon Technologies AG or one of its affiliates ("Infineon") and is protected by and subject to worldwide patent protection, worldwide copyright laws, and international treaty provisions. Therefore, you may use this Software only as provided in the license agreement accompanying the software package from which you obtained this Software. If no license agreement applies, then any use, reproduction, modification, translation, or compilation of this Software is prohibited without the express written permission of Infineon.
<br>
Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A SPECIFIC USE/PURPOSE OR MERCHANTABILITY. Infineon reserves the right to make changes to the Software without notice. You are responsible for properly designing, programming, and testing the functionality and safety of your intended application of the Software, as well as complying with any legal requirements related to its use. Infineon does not guarantee that the Software will be free from intrusion, data theft or loss, or other breaches (“Security Breaches”), and Infineon shall have no liability arising out of any Security Breaches. Unless otherwise explicitly approved by Infineon, the Software may not be used in any application where a failure of the Product or any consequences of the use thereof can reasonably be expected to result in personal injury.

