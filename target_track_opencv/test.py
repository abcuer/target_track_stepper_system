import cv2
import numpy as np
import time

from detect import detect_red_center
from serial_send import send_angle_cmd

cap = cv2.VideoCapture(0)
width, height = 320, 240
center_x, center_y = width // 2, height // 2
Kp = 0.1
current_angle_x = 0
current_angle_y = 0

frame_count = 0
start_time = time.time()
fps = 0.0  # 用于显示

while True:
    ret, frame = cap.read()
    if not ret:
        break

    target, bbox = detect_red_center(frame)
    if target and bbox:
        dx = target[0] - center_x
        dy = target[1] - center_y

        angle_x = current_angle_x + dx * Kp
        angle_y = current_angle_y + dy * Kp

        angle_x = max(0, min(360, angle_x))
        angle_y = max(0, min(360, angle_y))

        current_angle_x = angle_x
        current_angle_y = angle_y

        print(f"x:{angle_x:.2f}, y:{angle_y:.2f}")

        # 画矩形框
        # x, y, w, h = bbox
        # cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        # 画中心点
        cv2.circle(frame, target, 5, (0, 0, 255), -1)
         # 在圆点旁边标注坐标（文字位置稍微偏右上）
        cv2.putText(frame, f"({target[0]}, {target[1]})", 
                    (target[0] + 10, target[1] - 10), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

    # 计算FPS
    frame_count += 1
    elapsed_time = time.time() - start_time
    if elapsed_time >= 1.0:
        fps = frame_count / elapsed_time
        frame_count = 0
        start_time = time.time()

    # 在左上角绘制FPS
    cv2.putText(frame, f"FPS: {fps:.2f}", (10, 30), 
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)

    cv2.imshow("Camera", frame)
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
