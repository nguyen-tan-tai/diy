from datetime import datetime
import serial
ser = serial.Serial ("/dev/ttyAMA0")
ser.baudrate = 9600
cmd = "SET_TIME " +  datetime.now().isoformat()
print(cmd);
ser.write(bytes(cmd, 'utf-8'))
ser.close()
