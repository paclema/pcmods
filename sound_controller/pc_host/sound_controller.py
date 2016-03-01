import time
import warnings
import serial
import os

arduino = serial.Serial('COM4', baudrate=115200, timeout=1.0)
arduino.setDTR(False)
time.sleep(1)
arduino.flushInput()
arduino.setDTR(True)

with arduino:
    while True:
        try:
            line = arduino.readline()
            if not line == "":
            	#print(line)
            	#os.system("SoundVolumeView.exe /Mute \"chrome.exe\"")
                if line == "A\r\n":
                    print("Sound: Cascos")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{0f5e5f19-525f-44d8-81b2-d978b5c28ccd}\" 1")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{0f5e5f19-525f-44d8-81b2-d978b5c28ccd}\" 2")
                elif line == "B\r\n":
                    print("Sound: Altavoces")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{fee286a0-8402-4859-99ca-9f8d785ffbd3}\" 1")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{fee286a0-8402-4859-99ca-9f8d785ffbd3}\" 2")
                    #os.system("SHUTDOWN /S /T 10 ")

                continue
        except ValueError:
            warnings.warn("Line {} didn't parse, skipping".format(line))
            print(line)
        except KeyboardInterrupt:
            print("Exiting")
            break
