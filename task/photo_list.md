# 图片清单说明文档（精简版）

本文档说明课程报告中需要的核心截图，约15张左右。

---

## 图片存放目录

```
robot_arm_hw/task/report_images/
```

---

## 必须截图（12张）

### 第2章 环境配置

| 图片编号 | 文件名 | 内容要求 | 说明 |
|----------|--------|----------|------|
| 图2-1 | `fig2-1-rviz-ur5.png` | RViz中UR5机械臂仿真界面 | **必须** - 证明仿真环境正常 |

**验证命令**（报告中写明即可，截图可选）：
```bash
rosversion -d
roslaunch ur5_moveit_config demo.launch
```

---

### 第3章 轨迹规划（5张）

| 图片编号 | 文件名 | 内容要求 | 说明 |
|----------|--------|----------|------|
| 图3-1 | `fig3-1-six-points.png` | 六点控制执行过程截图 | **必须** - 展示RViz中机械臂运动 |
| 图3-2 | `fig3-2-cartesian.png` | 笛卡尔直线轨迹执行截图 | **必须** - 展示直线运动 |
| 图3-3 | `fig3-3-ellipse.png` | 椭圆轨迹Marker可视化 | **必须** - 红色曲线+机械臂 |
| 图3-4 | `fig3-4-ellipse-motion.png` | 椭圆轨迹执行过程 | **必须** - 机械臂绘制椭圆 |
| 图3-5 | `fig3-5-eight.png` | 8字轨迹Marker可视化 | **必须** - 展示8字形状 |

**运行命令**：
```bash
rosrun arm_control_demo six_points
rosrun arm_control_demo cartesian_line
rosrun arm_control_demo ellipse_path
rosrun arm_control_demo eight_path
```

---

### 第4章 虚拟画板（4张）

| 图片编号 | 文件名 | 内容要求 | 说明 |
|----------|--------|----------|------|
| 图4-1 | `fig4-1-qt-gui.png` | Qt GUI主界面截图 | **必须** - 展示画板和控件 |
| 图4-2 | `fig4-2-drawing.png` | GUI中绘制图形的界面 | **必须** - 画板有内容 |
| 图4-3 | `fig4-3-rviz-execution.png` | RViz中执行画板轨迹 | **必须** - 机械臂复现绘制 |
| 图4-4 | `fig4-4-complete.png` | 完成效果（GUI+RViz双视图） | **必须** - 完整书写效果 |

**运行命令**：
```bash
# 终端1
roslaunch ur5_moveit_config demo.launch
# 终端2
rosrun virtual_canvas_gui virtual_canvas_gui
# 终端3
rosrun canvas_executor canvas_executor
```

---

### 第5章 仿真效果（2张）

| 图片编号 | 文件名 | 内容要求 | 说明 |
|----------|--------|----------|------|
| 图5-1 | `fig5-1-trajectory-overview.png` | 多种轨迹规划效果汇总 | **必须** - Marker显示各类轨迹 |
| 图5-2 | `fig5-2-canvas-writing.png` | 画板书写效果 | **必须** - 最终书写效果 |

---

## 可选截图（3张）

如果时间充裕可以补充：

| 图片编号 | 文件名 | 内容要求 |
|----------|--------|----------|
| 图6-1 | `fig6-1-directory.png` | 项目目录结构 |
| 图6-2 | `fig6-2-ros-topics.png` | `rostopic list` 输出 |
| 图6-3 | `fig6-3-velocity-control.png` | 速度控制滑块演示 |

---

## 快速截图指南

### 截图顺序建议（按运行顺序）

1. **先跑程序再截图** - 确保效果正常再截
2. **每运行一个程序截一张** - 趁热打铁
3. **GUI和RViz各截一张** - 展示对比效果

### 截图技巧

- RViz截图：调整好视角后直接截图工具
- GUI截图：确保界面清晰，控件可见
- 可以用 `gnome-screenshot -a` 区域截图

---

## 图片命名规范

```
fig{章节}-{序号}-{描述}.png
```

**示例**：`fig3-3-ellipse.png`、`fig4-1-qt-gui.png`

---

## 报告中的引用

在Word中插入图片后，设置：
- 题注格式：`图X-X`
- 宽度：不超过15cm

---

**精简版 | 核心截图12张**
**创建时间**：2026年6月12日
