/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (c) 2018, Matteo Ragni
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Matteo Ragni.
 * 4. Neither the name of Matteo Ragni nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef MESSAGES_H_
#define MESSAGES_H_

#ifdef ARDUINO
#define FORCE_PACKED
#else
#define FORCE_PACKED __attribute__((packed))
#endif

#define SIGNATURE_MESSAGE 0x63

/** Input Message Protocol */

typedef struct FORCE_PACKED input_s {
  char command;
  float value;
  char check;
} input_s;

#define input_size (sizeof(input_s) - sizeof(char))
#define input_buffer_size sizeof(input_s)

/** Output Message Protocol */

typedef struct FORCE_PACKED output_s {
  float t_meas;     /**< Temeprature measurements */
  float p_meas;     /**< Actuator Pressure measurements */
  float q_meas;     /**< Accumulator pressure measurements */
  float kp;         /**< Pressure proportional gain */
  float ki;         /**< Pressure Integral gain */
  float t_set;      /**< Temeprature set point */
  float p_set;      /**< Pressure set point for actuator */
  float u_pres;     /**< Current output for pressure control */
  float period;     /**< Square wave generator period */
  float duty_cycle; /**< Duty cycle coefficient */
  float cycle;      /**< Cycles number */
  float max_cycle;  /**< Max cycles Number */
  char config;      /**< Actuators configuration */
  char state;       /**< Current state flag */
  char error;       /**< Last error flag */
  char check;       /**< Communication checksum (accumulated xor) */
} output_s;

#define output_size (sizeof(output_s) - sizeof(char))
#define output_buffer_size sizeof(output_s)

typedef enum CommandCode {
  cmdHearthbeat,                  /**< Command from the screen: it is alive! */
  cmdManualTemperatureControl,    /**< Overrides (disabling) the temperature control */
  cmdAutomaticTemperatureControl, /**< Enables the pressure control */
  cmdManualPressureControl,       /**< Overrides (disabling) the pressure control */
  cmdAutomaticPressureControl,    /**< Enables the pressure control */
  cmdTogglePauseCycle,            /**< Pauses / restarts the control system */
  cmdEmergencyStopCycle,          /**< Calls immediately the alarm status */
  cmdToggleChillerActuation,      /**< Enables chiller actuation */
  cmdToggleResistanceActuation,   /**< Enables resistance actuator */
  cmdSetTemperature,              /**< Sets temperature setpoint */
  cmdSetPressureHigh,             /**< Sets pressure set point for reference generator */
  cmdSetPressureLow,              /**< Sets pressure set point for reference generator */
  cmdSetPressure,                 /**< Overrides current pressure set point */
  cmdOverridePIControl,           /**< Override PI Control with voltage input */
  cmdSetPIProportionalGain,       /**< Sets PI proportional gain */
  cmdSetPIIntegrativeGain,        /**< Sets PI integrative gain */
  cmdSetMaximumCycleNumber,       /**< Sets target number for cycles */
  cmdSetCurrentCycleNumber,       /**< Override current Cycle number */
  cmdSetReferencePeriod,          /**< Setup reference square wave period */
  cmdSetReferenceDutyCycle,       /**< Setup reference square wave cycle */
  cmdSystemReboot,                /**< Force an immediate system reboot */
  cmdStartCycle,                  /**< Starts the system (Running state) */
  cmdSaveStorageConfig,           /**< Save current configuration in the EEPROM */
  cmdLoadStorageConfig,           /**< Load storage config from EEPROM. Extremely risky, it may be corrupted data */
  cmdLoadStorageCycle,            /**< Load current cycle number from EEPROM. Extremely Risky it may be corrupted data */
  cmdCommandCodeSize              /**< This last one is a size for the array of function pointers */
} CommandCode;

typedef enum ControlEnabler { CtrlEnChiller = 0x1, CtrlEnResistance = 0x2, CtrlEnPActuator = 0x4 } ControlEnabler;

typedef enum ErrorMessage {
  ErrMsgNoError = 0x0,
  ErrMsgTempSensor = 0x1,
  ErrMsgPActuatorSensor = 0x2,
  ErrMsgPAccumulatorSensor = 0x3,
  ErrMsgSerialCheck = 0x4,
  ErrMsgContactOpen = 0x5,
  ErrMsgTempControlErr = 0x6,
  ErrMsgPresControlErr = 0x7,
  ErrMsgPresAccumulatorErr = 0x8,
  ErrMsgCycleLimit = 0x9,
  ErrMsgSerialStop,
  ErrMessageCount
} ErrorMessage;

typedef enum StateCode {
  StateAlarm = 0x1,
  StatePause = 0x2,
  StateRunning = 0x4,
  StateWaiting = 0x8,
  StateSerialSetup = 0xA,
} StateCode;

#endif /* MESSAGES_H_ */
