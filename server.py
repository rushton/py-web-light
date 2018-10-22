from serial.tools.list_ports import comports
import serial
import struct
import time
from flask import Flask
api = Flask(__name__)

usb = serial.serial_for_url([p.device for p in comports() if 'usbserial' in p.device][0])
usb.timeout = .2

@api.route("/<int:address>/<int:red>/<int:green>/<int:blue>")
def set_led(address, red, green, blue):
    set_led(int(address), int(red), int(green), int(blue))
    return "done!"

def set_led(address, red, green, blue):
    ackd = False
    # try writing until the microcontroller sends an ACK
    # message back on the serial line
    while not ackd:

        #message format: <led_address:2-byte-int><red:2-byte-int><green:2-byte-int><blue:2-byte-int><intensity:2-byte-int>
        msg = struct.pack('4H', address, red, green ,blue)
        usb.write(msg)
        ack_msg = usb.readline()
        if b'ACK' in ack_msg:
            ackd = True

if __name__ == '__main__':
    api.run()
