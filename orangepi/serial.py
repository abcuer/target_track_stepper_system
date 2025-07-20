import serial

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.5)

def send_angle_cmd(angle_x, angle_y):
    if not ser.is_open:
        ser.open()

    if ser.is_open:
        cmd = 0x01
        data = bytearray([int(angle_x), int(angle_y)])
        length = len(data)
        checksum = cmd ^ length
        for b in data:
            checksum ^= b
        frame = bytearray([0xAA, 0x55, cmd, length]) + data + bytearray([checksum, 0x0D, 0x0A])
        ser.write(frame)
