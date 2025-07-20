import cv2
import numpy as np

from detect import detect_red_center
from serial import send_angle_cmd

cap = cv2.VideoCapture(0)
width, height = 640, 480
center_x, center_y = width // 2, height // 2
Kp = 0.1  # 控制系数
current_angle_x = 180  # 初始位置设为中间
current_angle_y = 180

while True:
    ret, frame = cap.read()
    if not ret:
        break

    target = detect_red_center(frame)
    if target:
        dx = target[0] - center_x
        dy = target[1] - center_y

        angle_x = current_angle_x + dx * Kp
        angle_y = current_angle_y + dy * Kp

        angle_x = max(0, min(360, angle_x))
        angle_y = max(0, min(360, angle_y))

        current_angle_x = angle_x
        current_angle_y = angle_y

        send_angle_cmd(angle_x, angle_y)

    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
