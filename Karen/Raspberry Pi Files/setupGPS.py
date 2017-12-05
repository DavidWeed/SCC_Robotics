from subprocess import call


call('sudo gpsd /dev/ttyACM0 -F /var/run/gpsd.sock', shell=True)
