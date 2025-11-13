# Galaxy Simulator - 星系模拟游戏

## 项目概述

这是一个基于虚幻引擎5.7的星系模拟游戏,玩家可以创建和管理各种天体,包括恒星、行星、卫星和小行星。游戏支持自定义星体的物理属性、轨道参数和视觉效果。

## 核心功能

### 1. 天体系统
- **天体类型**:
  - 恒星 (Star) - 带有光源效果
  - 行星 (Planet)
  - 卫星 (Moon)
  - 小行星 (Asteroid)

- **可自定义属性**:
  - 质量 (Mass)
  - 半径 (Radius)
  - 材质类型 (岩石、冰、熔岩、气态、沙漠、海洋)
  - 名称

### 2. 轨道系统
使用开普勒轨道参数实现真实的天体运动:
- 半长轴 (Semi-Major Axis)
- 离心率 (Eccentricity)
- 轨道倾角 (Inclination)
- 近日点幅角 (Argument of Periapsis)
- 升交点经度 (Longitude of Ascending Node)
- 轨道周期 (Orbital Period)
- 时间缩放功能

### 3. 星系管理器
- 管理所有天体
- 可选的N体引力模拟
- 动态添加/删除天体
- 预设太阳系创建功能

### 4. 玩家控制
- 自由相机移动 (WASD键)
- 相机升降 (E/Q键)
- 鼠标控制视角
- 鼠标滚轮缩放
- 点击选择天体

## 核心类说明

### C++ 类

1. **ACelestialBody** (`CelestialBody.h/cpp`)
   - 所有天体的基类
   - 包含物理属性、视觉组件和引力计算

2. **AStar** (`Star.h/cpp`)
   - 恒星类,继承自ACelestialBody
   - 包含光源组件和发光效果

3. **UOrbitComponent** (`OrbitComponent.h/cpp`)
   - 轨道运动组件
   - 实现开普勒轨道运动
   - 可视化轨道路径

4. **AGalaxyManager** (`GalaxyManager.h/cpp`)
   - 星系管理器
   - 管理所有天体及其交互
   - 提供创建、删除天体的接口

5. **AGalaxyPlayerController** (`GalaxyPlayerController.h/cpp`)
   - 玩家控制器
   - 处理输入和UI交互
   - 管理天体选择和编辑

6. **AGalaxyCameraPawn** (`GalaxyCameraPawn.h/cpp`)
   - 自由相机Pawn
   - 用于在3D空间中自由观察星系

7. **AGalaxyGameMode** (`GalaxyGameMode.h/cpp`)
   - 游戏模式
   - 设置默认的Pawn和控制器

## 使用说明

### 在编辑器中设置

1. **编译项目**:
   - 在VS Code中运行任务: "GalaxySimulatorEditor Win64 Development Build"
   - 或在Visual Studio中编译项目

2. **打开虚幻编辑器**:
   - 打开 `GalaxySimulator.uproject`

3. **创建星系场景**:
   - 在关卡中放置 `GalaxyManager` Actor
   - 可选:在蓝图中调用 `CreateSolarSystem()` 创建示例太阳系

4. **添加天体**:
   - 通过蓝图或C++代码调用 `GalaxyManager->SpawnCelestialBody()`
   - 设置天体属性 (质量、半径、材质类型等)
   - 为天体添加 `OrbitComponent` 并设置轨道参数

5. **配置轨道**:
   - 设置 `ParentBody` 指向要围绕运行的天体
   - 调整轨道参数 (半长轴、离心率等)
   - 启用 `bEnableOrbit` 开始轨道运动
   - 调整 `TimeScale` 加速或减速模拟

### 控制方式

- **W/S**: 前后移动相机
- **A/D**: 左右移动相机
- **E/Q**: 升降相机
- **鼠标移动**: 旋转视角 (按住鼠标右键)
- **鼠标滚轮**: 缩放
- **左键点击**: 选择天体
- **右键点击**: 取消选择

### 蓝图使用

所有核心类都标记为 `BlueprintType` 和 `BlueprintCallable`,可以在蓝图中使用:

