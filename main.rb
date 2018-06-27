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

sc.p_high = 25.0
sc.p_low = 5.0

sc.PI_ki = 0.3
sc.PI_kp = 6.0
sc.t_set = 40.0
sc.period = 6.0
sc.duty_cycle = 0.5

sc.play


begin
  while (1)
    sc.update
    puts "#{sc.cycle}, #{sc.p_actuator_meas}, #{sc.p_accumulator_meas}, #{sc.t_meas}"
    sleep 0.1 
  end
ensure
  sc.close
end