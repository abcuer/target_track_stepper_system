import serial

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.5)

def send_angle_cmd(angle_x, angle_y):
    if not ser.is_open:
        ser.open()

    angle_x = max(0, min(360, int(angle_x)))
    angle_y = max(0, min(360, int(angle_y)))

    angle_x_byte = int(angle_x / 360 * 255)
    angle_y_byte = int(angle_y / 360 * 255)

    cmd = 0x01
    data = bytearray([angle_x_byte, angle_y_byte])
    length = len(data)

    checksum = cmd ^ length
    for b in data:
        checksum ^= b

    frame = bytearray([0xAA, 0x55, cmd, length]) + data + bytearray([checksum, 0x0D, 0x0A])
    ser.write(frame)
    print(f"[串口发送] {list(frame)}")
