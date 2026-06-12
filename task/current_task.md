# 当前任务卡片

## 任务名称

Phase 6.2 - Qt GUI 与机械臂通信集成

## 所属阶段

Phase 6 - 虚拟画板系统

## 当前背景

Phase 6.1 已完成 Qt GUI 画板界面，可自由绘图。Phase 6.2 需要将 GUI 与机械臂控制集成，实现绘制后点击执行，机械臂跟随绘制轨迹运动。

**通信架构：**
```
[Qt GUI] --发布--> [/canvas_trajectory] --订阅--> [canvas_executor] --控制--> [UR5]
```

**工作空间定义：**
- 固定平面：y = 0.55
- X范围：-0.4 到 0.3
- Z范围：0.2 到 0.55

## 当前任务目标

1. Qt GUI 点击"执行"按钮后，发布轨迹到 ROS Topic
2. 创建 canvas_executor 节点订阅轨迹并控制机械臂执行
3. 在 RViz 中显示轨迹预览

## 已完成工作

- [x] Qt GUI 添加 ROS publisher 功能
- [x] 创建 canvas_executor 包
- [x] 实现轨迹订阅与机械臂控制
- [x] 发布 Marker 可视化

## 新增包说明

### virtual_canvas_gui
发布 `/canvas_trajectory` (geometry_msgs/PoseArray)

### canvas_executor
订阅 `/canvas_trajectory`，控制机械臂执行

## 验收标准

1. 点击"执行"按钮后，终端显示 "Trajectory published to /canvas_trajectory"
2. canvas_executor 节点运行后自动执行接收到的轨迹
3. 机械臂在 RViz 中沿轨迹运动
4. Marker 显示红色轨迹预览

## 后续接力提示

- [ ] 测试完整流程
- [ ] 添加轨迹平滑算法
- [ ] 添加速度控制
