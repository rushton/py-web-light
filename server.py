from serial.tools.list_ports import comports
from serial import SerialException
import serial
import struct
import time
from flask import Flask
import logging

logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)

api = Flask(__name__)

usb = None

@api.route("/<int:address>/<int:red>/<int:green>/<int:blue>")
def set_led(address, red, green, blue):
    set_led(int(address), int(red), int(green), int(blue), 0)
    return "done!"

@api.route("/<int:address>/<int:red>/<int:green>/<int:blue>/<int:duration>")
def transition_led(address, red, green, blue, duration):
    set_led(int(address), int(red), int(green), int(blue), int(duration))
    return "done!"

def set_led(address, red, green, blue, duration):
    global usb
    ackd = False
    #message format: <led_address:2-byte-int><red:2-byte-int><green:2-byte-int><blue:2-byte-int><intensity:2-byte-int>
    msg = struct.pack('5H', address, red, green ,blue, duration)
    # try writing until the microcontroller sends an ACK
    # message back on the serial line
    while not ackd:
        try:
            usb.write(msg)
            ack_msg = usb.readline()
            if b'ACK' in ack_msg:
                ackd = True
        except SerialException:
            logger.info("Caught serial exception, reinitializing serial conneciton.")
            usb = get_serial_connection()

def get_serial_connection():
    usb = serial.serial_for_url([p.device for p in comports() if 'usbserial' in p.device][0])
    usb.timeout = .2
    return usb

if __name__ == '__main__':
    usb = get_serial_connection()
    api.run()
