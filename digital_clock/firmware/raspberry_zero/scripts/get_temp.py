import serial
ser = serial.Serial ("/dev/ttyAMA0")
ser.baudrate = 9600
cmd = "GET_TEMP"
print(cmd);
ser.write(bytes(cmd, 'utf-8'))
ser.close()
