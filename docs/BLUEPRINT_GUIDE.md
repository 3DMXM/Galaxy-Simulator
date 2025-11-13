# 蓝图实现指南

本文档说明如何在蓝图中使用星系模拟器系统。

## 创建天体蓝图

### 1. 创建行星蓝图

1. 在Content Browser中右键 -> Blueprint Class
2. 选择 `CelestialBody` 作为父类
3. 命名为 `BP_Planet_Earth` (或其他名称)
4. 打开蓝图编辑器

**设置组件:**
- 选择 `MeshComponent`
- 在Details面板设置Static Mesh为球体
- 创建或指定材质 (如地球纹理材质)

**设置默认属性:**
在Class Defaults中设置:
```
Body Type: Planet
Material Type: Ocean
Body Name: "地球"
Mass: 1.0
Radius: 6371.0
```

**添加轨道组件:**
1. 点击 Add Component -> Orbit Component
2. 设置轨道参数:
```
Semi Major Axis: 149600.0
Eccentricity: 0.0167
Orbital Period: 365.25
Time Scale: 100.0
Enable Orbit: true
Draw Orbit Path: true
```

### 2. 创建恒星蓝图

1. 创建蓝图类,父类选择 `Star`
2. 命名为 `BP_Star_Sun`

**设置属性:**
```
Body Name: "太阳"
Mass: 333000.0
Radius: 696000.0
Luminosity: 1.0
Temperature: 5778.0
Star Color: (1.0, 1.0, 0.8, 1.0) // 黄白色
```

**优化光源:**
- 选择 `LightComponent`
- 调整Intensity和Attenuation Radius
- 设置Cast Shadows等属性

## 创建UI蓝图

### 主UI控制面板

1. 创建Widget Blueprint: `WBP_GalaxyUI`
2. 设计布局:

**顶部工具栏:**
- 按钮: 创建恒星
- 按钮: 创建行星
- 按钮: 创建卫星
- 按钮: 创建小行星
- 按钮: 删除选中天体

**侧边属性面板 (当选中天体时显示):**
- 文本框: 名称
- 滑动条: 质量 (0.01 - 1000000)
- 滑动条: 半径 (100 - 1000000)
- 下拉菜单: 材质类型
- 按钮: 应用更改

**底部信息栏:**
- 文本: 当前选中天体信息
- 滑动条: 模拟速度
- 按钮: 暂停/继续

### 蓝图脚本示例

**创建天体按钮事件:**
```
Event: OnClicked (Create Planet Button)
  -> Get Player Controller
  -> Cast to GalaxyPlayerController
  -> Get Galaxy Manager
  -> Spawn Celestial Body
     - Body Class: BP_Planet_Earth (选择类引用)
     - Spawn Location: (0, 0, 0) 或鼠标位置
```

**更新属性按钮事件:**
```
Event: OnClicked (Apply Changes Button)
  -> Get Player Controller
  -> Cast to GalaxyPlayerController
  -> Update Selected Body Properties
     - New Mass: (从滑动条获取)
     - New Radius: (从滑动条获取)
     - New Material Type: (从下拉菜单获取)
```

**删除天体按钮事件:**
```
Event: OnClicked (Delete Button)
  -> Get Player Controller
  -> Cast to GalaxyPlayerController
  -> Delete Selected Body
```

## 创建示例关卡

### Level Blueprint设置

1. 打开关卡蓝图 (Level Blueprint)
2. 在Begin Play事件后:

**生成星系管理器:**
```
Event BeginPlay
  -> Spawn Actor from Class
     - Class: GalaxyManager
     - Location: (0, 0, 0)
  -> (可选) Call CreateSolarSystem
```

**创建示例太阳系:**
```
// 获取GalaxyManager引用
Get Galaxy Manager
  -> Create Solar System (调用函数)
```

或手动创建:
```
Get Galaxy Manager -> Spawn Celestial Body
  - Class: BP_Star_Sun
  - Location: (0, 0, 0)
  -> Promote to Variable: SunReference

Get Galaxy Manager -> Spawn Celestial Body
  - Class: BP_Planet_Earth
  - Location: (14960, 0, 0)
  -> Get Return Value
  -> Set Parent Body = SunReference
```

## 高级蓝图技巧

### 1. 动态轨道调整

