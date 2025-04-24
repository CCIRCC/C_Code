import sensor, image, time, math

# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)  # 必须为 RGB565
sensor.set_framesize(sensor.QQVGA)    # 分辨率建议 QQVGA (160x120) 以提高速度
sensor.skip_frames(time=2000)         # 等待摄像头稳定
sensor.set_auto_gain(False)           # 关闭自动增益（避免影响检测）
sensor.set_auto_whitebal(False)       # 关闭自动白平衡

# AprilTag 检测参数
TAG_FAMILY = image.TAG36H11  # 指定标签家族为 TAG36H11

while True:
    img = sensor.snapshot()  # 捕获一帧图像

    # 检测所有 TAG36H11 标签
    tags = img.find_apriltags(families=TAG_FAMILY)

    # 遍历检测到的标签
    for tag in tags:
        # 绘制标签轮廓和中心点
        img.draw_rectangle(tag.rect(), color=(255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color=(0, 255, 0))

        # 获取标签信息
        tag_id = tag.id()                # 标签 ID
        rotation = tag.rotation()         # 旋转角度（弧度）
        x_translation = tag.x_translation()  # 三维空间中的 X 坐标（需提前标定）
        y_translation = tag.y_translation()  # Y 坐标
        z_translation = tag.z_translation()  # Z 坐标（距离）

        # 打印结果
        print(f"Tag ID: {tag_id}")
        print(f"Rotation: {math.degrees(rotation):.1f}°")
        print(f"Position (x, y, z): ({x_translation:.2f}, {y_translation:.2f}, {z_translation:.2f})")
        print("------------------")
