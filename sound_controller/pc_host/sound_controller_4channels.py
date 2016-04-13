import time
import warnings
import serial
import os

arduino = serial.Serial('COM3', baudrate=115200, timeout=1.0)
arduino.setDTR(False)
time.sleep(1)
arduino.flushInput()
arduino.setDTR(True)

#get volume from Channels

#channel_program = ["Google Chrome","Internet Explorer","TeamSpeak 3 Client","Origin","Telegram Desktop"]
channel_program = ["Google Chrome","Internet Explorer","VLC media player","{0.0.0.00000000}.{606228fb-75ee-4290-8147-df477b420cb5}|#%b{F72273B5-40B8-426B-9D86-2DB0D45E87C9}|1%b#"]

with arduino:
    while True:
        try:
            text = arduino.readline()
            if not text == "":
            	#print(line)
            	#os.system("SoundVolumeView.exe /Mute \"chrome.exe\"")
                #text example recved by SPP: text = "ch_volume:25,36,45,56"
                text = text.split(":")
                if text[0] == "ch_volume":
                    """"
                    print("Sound: Cascos")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{0f5e5f19-525f-44d8-81b2-d978b5c28ccd}\" 1")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{0f5e5f19-525f-44d8-81b2-d978b5c28ccd}\" 2")
                elif line == "B\r\n":
                    print("Sound: Altavoces")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{fee286a0-8402-4859-99ca-9f8d785ffbd3}\" 1")
                    os.system("SoundVolumeView.exe /SetDefault \"{0.0.0.00000000}.{fee286a0-8402-4859-99ca-9f8d785ffbd3}\" 2")
                    #os.system("SHUTDOWN /S /T 10 ")
                    """
                    ch_volumes = list(text[1].split(","))
                    print ch_volumes


                    for i in range(3):
                        os.system("SoundVolumeView.exe /SetVolume " + "\"" +channel_program[i] + "\"" + " " + ch_volumes[i])


                continue
        except ValueError:
            warnings.warn("text {} didn't parse, skipping".format(text))
            print(text)
        except KeyboardInterrupt:
            print("Exiting")
            break
