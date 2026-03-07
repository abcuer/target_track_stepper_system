import cv2
import numpy as np
from detect import detect_red_center

cap = cv2.VideoCapture(0)
# 设置分辨率
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 720)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

print("正在展示正常色域识别效果，按 'q' 退出...")

while True:
    ret, frame = cap.read()
    if not ret:
        breakq

    # 使用的逻辑寻找红色中心
    target, bbox = detect_red_center(frame)

    if target and bbox:
        # 在彩色原图上画出绿色的识别框
        x, y, w, h = bbox
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        
        # 在中心位置画一个红色的实心小圆点
        cv2.circle(frame, target, 5, (0, 0, 255), -1)
        
        # 在彩色画面上实时显示坐标
        cv2.putText(frame, f"Fire: {target}", (10, 20), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
    else:
        cv2.putText(frame, "Target Lost", (10, 20), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)

    # 展示彩色结果图
    cv2.imshow("Red Target Tracking", frame)

    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()