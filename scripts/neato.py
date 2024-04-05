#!/usr/bin/env python3

from serial import Serial
from time import sleep
import socket

if __name__ == '__main__':
    debug = True
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # ip_addr = socket.gethostbyname(socket.gethostname())
    ip_addr = '192.168.1.107'
    sock.bind((ip_addr, 5555))
    
    ser = Serial('/dev/ttyACM0')
    
    sleep(2)
    for _ in range(3):
        cmd = 'TestMode On'
        print(cmd)
        ser.write(bytes(cmd + '\n', 'ascii'))
        sleep(5)
    
    try:
        while True:
            data, addr = sock.recvfrom(1024)
            str_data = data.decode('ascii')
            # print(f'received message: {data}')
            l_wheel, r_wheel, speed = str_data.split(',')
            l_wheel = int(l_wheel)
            r_wheel = int(r_wheel)
            speed = int(speed)
            l_wheel = -l_wheel
            r_wheel = -r_wheel
            cmd = f'SetMotor LWheelDist {l_wheel} RWheelDist {r_wheel} Speed {speed}'
            print(cmd)
            ser.write(bytes(cmd + '\n', 'ascii'))
    except KeyboardInterrupt:
        pass
