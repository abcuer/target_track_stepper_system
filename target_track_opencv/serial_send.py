import serial
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.5)

def send_angle_cmd(angle_x, angle_y):
    if not ser.is_open:
        ser.open()

    # 将 [-180, 180] 映射到 [0, 255]
    angle_x_byte = max(0, min(255, int((angle_x + 180) / 360 * 255)))
    angle_y_byte = max(0, min(255, int((angle_y + 180) / 360 * 255)))

    cmd = 0x01
    data = bytearray([angle_x_byte, angle_y_byte])
    length = len(data)

    checksum = cmd ^ length
    for b in data:
        checksum ^= b

    frame = bytearray([0xAA, 0x55, cmd, length]) + data + bytearray([checksum, 0x0D, 0x0A])
    ser.write(frame)
    print(f"[串口发送] {list(frame)}")
