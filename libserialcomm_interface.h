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

#ifndef LIBSERIALCOMM_INTERFACE_H_
#define LIBSERIALCOMM_INTERFACE_H_

/** \brief A simpler interface for communication
 *
 * This file describes a simpler interface for communication
 * that handles everything, from the simpler opening of the serial port,
 * the listener launch and the callback for sending and receiving data.
 *
 * All the simplified interface runs on a void pointer that is used
 * to store the situation of the serial port.
 */

#include "libserialcomm.h"
#include "messages.h"

/** \brief Launches the connection on the serial port
 *
 * \param port the serial port string
 * \return a pointer for preserving the state of the serial port
 */
extern void *serialcomm_initialize(const char *port);
/** \brief Closes the connection and detaches the listener
 *
 * \param sc pointer to memory that saves the state of the serial port
 */
extern void serialcomm_destroy(void *sc);
/** \brief check if an error was raised.
 *
 * With no error, the function returns 0. The error number is global,
 * thus it do not depend on the serial port opened.
 */
extern int serialcomm_check_errors(void);
/** \brief Request an update from the remote endpoint
 *
 * The remote sends an update for the state only when requested
 * through an hearthbeat command. This function sends an hearthbeat.
 * \param sc pointer to memory that saves the state of the serial port.
 */
extern void serialcomm_update(void *sc);

/** \brief Receiving information function (to run after an update) */
extern float serialcomm_get_t_meas(void *sc);
extern float serialcomm_get_p_meas(void *sc);
extern float serialcomm_get_q_meas(void *sc);
extern float serialcomm_get_ki(void *sc);
extern float serialcomm_get_kp(void *sc);
extern float serialcomm_get_t_set(void *sc);
extern float serialcomm_get_p_set(void *sc);
extern float serialcomm_get_u_pres(void *sc);
extern float serialcomm_get_period(void *sc);
extern float serialcomm_get_duty_cycle(void *sc);
extern float serialcomm_get_cycle(void *sc);
extern float serialcomm_get_cycle_max(void *sc);
extern int serialcomm_get_actuator_config(void *sc);
extern int serialcomm_get_chiller_config(void *sc);
extern int serialcomm_get_resistance_config(void *sc);
extern int serialcomm_get_state(void *sc);
extern int serialcomm_get_error(void *sc);
extern const char *serialcomm_get_error_string(void *sc);
extern const char *serialcomm_get_state_string(void *sc);

/** \brief Send command function */

extern void serialcomm_set_temp(void *sc, float value);
extern void serialcomm_set_p_high(void *sc, float value);
extern void serialcomm_set_p_low(void *sc, float value);
extern void serialcomm_set_ki(void *sc, float value);
extern void serialcomm_set_kp(void *sc, float value);
extern void serialcomm_set_pres(void *sc, float value);
extern void serialcomm_set_u_pres(void *sc, float value);
extern void serialcomm_set_period(void *sc, float value);
extern void serialcomm_set_duty_cycle(void *sc, float value);
extern void serialcomm_set_cycle(void *sc, float value);
extern void serialcomm_set_cycle_max(void *sc, float value);
extern void serialcomm_set_temperature_manual(void *sc);
extern void serialcomm_set_pressure_manual(void *sc);
extern void serialcomm_set_temperature_auto(void *sc);
extern void serialcomm_set_pressure_auto(void *sc);
extern void serialcomm_toggle_chiller(void *sc);
extern void serialcomm_toggle_resistance(void *sc);
extern void serialcomm_toggle_pause(void *sc);
extern void serialcomm_emergency_stop(void *sc);
extern void serialcomm_system_reboot(void *sc);
extern void serialcomm_save_config(void *sc);
extern void serialcomm_load_config(void *sc);
extern void serialcomm_restore_cycle(void *sc);
extern void serialcomm_start_cycle(void *sc);

#endif /* LIBSERIALCOMM_INTERFACE_H_ */
