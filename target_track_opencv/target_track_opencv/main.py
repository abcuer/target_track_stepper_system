import cv2
import time
from detect import detect_red_center, normalize_angle
from serial_send import send_angle_cmd

width, height = 320, 240
center_x, center_y = width // 2, height // 2
current_angle_x = 0
current_angle_y = 0
FOV_H = 100.0                   # 水平视场角
factor = 0.5             
Kp = factor * FOV_H / width  # 实际比例系数

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        continue  # 如果读取失败就跳过

    target, bbox = detect_red_center(frame)
    if target and bbox:
        dx = target[0] - center_x
        dy = target[1] - center_y

        angle_x = current_angle_x + dx * Kp
        angle_y = current_angle_y + dy * Kp

        # 限幅
        angle_x = normalize_angle(angle_x)
        angle_y = normalize_angle(angle_y)

        current_angle_x = angle_x
        current_angle_y = angle_y

        send_angle_cmd(angle_x, angle_y)

    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
