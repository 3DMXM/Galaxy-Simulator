# Galaxy Simulator - 资源目录结构说明

## 📁 目录结构概览

```
GalaxySimulator/
├── Blueprints/              # 蓝图资源
│   ├── CelestialBodies/     # 天体蓝图
│   │   ├── Stars/           # 恒星蓝图 (BP_Star_Sun, BP_Star_RedGiant等)
│   │   ├── Planets/         # 行星蓝图 (BP_Planet_Earth, BP_Planet_Mars等)
│   │   ├── Moons/           # 卫星蓝图 (BP_Moon_Luna等)
│   │   └── Asteroids/       # 小行星蓝图
│   ├── GameModes/           # 游戏模式蓝图 (BP_GalaxyGameMode)
│   ├── Controllers/         # 控制器蓝图 (BP_GalaxyPlayerController)
│   ├── Pawns/               # Pawn蓝图 (BP_GalaxyCameraPawn)
│   └── Managers/            # 管理器蓝图 (BP_GalaxyManager)
│
├── Materials/               # 材质资源
│   ├── Planets/             # 行星材质
│   │   ├── Rocky/           # 岩石行星材质 (M_Planet_Rocky, MI_Mars等)
│   │   ├── Ice/             # 冰行星材质 (M_Planet_Ice, MI_Europa等)
│   │   ├── Lava/            # 熔岩行星材质 (M_Planet_Lava, MI_Io等)
│   │   ├── Gas/             # 气态行星材质 (M_Planet_Gas, MI_Jupiter等)
│   │   ├── Desert/          # 沙漠行星材质
│   │   └── Ocean/           # 海洋行星材质 (M_Planet_Ocean, MI_Earth等)
│   ├── Stars/               # 恒星材质 (M_Star_Base, MI_Sun等)
│   └── Effects/             # 特效材质 (光晕、大气等)
│
├── Textures/                # 纹理资源
│   ├── Planets/             # 行星纹理 (颜色贴图、法线贴图、粗糙度贴图)
│   ├── Stars/               # 恒星纹理
│   └── UI/                  # UI图标和按钮纹理
│
├── Meshes/                  # 网格资源
│   ├── Planets/             # 行星网格 (如果需要特殊形状)
│   └── Asteroids/           # 小行星网格 (不规则形状)
│
├── UI/                      # UI资源
│   ├── Widgets/             # UMG控件
│   │   ├── MainMenu/        # 主菜单UI (WBP_MainMenu)
│   │   ├── HUD/             # 游戏HUD (WBP_GalaxyHUD)
│   │   └── PropertyPanels/  # 属性编辑面板 (WBP_BodyProperties等)
│   └── Fonts/               # 字体资源
│
├── Maps/                    # 关卡地图
│   ├── MainLevel/           # 主关卡 (L_Galaxy_Main)
│   └── PresetScenes/        # 预设场景 (L_SolarSystem, L_BinaryStars等)
│
├── VFX/                     # 视觉特效
│   ├── Particles/           # 级联粒子系统 (旧版)
│   └── Niagara/             # Niagara特效 (星云、尾迹等)
│
├── Audio/                   # 音频资源
│   ├── Music/               # 背景音乐
│   └── SFX/                 # 音效 (UI点击声、创建天体声等)
│
└── Data/                    # 数据资源
    ├── DataTables/          # 数据表 (DT_PlanetPresets, DT_StarData等)
    └── Presets/             # 预设配置
```

## 🎯 使用指南

### 1. Blueprints - 蓝图目录

#### CelestialBodies (天体蓝图)
**Stars (恒星):**
- `BP_Star_Sun` - 类太阳恒星
- `BP_Star_RedGiant` - 红巨星
- `BP_Star_WhiteDwarf` - 白矮星
- `BP_Star_Neutron` - 中子星

