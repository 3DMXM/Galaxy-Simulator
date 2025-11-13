# 快速开始 - 创建你的第一个天体

本指南将帮助你在虚幻编辑器中快速创建第一个天体蓝图。

## 步骤 1: 创建恒星蓝图

1. 打开虚幻编辑器
2. 在 Content Browser 中导航到: `Content/GalaxySimulator/Blueprints/CelestialBodies/Stars/`
3. 右键 -> Blueprint Class
4. 选择 `Star` 作为父类
5. 命名为 `BP_Star_Sun`

### 配置恒星属性

在 Class Defaults 中设置:
```
Body Name: "太阳"
Body Type: Star
Material Type: Gas
Mass: 333000.0
Radius: 696000.0
Luminosity: 1.0
Temperature: 5778.0
Star Color: RGB(255, 255, 204) / (1.0, 1.0, 0.8)
```

### 调整光源
选择 `LightComponent`:
- Intensity: 10000.0
- Attenuation Radius: 1000000.0
- Use Inverse Squared Falloff: True

## 步骤 2: 创建行星蓝图

1. 导航到: `Content/GalaxySimulator/Blueprints/CelestialBodies/Planets/`
2. 创建 Blueprint Class，父类选择 `CelestialBody`
3. 命名为 `BP_Planet_Earth`

### 配置行星属性

```
Body Name: "地球"
Body Type: Planet
Material Type: Ocean
Mass: 1.0
Radius: 6371.0
```

### 添加轨道组件

1. 在 Components 面板，点击 Add Component
2. 搜索并添加 `Orbit Component`
3. 设置轨道参数:
```
Semi Major Axis: 149600.0
Eccentricity: 0.0167
Inclination: 0.0
Orbital Period: 365.25
Time Scale: 100.0
Enable Orbit: True
Draw Orbit Path: True
Orbit Color: Cyan
Orbit Segments: 100
```

## 步骤 3: 创建游戏模式蓝图

1. 导航到: `Content/GalaxySimulator/Blueprints/GameModes/`
2. 创建 Blueprint Class，父类选择 `GalaxyGameMode`
3. 命名为 `BP_GalaxyGameMode`

### 设置默认类
在 Class Defaults 中:
- Default Pawn Class: `BP_GalaxyCameraPawn` (下一步创建)
- Player Controller Class: `BP_GalaxyPlayerController` (下一步创建)

## 步骤 4: 创建相机Pawn

1. 导航到: `Content/GalaxySimulator/Blueprints/Pawns/`
2. 创建 Blueprint Class，父类选择 `GalaxyCameraPawn`
3. 命名为 `BP_GalaxyCameraPawn`

### 调整相机参数
```
Movement Speed: 2000.0
Fast Movement Multiplier: 3.0
Mouse Sensitivity: 1.0
```

## 步骤 5: 创建控制器

1. 导航到: `Content/GalaxySimulator/Blueprints/Controllers/`
2. 创建 Blueprint Class，父类选择 `GalaxyPlayerController`
3. 命名为 `BP_GalaxyPlayerController`

## 步骤 6: 创建星系管理器

1. 导航到: `Content/GalaxySimulator/Blueprints/Managers/`
2. 创建 Blueprint Class，父类选择 `GalaxyManager`
3. 命名为 `BP_GalaxyManager`

### 配置管理器
```
Enable Gravity Simulation: False (使用轨道系统)
Simulation Speed: 1.0
Pause Simulation: False
```

## 步骤 7: 创建测试关卡

1. 导航到: `Content/GalaxySimulator/Maps/MainLevel/`
2. 创建新关卡: File -> New Level -> Empty Level
3. 保存为 `L_Galaxy_Test`

### 设置关卡

1. 添加 `BP_GalaxyManager` 到场景
   - 位置: (0, 0, 0)

2. 在 Level Blueprint 中添加初始化脚本:
```
Event BeginPlay
  -> Get Actor of Class (BP_GalaxyManager)
  -> Spawn Celestial Body
     Class: BP_Star_Sun
     Location: (0, 0, 0)
     -> Promote to Variable: "Sun Reference"
  
  -> Spawn Celestial Body  
     Class: BP_Planet_Earth
     Location: (14960, 0, 0)
     -> Get Return Value
     -> Set Parent Body = Sun Reference
```

3. 在 World Settings 中:
   - GameMode Override: `BP_GalaxyGameMode`

## 步骤 8: 测试

1. 点击 Play (Alt+P)
2. 使用 WASD 移动相机
3. 使用鼠标旋转视角
4. 使用滚轮缩放
5. 应该能看到地球围绕太阳运行!

## 常见问题

### Q: 看不到天体?
A: 
1. 检查天体是否在相机视野内
2. 调整相机位置到合适距离
3. 确认天体的 Visibility 设置正确

### Q: 轨道不显示?
A:
1. 确认 `Draw Orbit Path` = True
2. 确认在 Play 模式下(编辑器模式下不显示)
3. 调整 `Orbit Color` 使其可见

### Q: 天体不移动?
A:
1. 检查 `Enable Orbit` = True
2. 确认设置了 `Parent Body`
3. 检查 `Time Scale` 不为0
4. 确认 Galaxy Manager 的 `Pause Simulation` = False

### Q: 天体移动太快/太慢?
A:
调整以下参数:
- `Time Scale` (OrbitComponent) - 轨道速度倍率
- `Simulation Speed` (GalaxyManager) - 全局速度倍率
- `Orbital Period` - 轨道周期(天)

## 下一步

完成基础设置后，你可以:

1. **创建更多天体**: 火星、木星、土星等
2. **设计材质**: 为不同行星创建真实材质
3. **实现UI**: 创建控制面板
4. **添加特效**: 大气、光晕、尾迹等
5. **构建场景**: 创建完整的太阳系

参考完整文档: `Content/GalaxySimulator/README.md`

---

祝你创建愉快! 🌌✨
