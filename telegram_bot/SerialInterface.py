"""
    SerialInterface.py
    --------------------------------------
    Controls a servo by sending commands through a serial port
"""

__author__ = "def"
__licence__ = "GPLv3"

import serial

class SerialInterface:

    def __init__(self):
        self.serialPort = None


    def connect(self, port, baudrate):
        """
        Connect with the serial port at a certain baudrate
        """
        # Setup serial port
        self.serialPort = serial.Serial()
        self.serialPort.port = port
        self.serialPort.baudrate = baudrate

        # Open port
        self.serialPort.open()

        if not self.serialPort.isOpen():
            raise IOError("Port could not be opened!")



    def sendFlagWave(self, waves):
        """
        Sends the command to move the servo to waves
        """
        if 0 <= waves <= 180:
            command = "SERVO_WAVE:%d;\n" % waves
        else:
            raise Exception('Position value %d out of range [0, 180]'%waves)

        try:
            self.serialPort.write(command)
        except AttributeError, e:
            print 'Not connected: [' + str(e) + ']'

    def sendRainbow(self):
        """
        Sends the command turn lights rainbow on 
        """
        command = "RAINBOW:5;\n"

        try:
            self.serialPort.write(command)
        except AttributeError, e:
            print 'Not connected: [' + str(e) + ']'

    def sendStrip(self, r=0, g=0, b=0):
        """
        Sends the command of turn lights as STRIP_COLOR:255,255,255; 
        """
        command = "STRIP_COLOR:%d,%d,%d;\n" % (r,g,b)

        try:
            self.serialPort.write(command)
        except AttributeError, e:
            print 'Not connected: [' + str(e) + ']'

# If the script is run directly, this example is executed:
if __name__ == "__main__":
    import time as t

    interface = SerialInterface()

    try:
        interface.connect('COM5', 19200)
    except AttributeError, e:
        print 'Arduino conection refused'

    t.sleep(5)
    interface.sendFlagWave(4)

