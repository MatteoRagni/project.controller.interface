require './libserialcomm_ruby.rb'

sc = SerialComm.new "/dev/ttyACM0"

sc.update
sleep(1)
print "Initial KI = "
puts sc.PI_ki

sc.PI_ki = 5.0
sc.update
sleep(1)
print "Modified KI = "
puts sc.PI_ki

sc.close
sleep(1)