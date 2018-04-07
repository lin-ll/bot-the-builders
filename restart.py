import os

pid = os.popen("ps -aux | grep pigpiod").read()
kill = pid.split()[1]
print("Killing process: %s" % kill)
os.popen("sudo kill " + kill)
os.popen("sudo pigpiod")

pid = os.popen("ps -aux | grep pigpiod").read()
kill = pid.split()[1]
print("New process: %s" % kill) 
