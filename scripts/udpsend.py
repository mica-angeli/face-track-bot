#!/usr/bin/env python3

import socket

# UDP_IP = "127.0.0.1"
UDP_IP = "192.168.1.107"
UDP_PORT = 5555

print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
# print("message: %s" % MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

msg = bytes()
msg += b'm'
msg += int(55).to_bytes(4, 'little', signed=True)
msg += int(-1001).to_bytes(4, 'little', signed=True)
msg += int(98).to_bytes(4, 'little', signed=True)
sock.sendto(msg, (UDP_IP, UDP_PORT))
