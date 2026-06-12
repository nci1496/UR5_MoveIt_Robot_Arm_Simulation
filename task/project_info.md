# 项目信息

## 项目名称

基于ROS Noetic与MoveIt的UR5机械臂轨迹规划与控制仿真系统

## 项目性质

机器人学实验大作业

## 开发环境

- Ubuntu 20.04
- ROS Noetic
- MoveIt
- RViz
- C++

## 机械臂型号

UR5

## 项目目标

完成机械臂点位控制与轨迹控制。满足课程要求：

1. 六个空间点位运动
2. 椭圆轨迹运动
3. 8字轨迹运动
4. 其他创新功能

## 当前进度

基础功能已完成：

- MoveIt环境搭建
- UR5加载
- 点位控制
- 六点控制
- 当前位姿获取
- 笛卡尔直线轨迹
- 椭圆轨迹
- 8字轨迹
- Marker轨迹显示

## 运行指令

### 1. 启动机械臂仿真环境（必须先执行）

```bash
roslaunch ur5_moveit_config demo.launch
```

### 2. 运行测试节点

```bash
cd ~/robot_arm_hw
source devel/setup.bash

# 点位移动测试
rosrun arm_control_demo move_to_point

# 六点轨迹测试
rosrun arm_control_demo six_points

# 工作空间验证
rosrun arm_control_demo workspace_test

# 笛卡尔直线
rosrun arm_control_demo cartesian_line

# 椭圆轨迹
rosrun arm_control_demo ellipse_path

# 8字轨迹
rosrun arm_control_demo eight_path
```

### 3. 虚拟画板GUI

```bash
# 终端1：GUI（发布轨迹）
rosrun virtual_canvas_gui virtual_canvas_gui

# 终端2：执行器（订阅轨迹并控制机械臂）
rosrun canvas_executor canvas_executor
```

## 后续方向

以"机械臂书写系统"为主线继续扩展。