**Planets (行星):**
- `BP_Planet_Earth` - 地球类行星
- `BP_Planet_Mars` - 火星类行星
- `BP_Planet_Jupiter` - 木星类气态巨行星
- `BP_Planet_Saturn` - 土星类行星(带环)
- `BP_Planet_Ice` - 冰行星
- `BP_Planet_Lava` - 熔岩行星

**Moons (卫星):**
- `BP_Moon_Luna` - 月球类卫星
- `BP_Moon_Io` - 火山卫星
- `BP_Moon_Europa` - 冰卫星

**Asteroids (小行星):**
- `BP_Asteroid_Small` - 小型小行星
- `BP_Asteroid_Medium` - 中型小行星
- `BP_Asteroid_Large` - 大型小行星

#### 其他蓝图
- **GameModes/**: `BP_GalaxyGameMode` - 继承自C++的AGalaxyGameMode
- **Controllers/**: `BP_GalaxyPlayerController` - 玩家控制器
- **Pawns/**: `BP_GalaxyCameraPawn` - 自由相机
- **Managers/**: `BP_GalaxyManager` - 星系管理器

### 2. Materials - 材质目录

#### 命名规范
- `M_` - 材质主文件 (Material)
- `MI_` - 材质实例 (Material Instance)
- `MF_` - 材质函数 (Material Function)

#### 行星材质示例
**岩石行星:**
```
M_Planet_Rocky (主材质)
├── MI_Mercury (水星)
├── MI_Mars (火星)
└── MI_Moon (月球)
```

**气态行星:**
```
M_Planet_Gas (主材质)
├── MI_Jupiter (木星)
├── MI_Saturn (土星)
└── MI_Neptune (海王星)
```

### 3. Textures - 纹理目录

#### 纹理命名规范
- `T_PlanetName_BaseColor` - 基础颜色
- `T_PlanetName_Normal` - 法线贴图
- `T_PlanetName_Roughness` - 粗糙度
- `T_PlanetName_Emissive` - 自发光

#### 推荐纹理分辨率
- 恒星: 2048x1024 或 4096x2048
- 大型行星: 2048x1024
- 中型行星: 1024x512
- 小型天体: 512x256
- UI图标: 256x256 或 512x512

### 4. UI - 用户界面

#### 主要Widget
**WBP_GalaxyHUD** - 主游戏界面
- 顶部工具栏 (创建天体按钮)
- 侧边属性面板
- 底部信息栏
- 时间控制

**WBP_BodyProperties** - 天体属性编辑面板
- 名称输入框
- 质量滑动条
- 半径滑动条
- 材质类型下拉菜单
- 轨道参数编辑

**WBP_MainMenu** - 主菜单
- 新建星系
- 加载星系
- 设置
- 退出

**WBP_CreateBodyMenu** - 创建天体菜单
- 天体类型选择
- 快速预设选项

### 5. Maps - 关卡地图

#### 主关卡
**L_Galaxy_Main**
- 空白星系场景
- 包含GalaxyManager
- 默认相机位置

#### 预设场景
**L_SolarSystem** - 太阳系
- 太阳 + 8大行星
- 主要卫星

**L_BinaryStars** - 双星系统
- 两颗恒星互相环绕

**L_Sandbox** - 沙盒模式
- 完全空白，供玩家自由创建

### 6. VFX - 视觉特效

#### Niagara特效
- `NS_StarGlow` - 恒星光晕效果
- `NS_PlanetAtmosphere` - 行星大气效果
- `NS_OrbitTrail` - 轨道轨迹特效
- `NS_SpaceParticles` - 空间粒子背景
- `NS_Explosion` - 天体碰撞爆炸效果

### 7. Audio - 音频资源

#### 音乐
- `MUS_Ambient_Space` - 空间环境音乐
- `MUS_Menu_Main` - 主菜单音乐

#### 音效
- `SFX_UI_Click` - UI点击音效
- `SFX_Body_Create` - 创建天体音效
- `SFX_Body_Delete` - 删除天体音效
- `SFX_Orbit_Whoosh` - 天体运动音效

### 8. Data - 数据资源

#### DataTables (数据表)
**DT_PlanetPresets** - 行星预设数据
```
列: Name, Mass, Radius, MaterialType, OrbitDistance
行: Earth, Mars, Jupiter, Saturn...
```

**DT_StarTypes** - 恒星类型数据
```
列: Type, Mass, Radius, Temperature, Color, Luminosity
行: MainSequence, RedGiant, WhiteDwarf...
```

## 📝 资源创建清单

### 优先级 1 - 核心功能
- [ ] BP_GalaxyGameMode
- [ ] BP_GalaxyPlayerController
- [ ] BP_GalaxyCameraPawn
- [ ] BP_GalaxyManager
- [ ] WBP_GalaxyHUD (基础版)
- [ ] L_Galaxy_Main
- [ ] BP_Star_Sun (基础恒星)
- [ ] BP_Planet_Earth (基础行星)

### 优先级 2 - 材质系统
- [ ] M_Planet_Rocky (主材质)
- [ ] M_Planet_Gas (主材质)
- [ ] M_Planet_Ocean (主材质)
- [ ] M_Star_Base (主材质)
- [ ] 为每种材质创建2-3个实例

### 优先级 3 - 完整天体库
- [ ] 创建所有行星类型蓝图
- [ ] 创建卫星蓝图
- [ ] 创建小行星蓝图
- [ ] 创建不同类型恒星蓝图

### 优先级 4 - UI完善
- [ ] WBP_BodyProperties (完整版)
- [ ] WBP_CreateBodyMenu
- [ ] WBP_MainMenu
- [ ] UI图标和按钮设计

### 优先级 5 - 视觉增强
- [ ] 星云背景
- [ ] 恒星光晕特效
- [ ] 行星大气特效
- [ ] 轨道可视化特效

### 优先级 6 - 预设场景
- [ ] L_SolarSystem (太阳系)
- [ ] L_BinaryStars (双星系统)
- [ ] L_Sandbox (沙盒)
- [ ] 数据表和预设配置

## 🎨 推荐资源来源

### 免费纹理资源
- [Solar System Scope](https://www.solarsystemscope.com/textures/) - 行星纹理
- [NASA Images](https://images.nasa.gov/) - 真实太空图像
- [TexturesCom](https://www.textures.com/) - 通用纹理

### 3D模型
- [Sketchfab](https://sketchfab.com/) - 小行星模型
- [TurboSquid](https://www.turbosquid.com/) - 商业模型

### 音效和音乐
- [Freesound](https://freesound.org/) - 免费音效
- [Incompetech](https://incompetech.com/) - 免费音乐

## 💡 最佳实践

### 命名规范
1. **蓝图**: `BP_TypeName` (如 BP_Planet_Earth)
2. **材质**: `M_TypeName` / `MI_InstanceName`
3. **纹理**: `T_ObjectName_Type` (如 T_Earth_BaseColor)
4. **Widget**: `WBP_WidgetName`
5. **关卡**: `L_LevelName`
6. **特效**: `NS_EffectName` (Niagara) / `PS_EffectName` (Particle)

### 文件夹组织
- 每个主要功能放在独立文件夹
- 相关资源放在一起
- 避免深层嵌套(不超过4层)

### 性能优化
- 使用材质实例而非创建新材质
- 纹理使用适当的分辨率
- LOD系统用于远距离天体
- 合理使用Tick和定时器

## 🔄 下一步

1. 创建核心蓝图类(从C++类派生)
2. 设计并实现主UI界面
3. 创建基础材质和纹理
4. 构建示例场景
5. 实现保存/加载系统
6. 性能优化和打包

---

**提示**: 这个目录结构是为星系模拟游戏优化的，可以根据项目需求调整。记得定期备份重要资源！
