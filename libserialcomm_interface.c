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

#include "libserialcomm_interface.h"

const char * SerialCommErrString[ErrMessageCount] = {
  "No Error",
  "Temperature sensor fault reading",
  "Pressure Actuator sensor fault reading",
  "Pressure Accumulator sensor fault reading",
  "Serial Check Error, communication went wrong",
  "Emergency contact pressed",
  "Temperature controller error - System deviates from reference",
  "Pressure controller error - System deviates from reference",
  "Pressure Accumulator Error - Accumulator deviates from circuit",
  "All cycles have been completed",
  "Stop received via serial"
};

const char * SerialCommStateString[5] = {
  "Alarm state raised",
  "System is Paused",
  "System is Running",
  "System is Waiting",
  "System is in Serial Setup"
};

int serialcomm_last_error = 0;
void _serialcomm_update_last_error(SerialCommErr err, SerialComm *sc) {
  serialcomm_last_error = (int)err;
  return;
}

extern void *serialcomm_initialize(const char *port) {
  void *sc_v = (void *)serialcomm_open(port, _serialcomm_update_last_error);
  sleep(2);
  serialcomm_sync((SerialComm *)sc_v);
  sleep(1);
  serialcomm_start_listener((SerialComm *)sc_v);
  return sc_v;
}

extern void serialcomm_destroy(void *sc) { 
  serialcomm_close((SerialComm *)sc);
}

extern int serialcomm_check_errors() { 
  return serialcomm_last_error; 
}

extern void serialcomm_update(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdHearthbeat, 0.0);
}

extern float serialcomm_get_t_meas(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.t_meas; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_p_meas(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.p_meas; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_q_meas(void *sc)  { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.q_meas; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_ki(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.ki; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_kp(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.kp; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_t_set(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.t_set; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_p_set(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.p_set; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_u_pres(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.u_pres; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_period(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.period; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_duty_cycle(void *sc)  { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.duty_cycle; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_cycle(void *sc)  { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.cycle; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern float serialcomm_get_cycle_max(void *sc) { 
  float v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = ((SerialComm *)sc)->output.s.max_cycle; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern int serialcomm_get_actuator_config(void *sc) { 
  int v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = (int)((SerialComm *)sc)->output.s.config & CtrlEnPActuator; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern int serialcomm_get_chiller_config(void *sc) { 
  int v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = (int)((SerialComm *)sc)->output.s.config & CtrlEnChiller; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern int serialcomm_get_resistance_config(void *sc) { 
  int v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = (int)((SerialComm *)sc)->output.s.config & CtrlEnResistance; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern int serialcomm_get_state(void *sc) { 
  int v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = (int)((SerialComm *)sc)->output.s.state; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern const char * serialcomm_get_state_string(void *sc) { 
  int st = serialcomm_get_state(sc);
  switch(st) {
    case StateAlarm:
      return SerialCommStateString[0];
      break;
    case StatePause:
      return SerialCommStateString[1];
      break;
    case StateRunning:
      return SerialCommStateString[2];
      break;
    case StateWaiting:
      return SerialCommStateString[3];
      break;
    case StateSerialSetup:
    default:
      return SerialCommStateString[4];
      break;
  }
}

extern int serialcomm_get_error(void *sc) { 
  int v;
  pthread_mutex_lock(&(((SerialComm *)sc)->output_lock));
  v = (int)((SerialComm *)sc)->output.s.error; 
  pthread_mutex_unlock(&(((SerialComm *)sc)->output_lock));
  return v;
}

extern const char *serialcomm_get_error_string(void *sc) {
  char e = serialcomm_get_error(sc);
  return SerialCommErrString[(size_t)e];
}

extern void serialcomm_set_temp(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetTemperature, value);
}

extern void serialcomm_set_p_high(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetPressureHigh, value);
}
extern void serialcomm_set_p_low(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetPressureLow, value);
}

extern void serialcomm_set_ki(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetPIIntegrativeGain, value);
}

extern void serialcomm_set_kp(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetPIProportionalGain, value);
}

extern void serialcomm_set_pres(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetPressure, value);
}

extern void serialcomm_set_u_pres(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdOverridePIControl, value);
}

extern void serialcomm_set_period(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetReferencePeriod, value);
}

extern void serialcomm_set_duty_cycle(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetReferenceDutyCycle, value);
}

extern void serialcomm_set_cycle(void *sc, float value) {
  serialcomm_send((SerialComm *)sc, cmdSetCurrentCycleNumber, value);
}

extern void serialcomm_set_cycle_max(void *sc, float value){
  serialcomm_send((SerialComm *)sc, cmdSetMaximumCycleNumber, value);
}

extern void serialcomm_set_temperature_manual(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdManualTemperatureControl, 0.0);
}

extern void serialcomm_set_pressure_manual(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdManualPressureControl, 0.0);
}

extern void serialcomm_set_temperature_auto(void *sc)  {
  serialcomm_send((SerialComm *)sc, cmdAutomaticTemperatureControl, 0.0);
}

extern void serialcomm_set_pressure_auto(void *sc)  {
  serialcomm_send((SerialComm *)sc, cmdAutomaticPressureControl, 0.0);
}

extern void serialcomm_toggle_chiller(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdToggleChillerActuation, 0.0);
}

extern void serialcomm_toggle_resistance(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdToggleResistanceActuation, 0.0);
}

extern void serialcomm_toggle_pause(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdTogglePauseCycle, 0.0);
}

extern void serialcomm_emergency_stop(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdEmergencyStopCycle, 0.0);
}

extern void serialcomm_system_reboot(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdSystemReboot, 0.0);
}

extern void serialcomm_save_config(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdSaveStorageConfig, 0.0);
}

extern void serialcomm_load_config(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdLoadStorageConfig, 0.0);
}

extern void serialcomm_restore_cycle(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdLoadStorageCycle, 0.0);
}

extern void serialcomm_start_cycle(void *sc) {
  serialcomm_send((SerialComm *)sc, cmdStartCycle, 0.0);
}

