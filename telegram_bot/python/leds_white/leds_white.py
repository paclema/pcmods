import serial, time
arduino = serial.Serial('COM5', 19200, timeout=.1)
time.sleep(2) #give the connection a second to settle
arduino.write("STRIP_COLOR:255,255,255;\r\n")