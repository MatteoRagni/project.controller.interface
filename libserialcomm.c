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
#include <wiringSerial.h>
#include "libserialcomm.h"


/** \brief Evaluates the checksum */
char serialcomm_lcr_check(char * b, size_t size) {
  char sum = 0x00;
  for (size_t i = 0; i < size; i++)
    sum ^= b[i];
  return sum;
}

extern SerialComm * serialcomm_open(const char * port, serialcomm_error_clbk err) {
  // Phase 1: Initializes the structure
  SerialComm * sc = (SerialComm*)malloc(sizeof(SerialComm));
  if (!sc) {
    if (err)
      err(SerialCommErrAllocErr, NULL);
    return NULL;
  }

  memset(sc->input.b, 0, input_buffer_size);
  memset(sc->output.b, 0, output_buffer_size);
  sc->state = SerialStateClose;
  sc->listener_exit = 0;

  // Preparing memory lock systems
  pthread_mutex_init(&(sc->input_lock), NULL);
  pthread_mutex_init(&(sc->output_lock), NULL);

  sc->port = port;
  sc->err_clbk = err;
  sc->serial = serialOpen(sc->port, 115200);

  if (sc->serial < 0) {
    if (sc->err_clbk)
      sc->err_clbk(SerialCommErrCannotOpen, sc);
    serialcomm_close(sc);
    return NULL;
  }
  sc->state = SerialStateOpen;
  return sc;
} // serialcomm_opne

extern void serialcomm_sync(SerialComm * sc) {
  if (!sc) {
    return;
  }
  if (sc->state != SerialStateOpen) {
    if (sc->err_clbk)
      sc->err_clbk(SerialCommErrIsClosed, sc);
    return;
  }
  serialFlush(sc->serial);
  serialPutchar(sc->serial, SIGNATURE_MESSAGE);
  sc->state = SerialStateSync;
} // serialcomm_sync


extern void serialcomm_send(SerialComm * sc, CommandCode cmd, float value) {
  if (!sc)
    return;

  if (sc->state != SerialStateSync) {
    if (sc->err_clbk)
      sc->err_clbk(SerialCommErrNotSynced, sc);
    return;
  }
  
  pthread_mutex_lock(&(sc->input_lock));
  sc->input.s.command = cmd;
  sc->input.s.value = value;
  sc->input.s.check = serialcomm_lcr_check(sc->input.b, input_size);
  pthread_mutex_unlock(&(sc->input_lock));
  
  pthread_mutex_lock(&(sc->input_lock));
  for (size_t i = 0; i < input_buffer_size; i++) 
    serialPutchar(sc->serial, sc->input.b[i]);
  pthread_mutex_unlock(&(sc->input_lock));
} // serialcomm_send


extern void serialcomm_close(SerialComm * sc) {
  if (sc) {
    sc->listener_exit = 1;
    pthread_join(sc->listener, NULL); // Joining listener thread

    pthread_mutex_unlock(&(sc->input_lock));
    pthread_mutex_destroy(&(sc->input_lock));

    pthread_mutex_unlock(&(sc->output_lock));
    pthread_mutex_destroy(&(sc->output_lock));
    
    if (sc->serial > 0) {
      serialClose(sc->serial);
    }
    free(sc);
  }
} // serialcomm_close


extern void serialcomm_start_listener(SerialComm * sc) {
  if (!sc)
    return;

  if (sc->state != SerialStateSync) {
    if (sc->err_clbk)
      sc->err_clbk(SerialCommErrNotSynced, sc);
    return;
  }
  
  if (pthread_create(&(sc->listener), NULL, serialcomm_receive_thread, (void*)sc)) {
    sc->err_clbk(SerialCommErrSendPthread, sc);
    return;
  }
} // serialcomm_start_listener

void * serialcomm_receive_thread(void * sc_v) {
  if (!sc_v)
    pthread_exit(NULL);

  SerialComm * sc = (SerialComm*)sc_v;
  if (sc->state != SerialStateSync) {
    if (sc->err_clbk)
      sc->err_clbk(SerialCommErrNotSynced, sc);
    pthread_exit(NULL);
  }

  serialFlush(sc->serial);

  // Reading operation
  char buffer[output_buffer_size];
  size_t pos = 0;

  while (!sc->listener_exit) {
    if (serialDataAvail(sc->serial) > 0) {
      char b = serialGetchar(sc->serial);
      buffer[pos++] = b;

      if (pos >= output_buffer_size) {
        char check = serialcomm_lcr_check(buffer, output_size);
        if (check == buffer[output_size]) {
          pthread_mutex_lock(&(sc->output_lock));
          memcpy((void*)(sc->output.b), (void*)buffer, output_buffer_size);
          pthread_mutex_unlock(&(sc->output_lock));
        } else {
          if (sc->err_clbk)
            sc->err_clbk(SerialCommErrBadData, sc);
        }
        pos = 0;
      }
    }

  }
  
  pthread_exit(NULL);
}
