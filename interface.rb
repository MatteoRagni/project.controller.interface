#!/usr/bin/env ruby

# Copyright (c) 2018, Matteo Ragni
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#    This product includes software developed by Matteo Ragni.
# 4. Neither the name of Matteo Ragni nor the
#    names of its contributors may be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# In this example we put the system in the minimum informations for running a cycle
# The code generates the output as in the image of the readme, when the SIL_SIM is
# defined in firmware.

require "./libserialcomm_ruby.rb"

sc = SerialComm.new "/dev/ttyACM0"

sc.auto_pres
sc.auto_temp

sc.p_high = 31.5
sc.p_low = 26.0

sc.PI_ki = 1.0
sc.PI_kp = 10.0
sc.t_set = 70.0
sc.period = 1.0
sc.cycle = 331630.0

sc.play

begin
  while (1)
    system("clear")
    
    sc.update
    puts " STATUS   : #{sc.state}"
    puts " ERROR    : #{sc.error_string}"
    puts " CYCLES   : #{sc.cycle.to_i} of #{sc.max_cycle.to_i} (#{'% 3.1%' % (sc.cycle/sc.max_cycle * 100.0) }%)"
    puts
    puts " SENSORS :"
    puts "   - Pressure Actuator    : #{'% 3.2f' % sc.p_actuator_meas} bar"
    puts "   - Pressure Accumulator : #{'% 3.2f' % sc.p_accumulator_meas} bar"
    puts "   - Temperature          : #{'% 3.2f' % sc.t_meas} degrees celsius"
    puts
    puts " ACTUATION                : P is #{sc.actuator ? "on" : "off"} :: R is #{sc.resistance ? "on" : "off"} :: C is #{sc.chiller ? "on" : "off"}"
    puts "   - Reference            : #{'% 2.2f' % sc.period} sec at #{'% 3.0f' % (sc.duty_cycle * 100)}%"
    puts "   - Pressure set         : #{'% 3.2f' % sc.p_set} bar"
    puts "   - Temperature Set      :#{'% 3.2f' % sc.t_set} degrees celsius"
    puts
    puts "   - Controller set       : #{'% 3.2f' % sc.u_pres} of 255.0 units"
    puts "   - PI gains             : KP = #{'% 3.2f' % sc.PI_kp}, KI = #{'% 3.2f' % sc.PI_ki}"

    sleep 0.1 
  end
ensure
  sc.close
end
