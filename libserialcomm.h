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
#ifndef LIBSERIALCOMM_H_
#define LIBSERIALCOMM_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringSerial.h>
#include "messages.h"


typedef union output_u {
  output_s s;
  char b[output_buffer_size];
} output_u;

typedef union input_u {
  input_s s;
  char b[input_buffer_size];
} input_u;

typedef enum SerialState {
  SerialStateOpen,
  SerialStateSync,
  SerialStateClose
} SerialState;

typedef enum SerialCommErr {
  SerialCommErrNoErr = 0,
  SerialCommErrIsClosed,
  SerialCommErrCannotOpen,
  SerialCommErrCannotSetAttr,
  SerialCommErrAllocErr,
  SerialCommErrNotSynced,
  SerialCommErrSendPthread,
  SerialCommErrReceivePthread,
  SerialCommErrBadData
} SerialCommErr;

typedef struct SerialComm SerialComm;

/** \brief Callback for error handling
 * 
 * The callback is called every time there is an error. The 
 * callback receives an error number (which defines the error itself)
 * and a pointer to the serial communication structure.
 */
typedef void (*serialcomm_error_clbk)(SerialCommErr err, SerialComm * sc);

/** \brief SerialComm is the struct which represents the serial connection */
struct SerialComm {
  input_u input; /**< Command union for sending commands */
  output_u output; /**< Machine state input union */
  SerialState state; /**< State of the serial connection */
  pthread_mutex_t input_lock;  /**< Memory lock for writing a new command in memory */
  pthread_mutex_t output_lock; /**< Memory lock for receiving new state from serial */
  pthread_t listener; /**< Incoming messages listener thread */
  char listener_exit; /**< Request for quit listener thread */
  int serial; /**< Serial port descriptor */
  const char * port; /**< Port name */
  serialcomm_error_clbk err_clbk; /**< Error callback for serial */
};

/** \brief Open the serial port
 * 
 * This serial port is configured to work with Arduino: 8N1 at 115200bps.
 * The configuration is fixed, and the user should always call first the 
 * open and at the end the close method in order to meke everything work.
 * Operation performed:
 *  0. Allocates a SerialComm structure (it may fails, returning null)
 *  1. Opens the serial port (it may return a failing state)
 *  2. Configure the serial port settings
 *  3. Applies the serial port configuration (it may fail) and wait it to open
 *  4. Syncronizes the serial ends by flushing and sending the signature byte
 * The failing should be reported through standard error number, but it is also 
 * stated in the sc->state variable.
 * \param port portname for connection
 * \param err pointer to error callback
 * \return a pointer to the SerialComm structure or NULL on error
 */
extern SerialComm * serialcomm_open(const char * port, serialcomm_error_clbk err);
/** \brief The function syncronize the sygnals through a signature
 * 
 * This function synchronize with the other end-point, by sending 
 * a synchronization char that is included asd a define. It also flushes
 * all the input data from serial.
 * \param sc pointer to the communication structure
 */
extern void serialcomm_sync(SerialComm * sc);
/**  \brief Function for thread: Receiving data
 * 
 * The function shall run in a thread and it is used to receive a packet of data from
 * the serial connection. The data received is specified in the output_s, and are copied
 * only once all the data have been received.
 * \param sc_v a pointer to the communication structure
 */
void * serialcomm_receive_thread(void * sc_v);
/** \brief Sending a command to the remote device
 * 
 * The function sends a command over the serial connection.
 * When the command is written by the external send command function, the memory is put in a locked
 * state. When the lock state is intercepted by the function, the memory is copied, the memory is 
 * made unlocked, and then it is written in serial.
 * \param sc a pointer to the communication structure
 * \param cmd The command code to send
 * \param value a float value to send (also for unsigned long, the data to send is float, converted in receiver)
 */
extern void serialcomm_send(SerialComm * sc, CommandCode cmd, float value);
/** \brief Close the connection and frees the space occupied by the SerialComm
 * 
 * The function closes the serial port if it is still open, then frees up
 * the space occupied by the SerialComm structure, that has been previously
 * allocated by the open function.
 * \param sc The serial comm allocated in the open function
 */
extern void serialcomm_close(SerialComm * sc);
/** \brief Request an update, this function in not blocking and uses a thread
 *
 * The function creates a new thread that executes the update of the internal structure.
 * The operation is not blocking and requires some time.
 */
extern void serialcomm_start_listener(SerialComm * sc);


#endif /* LIBSERIALCOMM_H_ */