创建函数蓝图:
```
Function: AdjustOrbit
Inputs:
  - Target Body (CelestialBody Reference)
  - New Semi Major Axis (Float)
  - New Eccentricity (Float)

Script:
  -> Get Component by Class (OrbitComponent)
  -> Set Orbit Parameters
     - Semi Major Axis: New Semi Major Axis
     - Eccentricity: New Eccentricity
```

### 2. 相机跟随天体

在Player Controller蓝图中:
```
Event Tick
  -> Is Valid (Selected Body)
     TRUE:
       -> Get Actor Location (Selected Body)
       -> Set View Target with Blend
          - New View Target: Selected Body
          - Blend Time: 1.0
```

### 3. 时间控制

创建Widget函数:
```
Function: SetSimulationSpeed
Input: Speed (Float)

Script:
  -> Get All Actors of Class (GalaxyManager)
  -> ForEach Loop
     -> Set Simulation Speed = Speed
```

暂停/继续:
```
Function: TogglePause

Script:
  -> Get All Actors of Class (GalaxyManager)
  -> ForEach Loop
     -> Get Pause Simulation
     -> NOT Boolean
     -> Set Pause Simulation
```

### 4. 轨道预览

显示轨道路径:
```
Function: ShowOrbitPath
Input: Body (CelestialBody Reference)
      Show (Boolean)

Script:
  -> Get Component by Class (OrbitComponent)
  -> Set Draw Orbit Path = Show
```

## 材质设置

### 创建行星材质

1. 创建Material: `M_Planet_Base`
2. 设置节点:
   - Texture Sample (Base Color贴图)
   - Texture Sample (Normal贴图)
   - Texture Sample (Roughness贴图)
   - 连接到Material Output

3. 创建Material Instance: `MI_Planet_Earth`
4. 设置参数和纹理

### 在蓝图中应用材质

在Construction Script中:
```
Event Construction Script
  -> Switch on Enum (Material Type)
     Rocky: Set Material (MeshComponent) -> MI_Planet_Rocky
     Ice: Set Material (MeshComponent) -> MI_Planet_Ice
     Lava: Set Material (MeshComponent) -> MI_Planet_Lava
     Gas: Set Material (MeshComponent) -> MI_Planet_Gas
     etc...
```

## 输入处理

### 自定义键盘快捷键

在Player Controller蓝图中:
```
Event: Key Press (按键名,如 "C")
  -> Branch (是否按下Shift)
     TRUE: 创建新天体在鼠标位置
     FALSE: 其他操作
```

### 鼠标拖放天体

```
Event: Left Mouse Button (Pressed)
  -> Line Trace by Channel
  -> Break Hit Result
     Block Hit:
       -> Cast to CelestialBody
       -> Set as Dragging Target

Event: Left Mouse Button (Released)
  -> Clear Dragging Target

Event Tick (当Dragging Target有效时)
  -> Get Mouse Position
  -> Convert to World Location
  -> Set Actor Location (Dragging Target)
```

## 调试工具蓝图

创建Debug Widget显示信息:

**显示当前选中天体信息:**
```
Event Tick
  -> Get Player Controller
  -> Cast to GalaxyPlayerController
  -> Get Selected Body
  -> Is Valid
     TRUE:
       -> Get Body Name
       -> Get Mass
       -> Get Radius
       -> Format Text: "名称: {0}\n质量: {1}\n半径: {2}"
       -> Set Text (Text Block)
```

## 保存/加载系统 (未来扩展)

预留的蓝图结构:
```
Function: SaveGalaxy
  -> Get All Actors of Class (CelestialBody)
  -> ForEach:
     -> 创建结构体保存属性
     -> 添加到数组
  -> Save Game to Slot

Function: LoadGalaxy
  -> Load Game from Slot
  -> ForEach saved data:
     -> Spawn Celestial Body
     -> 恢复属性
```

## 性能优化提示

1. **减少Tick频率:**
   - 远离相机的天体降低Tick频率
   - 使用Timer代替Tick

2. **LOD使用:**
   ```
   Event Tick
     -> Get Distance to Camera
     -> Branch (Distance > 50000)
        TRUE: Set LOD = 2
        FALSE: Set LOD = 0
   ```

3. **对象池:**
   - 预先生成天体
   - 重用而非频繁创建/销毁

## 测试场景

创建几个预设场景蓝图:

1. **BP_Scene_SolarSystem**: 太阳系
2. **BP_Scene_BinaryStars**: 双星系统
3. **BP_Scene_Sandbox**: 空白沙盒

在关卡中放置并通过UI切换。
