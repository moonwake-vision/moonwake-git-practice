# moonwake_ros2_training

一个用于 ROS 2 Humble 学习与训练的 C++ Package，包含多个通信练习节点。本 Task 主要通过 `target_offset_publisher` 和 `steering_hint_node` 演示 ROS 2 Topic 通信、参数配置（YAML）和 Launch 启动。

## 目录

- `src/target_offset_publisher.cpp`：周期性发布目标偏移量；
- `src/steering_hint_node.cpp`：订阅目标偏移量，并发布方向提示；
- `config/training.yaml`：配置发布频率、偏移量幅度和方向判断阈值；
- `launch/training.launch.py`：加载 YAML 参数并启动两个节点，配置 Topic 重映射；
- `package.xml`：Package 元数据和依赖；
- `CMakeLists.txt`：构建与安装配置。

## 依赖

- Ubuntu 22.04；
- ROS 2 Humble；
- `ament_cmake`；
- `rclcpp`；
- `std_msgs`。


## 构建与运行

在全新的终端中执行以下命令：

```bash
source /opt/ros/humble/setup.bash
cd ~/moonwake_autonomy
colcon build --packages-select moonwake_ros2_training
source install/setup.bash
ros2 launch moonwake_ros2_training training.launch.py
```

Launch 文件会启动 `target_offset_publisher` 和 `steering_hint_node`，并加载 `config/training.yaml` 中的参数。

按 `Ctrl+C` 可以停止两个节点。

在另一个终端中执行以下命令，检查节点和 Topic：

```bash
source /opt/ros/humble/setup.bash
source ~/moonwake_autonomy/install/setup.bash
ros2 node list
ros2 topic list
```

预期可以找到以下节点：

- `/target_offset_publisher`
- `/steering_hint_node`

预期可以找到以下 Topic：

- `/training/target_offset`
- `/training/steering_hint`

## 参数配置

配置文件：`config/training.yaml`

| 节点 | 参数 | 默认值 | 作用 |
|---|---|---:|---|
| `target_offset_publisher` | `publish_hz` | `5.0` | 偏移量发布频率，单位 Hz |
| `target_offset_publisher` | `amplitude` | `0.8` | 偏移量序列的幅度 |
| `steering_hint_node` | `deadband` | `0.10` | 方向判断的死区阈值 |

发布节点按顺序循环发布以下偏移量：

`-A → -A/2 → 0 → A/2 → A`

其中 `A` 由 `amplitude` 参数决定。

## 方向判断规则

订阅节点根据偏移量 `offset` 和死区阈值 `deadband` 判断方向：

| 判断条件 | 输出提示 |
|---|---|
| `offset < -deadband` | `LEFT` |
| `-deadband <= offset <= deadband` | `STRAIGHT` |
| `offset > deadband` | `RIGHT` |

当偏移量恰好等于 `-deadband` 或 `deadband` 时，均输出 `STRAIGHT`。

## 查看参数与 Topic

查看节点参数：

```bash
ros2 param get /target_offset_publisher amplitude
ros2 param get /target_offset_publisher publish_hz
ros2 param get /steering_hint_node deadband
```

查看偏移量 Topic 的发布端、订阅端和 QoS：

```bash
ros2 topic info /training/target_offset --verbose
```

查看方向提示：

```bash
ros2 topic echo /training/steering_hint
```

检查节点之间的连接关系：

```bash
rqt_graph
```


## 使用范围与安全说明

本 Task 仅用于 ROS 2 学习与训练，演示 Topic 通信、YAML 参数配置、Launch 启动和方向提示逻辑。

`LEFT`、`STRAIGHT`、`RIGHT` 仅为训练用方向提示，不是实际控制指令。本程序不连接真实传感器、执行器或自动驾驶控制系统。

## Git 提交流程

在工作区中进入本 Package 目录：

```bash
cd ~/moonwake_autonomy/src/moonwake_ros2_training
```

检查修改状态：

```bash
git status
```

确认当前分支符合任务要求，并检查修改内容后，再暂存和提交：

```bash
git add README.md config/training.yaml launch/training.launch.py src/target_offset_publisher.cpp src/steering_hint_node.cpp CMakeLists.txt package.xml
git commit -m "Complete ROS 2 task 02 communication and configuration"
git push -u origin <任务分支名>
```

将 `<任务分支名>` 替换为实际的任务分支名，不要原样输入尖括号内容。

推送成功后，在 GitHub 上创建 Pull Request，将任务分支合并到项目要求的目标分支。
