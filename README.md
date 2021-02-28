# -
stm32c8t6 + mmpu6050无刷四轴

IMU姿态解算移植于外部。

由于飞控仅采用单级pid，建议330机架及以下，重量不宜超过1KG。

关于接线：
A0-A3：接收机1-4通道。
B6-B9：电调x4。
B11,B10：mpu6050 SDA SCL。
PC13：开发板自带led，用逻辑分析一或示波器抓取频率，得到控制周期。

效果地址：https://www.bilibili.com/video/BV1Zr4y1A7Ex
