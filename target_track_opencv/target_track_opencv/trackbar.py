import cv2
import numpy as np

def nothing(x):
    pass

# 创建窗口
cv2.namedWindow('Trackbars')

# 创建6个滑动条，范围均设定为HSV常用范围
cv2.createTrackbar('H_low', 'Trackbars', 0, 179, nothing)
cv2.createTrackbar('H_high', 'Trackbars', 10, 179, nothing)
cv2.createTrackbar('S_low', 'Trackbars', 100, 255, nothing)
cv2.createTrackbar('S_high', 'Trackbars', 255, 255, nothing)
cv2.createTrackbar('V_low', 'Trackbars', 100, 255, nothing)
cv2.createTrackbar('V_high', 'Trackbars', 255, 255, nothing)

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 读取滑动条的阈值
    h_low = cv2.getTrackbarPos('H_low', 'Trackbars')
    h_high = cv2.getTrackbarPos('H_high', 'Trackbars')
    s_low = cv2.getTrackbarPos('S_low', 'Trackbars')
    s_high = cv2.getTrackbarPos('S_high', 'Trackbars')
    v_low = cv2.getTrackbarPos('V_low', 'Trackbars')
    v_high = cv2.getTrackbarPos('V_high', 'Trackbars')

    # 制作掩码
    lower_red1 = np.array([h_low, s_low, v_low])
    upper_red1 = np.array([h_high, s_high, v_high])

    # 注意红色在HSV中有两个区间，需要分别设置并合并
    # 这里演示只用一个区间，实际需要你调整两个区间的滑条或增加代码
    mask = cv2.inRange(hsv, lower_red1, upper_red1)

    # 膨胀腐蚀处理减少噪点（可选）
    kernel = np.ones((5,5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

    # 显示掩码窗口
    cv2.imshow('Mask', mask)

    # 叠加显示检测效果
    result = cv2.bitwise_and(frame, frame, mask=mask)
    cv2.imshow('Result', result)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
