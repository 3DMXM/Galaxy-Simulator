// Copyright Epic Games, Inc. All Rights Reserved.

#include "OrbitComponent.h"
#include "CelestialBody.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

UOrbitComponent::UOrbitComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // 默认轨道参数（类地球轨道）
    SemiMajorAxis = 149600.0f; // 1 AU in thousands of km
    Eccentricity = 0.0167f;
    Inclination = 0.0f;
    ArgumentOfPeriapsis = 0.0f;
    LongitudeOfAscendingNode = 0.0f;
    OrbitalPeriod = 365.25f; // 天
    CurrentAnomaly = 0.0f;
    bEnableOrbit = true;
    TimeScale = 1.0f;

    bDrawOrbitPath = true;
    OrbitSegments = 100;
    OrbitColor = FColor::Cyan;

    AccumulatedTime = 0.0f;
    ParentBody = nullptr;
}

void UOrbitComponent::BeginPlay()
{
    Super::BeginPlay();

    // 获取父天体引用
    ACelestialBody *Owner = Cast<ACelestialBody>(GetOwner());
    if (Owner)
    {
        ParentBody = Owner->ParentBody;
    }
}

void UOrbitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bEnableOrbit || !ParentBody)
    {
        return;
    }

    // 更新轨道位置
    AccumulatedTime += DeltaTime * TimeScale;

    // 计算当前真近点角
    float DaysPerSecond = TimeScale / 86400.0f;     // 转换为天
    float AngularVelocity = 360.0f / OrbitalPeriod; // 度/天
    CurrentAnomaly += AngularVelocity * DaysPerSecond * DeltaTime;

    // 保持在0-360度范围内
    if (CurrentAnomaly >= 360.0f)
    {
        CurrentAnomaly -= 360.0f;
    }

    // 计算新位置
    FVector NewPosition = CalculatePositionFromAnomaly(CurrentAnomaly);
    GetOwner()->SetActorLocation(ParentBody->GetActorLocation() + NewPosition);

    // 绘制轨道路径
    if (bDrawOrbitPath)
    {
        DrawOrbit();
    }
}

void UOrbitComponent::SetOrbitParameters(float InSemiMajorAxis, float InEccentricity, float InInclination)
{
    SemiMajorAxis = InSemiMajorAxis;
    Eccentricity = FMath::Clamp(InEccentricity, 0.0f, 0.99f);
    Inclination = InInclination;
}

FVector UOrbitComponent::CalculateOrbitPosition(float Anomaly)
{
    return CalculatePositionFromAnomaly(Anomaly);
}

FVector UOrbitComponent::CalculatePositionFromAnomaly(float Anomaly)
{
    if (!ParentBody)
    {
        return FVector::ZeroVector;
    }

    // 将角度转换为弧度
    float AnomalyRad = FMath::DegreesToRadians(Anomaly);
    float InclinationRad = FMath::DegreesToRadians(Inclination);
    float ArgPeriapsisRad = FMath::DegreesToRadians(ArgumentOfPeriapsis);
    float LongNodeRad = FMath::DegreesToRadians(LongitudeOfAscendingNode);

    // 计算轨道平面上的位置
    float r = SemiMajorAxis * (1.0f - Eccentricity * Eccentricity) / (1.0f + Eccentricity * FMath::Cos(AnomalyRad));

    // 轨道坐标系中的位置
    float x = r * FMath::Cos(AnomalyRad);
    float y = r * FMath::Sin(AnomalyRad);

    // 应用轨道旋转
    // 1. 近日点幅角旋转
    float x1 = x * FMath::Cos(ArgPeriapsisRad) - y * FMath::Sin(ArgPeriapsisRad);
    float y1 = x * FMath::Sin(ArgPeriapsisRad) + y * FMath::Cos(ArgPeriapsisRad);

    // 2. 倾角旋转
    float x2 = x1;
    float y2 = y1 * FMath::Cos(InclinationRad);
    float z2 = y1 * FMath::Sin(InclinationRad);

    // 3. 升交点经度旋转
    float x3 = x2 * FMath::Cos(LongNodeRad) - y2 * FMath::Sin(LongNodeRad);
    float y3 = x2 * FMath::Sin(LongNodeRad) + y2 * FMath::Cos(LongNodeRad);
    float z3 = z2;

    // 缩放到虚幻引擎单位 (1000km = 100单位)
    float Scale = 0.1f;
    return FVector(x3 * Scale, y3 * Scale, z3 * Scale);
}

void UOrbitComponent::DrawOrbit()
{
    if (!ParentBody || !GetWorld())
    {
        return;
    }

    FVector ParentLocation = ParentBody->GetActorLocation();

    // 绘制轨道路径
    for (int32 i = 0; i < OrbitSegments; i++)
    {
        float Angle1 = (360.0f / OrbitSegments) * i;
        float Angle2 = (360.0f / OrbitSegments) * ((i + 1) % OrbitSegments);

        FVector Point1 = ParentLocation + CalculatePositionFromAnomaly(Angle1);
        FVector Point2 = ParentLocation + CalculatePositionFromAnomaly(Angle2);

        DrawDebugLine(GetWorld(), Point1, Point2, OrbitColor, false, -1.0f, 0, 2.0f);
    }
}
