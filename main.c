#include <stdio.h>
#include "libserialcomm_interface.h"
#include "messages.h"


void print_structure(void * sc) {
  printf("STRUCTURE\n");
  printf("     t_meas = %3.3f\n", serialcomm_get_t_meas(sc));
  printf("     p_meas = %3.3f\n", serialcomm_get_p_meas(sc));
  printf("     q_meas = %3.3f\n", serialcomm_get_q_meas(sc));
  printf("         kp = %3.3f\n", serialcomm_get_kp(sc));
  printf("         ki = %3.3f\n", serialcomm_get_ki(sc));
  printf("      t_set = %3.3f\n", serialcomm_get_t_set(sc));
  printf("      p_set = %3.3f\n", serialcomm_get_p_set(sc));
  printf("     u_pres = %3.3f\n", serialcomm_get_u_pres(sc));
  printf("     period = %3.3f\n", serialcomm_get_period(sc));
  printf(" duty_cycle = %3.3f\n", serialcomm_get_duty_cycle(sc));
  printf("      cycle = %f\n", serialcomm_get_cycle(sc));
  printf("  max_cycle = %f\n", serialcomm_get_cycle_max(sc));
  printf("Config:\n");
  printf(" - Chiller is %s\n", (serialcomm_get_chiller_config(sc) ? "on" : "off"));
  printf(" - Resistance is %s\n", (serialcomm_get_actuator_config(sc) ? "on" : "off"));
  printf(" - Actuator is %s\n", (serialcomm_get_actuator_config(sc) ? "on" : "off"));
  printf("State: %s\n", serialcomm_get_state_string(sc));
  printf("Error: %s\n", serialcomm_get_error_string(sc));
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Run with 1 argument (port name)\n");
    return -1;
  }
  const char * serial_port = argv[1];

  void *sc;

  sc = serialcomm_initialize(serial_port);

  printf("Opened serial port\n"); 

  serialcomm_set_temp(sc, 45.7);
  serialcomm_set_pres(sc, 21.3);
  serialcomm_set_cycle_max(sc, 123456.0);
  serialcomm_set_cycle(sc, 987654.0);
  serialcomm_set_temperature_auto(sc);
  serialcomm_set_pressure_auto(sc);
  serialcomm_toggle_chiller(sc);
  
  serialcomm_update(sc);
  sleep(2);

  print_structure(sc);

  serialcomm_destroy(sc);
  return 0;
}
