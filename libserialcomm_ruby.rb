#!/usr/bin/env ruby

if not require 'ffi'
  raise RuntimeError, "Please install ffi: sudo apt install ruby-ffi"
end

module SerialCommInterface
  extend FFI::Library
  ffi_lib "./libserialcomm.so"

  attach_function :serialcomm_initialize, [:string], :pointer
  attach_function :serialcomm_destroy, [:pointer], :void
  attach_function :serialcomm_check_errors, [], :int
  attach_function :serialcomm_update, [:pointer], :void
  
  [
    :serialcomm_get_t_meas,
    :serialcomm_get_p_meas,
    :serialcomm_get_q_meas,
    :serialcomm_get_ki,
    :serialcomm_get_kp,
    :serialcomm_get_t_set,
    :serialcomm_get_p_set,
    :serialcomm_get_u_pres,
    :serialcomm_get_period,
    :serialcomm_get_duty_cycle,
    :serialcomm_get_cycle,
    :serialcomm_get_cycle_max
  ].each do |f|
    attach_function f, [:pointer], :float
  end

  [
    :serialcomm_get_actuator_config,
    :serialcomm_get_chiller_config,
    :serialcomm_get_resistance_config,
    :serialcomm_get_state,
    :serialcomm_get_error
  ].each do |f|
    attach_function f, [:pointer], :int
  end

  [
    :serialcomm_set_temp,
    :serialcomm_set_p_high,
    :serialcomm_set_p_low,
    :serialcomm_set_ki,
    :serialcomm_set_kp,
    :serialcomm_set_pres,
    :serialcomm_set_u_pres,
    :serialcomm_set_period,
    :serialcomm_set_duty_cycle,
    :serialcomm_set_cycle,
    :serialcomm_set_cycle_max
  ].each do |f|
    attach_function f, [:pointer, :float], :void
  end

  [
    :serialcomm_set_temperature_manual,
    :serialcomm_set_pressure_manual,
    :serialcomm_set_temperature_auto,
    :serialcomm_set_pressure_auto,
    :serialcomm_toggle_chiller,
    :serialcomm_toggle_resistance,
    :serialcomm_toggle_pause,
    :serialcomm_emergency_stop,
    :serialcomm_system_reboot,
    :serialcomm_save_config,
    :serialcomm_load_config,
    :serialcomm_restore_cycle,
    :serialcomm_start_cycle
  ].each do |f|
    attach_function f, [:pointer], :void
  end
end

class SerialComm
  include SerialCommInterface
  include ObjectSpace

  def initialize(port)
    raise ArgumentError, "port must be a string" unless port.is_a? String
    raise ArgumentError, "Serial connection #{port} does not exist" unless File.exist? port

    @port = port
    @sc = serialcomm_initialize(port)
    if serialcomm_check_errors() != 0
     raise RuntimeError, "Connection error for SerialComm class"
    end
  end

  def update
    serialcomm_update(@sc)
  end

  def t_meas
    serialcomm_get_t_meas(@sc)
  end

  def p_actuator_meas
    serialcomm_get_p_meas(@sc)
  end

  def p_accumulator_meas
    serialcomm_get_q_meas(@sc)
  end

  def PI_ki
    serialcomm_get_ki(@sc)
  end

  def PI_kp
    serialcomm_get_kp(@sc)
  end
  
  def t_set
    serialcomm_get_t_set(@sc)
  end

  def p_set
    serialcomm_get_p_set(@sc)
  end

  def u_pres
    serialcomm_get_u_pres(@sc)
  end

  def period
    serialcomm_get_period(@sc)
  end

  def duty_cycle
    serialcomm_get_duty_cycle(@sc)
  end

  def cycle
    serialcomm_get_cycle(@sc)
  end

  def cycle_max
    serialcomm_get_cycle_max(@sc)
  end

  def chiller
    (serialcomm_get_chiller_config(@sc) > 0)
  end

  def actuator
    (serialcomm_get_actuator_config(@sc) > 0)
  end

  def resistance
    (serialcomm_get_resistance_config(@sc) > 0)
  end

  def state
    st = {
      1 => :alarm, 
      2 => :pause, 
      4 => :running, 
      8 => :waiting, 
      10 => :serial_setup
    }
    st[serialcomm_get_state(@sc)]
  end
  
  def error
    err = [
      :no_error,
      :temp_sensor_fault,
      :pres_act_sensor_fault,
      :pres_acc_sensor_fault,
      :serial_check,
      :emergency_button,
      :temp_control_emergency,
      :pres_control_emergency,
      :pres_accumulator_emergency,
      :cycle_complete,
      :serial_stop
    ]
    err[serialcomm_get_error(@sc)]
  end

  def error_string
    err = {
      :no_error => "No Error",
      :temp_sensor_fault => "Temperature sensor fault reading",
      :pres_act_sensor_fault => "Pressure Actuator sensor fault reading",
      :pres_acc_sensor_fault => "Pressure Accumulator sensor fault reading",
      :serial_check => "Serial Check Error, communication went wrong",
      :emergency_button => "Emergency contact pressed",
      :temp_control_emergency => "Temperature controller error - System deviates from reference",
      :pres_control_emergency =>  "Pressure controller error - System deviates from reference",
      :pres_accumulator_emergency => "Pressure Accumulator Error - Accumulator deviates from circuit",
      :cycle_complete => "All cycles have been completed",
      :serial_stop => "Stop received via serial"
    }
    err[error()]
  end

  def t_set=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_temp(@sc, f)
  end

  def p_high=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_p_high(@sc, f)
  end

  def p_low=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_p_low(@sc, f)
  end

  def PI_ki=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_ki(@sc, f)
  end

  def PI_kp=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_kp(@sc, f)
  end

  def p_set=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_pres(@sc, f)
  end

  def period=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_period(@sc, f)
  end

  def duty_cycle=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_duty_cycle(@sc, f)
  end

  def cycle=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_temp(@sc, f)
  end

  def cycle_max=(f)
    f = f.to_f if f.is_a? Fixnum
    raise ArgumentError, "A float value is required" unless f.is_a? Float
    serialcomm_set_cycle_max(@sc, f)
  end

  def auto_temp
    serialcomm_set_temperature_auto(@sc)
  end

  def auto_pres
    serialcomm_set_pressure_auto(@sc)
  end

  def manual_temp
    serialcomm_set_temperature_manual(@sc)
  end

  def manual_pres
    serialcomm_set_pressure_manual(@sc)
  end

  def toggle_chiller
    serialcomm_toggle_chiller(@sc)
  end

  def toggle_resistance
    serialcomm_toggle_resistance(@sc)
  end

  def pause
    serialcomm_toggle_pause(@sc)
  end

  def stop
    serialcomm_emergency_stop(@sc)
  end

  def reboot
    serialcomm_system_reboot(@sc)
  end

  def save_conf
    serialcomm_save_config(@sc)
  end

  def load_conf
    serialcomm_load_conf(@sc)
  end

  def load_cycle
    serialcomm_restore_cycle(@sc)
  end

  def play
    serialcomm_start_cycle(@sc)
  end

  def close
    serialcomm_destroy(@sc)
  end

  def SerialComm.finalize(id)
    obj = ObjectSpace._id2ref(id)
    obj.close
  end
end