1. **创建天体蓝图**:
   - 从 `CelestialBody` 或 `Star` 派生蓝图
   - 自定义材质和视觉效果
   - 设置默认属性值

2. **创建UI蓝图**:
   - 创建UMG Widget继承自UserWidget
   - 绑定到 `GalaxyPlayerController->MainUIWidgetClass`
   - 调用控制器函数操作天体

3. **自定义游戏模式**:
   - 从 `GalaxyGameMode` 派生蓝图
   - 在Project Settings中设置为默认游戏模式

## 扩展建议

### 增强功能

1. **视觉效果**:
   - 为不同材质类型创建专门的材质
   - 添加大气效果
   - 添加行星环、云层等

2. **物理模拟**:
   - 实现更精确的引力计算
   - 添加碰撞检测
   - 实现潮汐锁定效果

3. **UI功能**:
   - 创建天体编辑面板
   - 添加时间控制UI (暂停、加速、减速)
   - 显示轨道信息和统计数据
   - 添加预设场景选择器

4. **游戏性**:
   - 添加任务系统
   - 实现稳定轨道挑战
   - 添加资源管理
   - 多人协作模式

5. **性能优化**:
   - 实现LOD系统
   - 优化远距离天体渲染
   - 使用实例化渲染大量小行星

## 文件结构

```
GalaxySimulator/
├── Source/GalaxySimulator/
│   ├── CelestialBody.h/cpp          # 基础天体类
│   ├── Star.h/cpp                   # 恒星类
│   ├── OrbitComponent.h/cpp         # 轨道组件
│   ├── GalaxyManager.h/cpp          # 星系管理器
│   ├── GalaxyPlayerController.h/cpp # 玩家控制器
│   ├── GalaxyCameraPawn.h/cpp       # 相机Pawn
│   ├── GalaxyGameMode.h/cpp         # 游戏模式
│   └── GalaxySimulator.Build.cs     # 模块构建配置
├── Config/
│   ├── DefaultEngine.ini            # 引擎配置
│   ├── DefaultGame.ini              # 游戏配置
│   └── DefaultInput.ini             # 输入配置
└── Content/                         # 蓝图和资源目录
```

## 技术要点

### 轨道计算
使用开普勒轨道方程计算天体位置:
- r = a(1-e²)/(1+e·cos(θ))
- 其中 a=半长轴, e=离心率, θ=真近点角

### 引力计算
使用牛顿万有引力公式:
- F = G·(m₁·m₂)/r²
- 可通过 `bEnableGravitySimulation` 启用N体模拟

### 坐标系统
- 虚幻引擎单位: 厘米
- 游戏中: 1000千米 = 100虚幻单位 (可调整)
- 使用右手坐标系

## 调试建议

1. **轨道可视化**:
   - 启用 `OrbitComponent->bDrawOrbitPath` 查看轨道
   - 调整 `OrbitSegments` 改变轨道线段数

2. **日志输出**:
   - 使用 `UE_LOG(LogTemp, Log, TEXT("..."))` 调试
   - 查看Output Log窗口

3. **性能监控**:
   - 使用 `stat FPS` 查看帧率
   - 使用 `stat Unit` 查看性能统计

## 常见问题

**Q: 天体不移动?**
A: 检查 OrbitComponent 的 `bEnableOrbit` 是否为true,以及是否设置了 `ParentBody`

**Q: 轨道路径不显示?**
A: 确保 `bDrawOrbitPath` 为true,并且在Play模式下运行

**Q: 相机控制不响应?**
A: 检查 `DefaultInput.ini` 是否正确配置了输入绑定

**Q: 如何加快模拟速度?**
A: 增加 `OrbitComponent->TimeScale` 或 `GalaxyManager->SimulationSpeed`

## 版权信息

Copyright Epic Games, Inc. All Rights Reserved.

## 下一步开发

1. 创建示例地图和预设场景
2. 实现完整的UI系统
3. 添加保存/加载功能
4. 创建教程关卡
5. 优化性能和内存使用
