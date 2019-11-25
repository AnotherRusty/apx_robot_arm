# Openarm firmware

针对机械臂 Vcitor 和 Neo

# Configuration/Settings

参数配置位于 platformio.ini

motor_pins  舵机驱动IO

    -D JOINT0_PIN=8     舵机0
    -D JOINT1_PIN=9     舵机1
    -D JOINT2_PIN=10    舵机2
    -D JOINT3_PIN=11    舵机3
    -D JOINT4_PIN=12    舵机4

ps2_params  PS2手柄相关参数

    -D PRESSURES=0      暂不支持
    -D RUMBLE=0         暂不支持
    -D PS2_DAT=4        DAT
    -D PS2_CMD=5        CMD
    -D PS2_SEL=6        SLECT
    -D PS2_CLK=7        CLK

其他

    -D ROBOT_NAME='"Victor5"' ; "Neo5"          机械臂名称
    -D BAUDRATE=9600                            波特率
    -D SERVO_SPEED=0.5  ;do not exceed 1.0      舵机转速（不要设置超过1.0）
    -D NUM_JOINTS=5                             关节数
    -D DEBUG_ON=0                               调试输出开关
    -D USE_PS2=0                                PS2手柄控制开关


# 编译上传

针对不同类型arduino板，需要设置编译环境变量，编译上传前修改platformio.ini文件。

UNO：
```
[env:uno]
platform = atmelavr
board = uno
framework = arduino
build_flags = ${robot_configurations.build_flags}
lib_deps = ${robot_configurations.lib_deps_builtin}
```

Mega2560:
```
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
framework = arduino
build_flags = ${robot_configurations.build_flags}
lib_deps = ${robot_configurations.lib_deps_builtin}
```