# lion_bot

此Project为低耦合、归一化各兵种的STM32端底层代码。


## 总览

英雄。

### 架构

整个工程主要分为3个层级，由上至下分别为应用层application layer、核心层core layer、驱动层driver layer。这个分层在keil项目的文件树和实际代码本地路径上均要体现，_.c_和_.h_暂不分开放置。详细结构如下：

```

lion_bot
├── application
    ├── user                // main
    └── configureation      // 计划用于放配置文件，存在的必要待定
├── core
    ├── common              // 总体性的任务，如主状态机、监控等
    ├── communicate         // 通信类任务，如can通信、串口通信等
    └── control             // 控制类任务，如底盘控制、云台控制、射击控制
├── driver
    ├── can                 // can驱动配置，具体区分can1和can2
    ├── uart                // 串口驱动配置，具体区分串口号
    ├── timer               // 时钟配置，包括普通应用的时钟和PWM用途的时钟
    └── others              // 其他未特殊分类的驱动
├── library
    ├── lion                // 队内整理的库文件(不一定原创)
    ├── rm                  // RoboMaster官方的库文件
    └── st                  // 意法半导体提供的官方库文件
└── readme.md

```

### 文件

单个文件仅负责且完全负责一个任务，如driver/can中的_can1.c_只负责can1外设的驱动，包括实现其初始化和中断服务函数；更高层次的任务在接收或中断处理函数中调用_core/communicate/can\_task.c_完成。同理_can\_task.c_中不包含配置can1或can2驱动的任何代码，仅完成对can通信接收到的数据的进一步处理。

### 变量

同时相关变量直接在对应任务的文件中声明和定义，如底盘电机编码器的结构体chassis\_encoder在_can\_task.h_中声明，在_can\_task.c_中定义，如果变量在其他不止一个文件中需要使用，可以在任务的头文件中声明`extern`，如can\_task中的chasis\_encoder常在chassis\_task和timer(用于调试的printf)中使用，则在_can\_task.h_中声明chasis\_encoder为`extern`。

### include

除了library中的文件以外，一般情况下，_.c_文件只包含(include)自身的_.h_文件，如_can\_task.c_只包含_can\_task.h_，.h文件只包含main.h，其他必要的头文件直接分类写在_main.h_中。

### define

计划通过`#define`来分辨当前代码所属兵种，语法如下。

```c

# ifdef big_brother
   // 大娃的代码
# else
   // 不是大娃的代码
# endif

```

### 命名规范

1. 除常见的名词缩写以外，尽可能使用全拼
2. 多个单词之间以下划线连接，如chassis\_encoder、ecd\_value
3. 结构体使用`typedef`命名，以\_t结尾，如encoder\_t
4. 枚举类型使用`typedef`命名，以\_e结尾，如chassis\_mode\_e
5. 几乎所有的命名都用小写，宏定义(`#define`)除外。宏定义命名以描述对应任务的单个单词开头，要求大写，如CHASSIS\_WHEEL\_TOTAL\_NUM、RC\_FRAME\_LENGTH
6. 其他未尽描述按当前工程中的样式为准

### 其他要求

1. 坚决不允许使用`\t`缩进，所有缩进必须使用空格，通常而言使用4格空格代表一次缩进
2. 按工程现有样式在每个函数前添加注释
3. 驱动初始化的调用写在核心层对应的task的初始化函数中
4. 变量的初始化写在对应task的初始化函数中(使用`memset`函数)


## 应用层 application layer

### application/user

暂时还没有很好的分层想法，目前是将不好定层的文件放在了应用层，如main和stm32f4xx\_it，但实际上为了迎合大家目前的项目习惯，不考虑将各中断整理到_stm32f4xx\_it.c_中。

### application/configureation

考虑要将配置文件从工程各处中抽离出来，但是还没有很好的想法，也许将来可以用一个单独的_configureation.h_文件实现。

## 核心层 core layer

### core/common

*fsm_task*

由timer6驱动，1ms维护一次，负责全车状态机的状态切换。


*robot_status*

本来想用来维护上传到tx2的状态信息的，但是目前还没有好的想法，暂时闲置。


*supervisor_task*

监控任务，负责监控各通信任务是否异常，从而判断模块是否离线。

### core/communicate

*can_task*

负责与电调通信，处理电调回传的编码器信息，提供`set_chassis_current()`和`set_gimbal_current()`接口，后续其他需求可仿照修改添加。


*rc_task*

遥控器数据处理，将收到的信息按控制方式分装在`rc_ctrl`变量中待后续使用。


*js_task*

裁判系统数据处理，负责与裁判系统通讯，包括处理下行数据和上传自定义数据等。


*tx2_task*

负责与tx2通讯，目前还在犹豫要不要使用crc校验，所以还需要修改。


*imu_task*

负责与imu通讯，当前使用的为mpu6050传感器。

### core/control

*chassis_task*

负责底盘驱动相关的各类函数，如底盘初始化、麦克纳姆轮速度换算等。


*gimbal_task*

负责云台驱动相关的各类函数，如云台初始化等


*shoot_task*

负责射击相关的各类函数，包括摩擦轮电机的运行控制和拨弹轮电机的运行控制，结合枪口热量的策略进行设计。


## 驱动层 driver layer

### driver/can

*can1*  云台电机和底盘电机控制


*can2*  暂时闲置

### driver/usart

*usart1*  遥控器DBUS


*usart2*  裁判系统


*usart3*  pc/tx2


*usart6*  imu

### driver/timer

*pwm*  摩擦轮


*timer* tim6，1ms主时钟

### driver/others

不好归类的其他驱动
