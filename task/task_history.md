# SolarWing 任务历史与阶段路线图

## 1. 项目背景
- 项目名称：SolarWing
- 技术栈：Qt Widgets + C++
- 目标：实现太阳翼展开时变重力卸载控制的可视化编程与仿真系统
- 需求来源：`task/total_task.md`
- 协作约束：每次仅完成指定任务，验收完成即可，其余文件如无必要不修改

## 2. 当前项目状态
- 当前仓库仅包含最小 Qt Widgets 骨架：`main.cpp`、`mainwindow.h`、`mainwindow.cpp`、`mainwindow.ui`
- 尚未建立如下模块目录与文件：`dialogs/`、`simulation/`、`widgets/`、`data/`、`resources/`
- `task/current_task.md` 与本文件用于多轮 AI 协作接力维护


- [x] RViz显示

---

## Phase2 基础控制

- [x] 单点位控制
- [x] 六点位控制
- [x] 当前位姿获取

---

## Phase3 轨迹规划

- [x] 笛卡尔直线轨迹
- [x] 椭圆轨迹
- [x] 8字轨迹

---

## Phase4 可视化

- [x] Marker轨迹显示

---

## Phase5 书写系统

- [ ] 工作平面验证
- [ ] 字母L
- [ ] 字母N
- [ ] 字母K
- [ ] 字母U
- [ ] NKU连续书写

---

## Phase6 交互系统

- [ ] 自定义轨迹输入
- [ ] 自由绘图
- [ ] 绘图映射到机械臂轨迹


## 5. 单次协作规则
- 每一轮只做 `task/current_task.md` 中定义的一个子任务
- 如果当前任务未验收，不提前进入下一个阶段
- 除非当前任务确有需要，否则不修改无关文件
- 每次完成后，在本文件末尾追加“已完成记录”

## 6. 当前推荐起始任务
- 当前推荐执行：Phase 4，前4阶段已完成


## 7. 已完成记录
