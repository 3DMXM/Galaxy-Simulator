// Copyright Epic Games, Inc. All Rights Reserved.

#include "Star.h"
#include "Components/PointLightComponent.h"

AStar::AStar()
{
    BodyType = ECelestialBodyType::Star;
    MaterialType = EPlanetMaterialType::Gas;

    // 默认恒星属性
    Mass = 333000.0f;                           // 太阳质量（以地球质量为单位）
    Radius = 696000.0f;                         // 太阳半径（千米）
    Luminosity = 1.0f;                          // 相对太阳光度
    Temperature = 5778.0f;                      // 太阳表面温度
    StarColor = FLinearColor(1.0f, 1.0f, 0.8f); // 黄白色

    // 自转属性 (太阳自转周期约25天)
    RotationSpeed = 0.58f;                    // 度/秒 (360度/25天 ≈ 0.58度/秒)
    RotationAxis = FVector(0.0f, 0.0f, 1.0f); // 默认绕Z轴自转
    bEnableRotation = true;

    // 创建光源组件
    LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
    LightComponent->SetupAttachment(RootComponent);
    LightComponent->SetIntensity(10000.0f);
    LightComponent->SetAttenuationRadius(1000000.0f);
    LightComponent->SetLightColor(StarColor);

    SetBodyScale(Radius);
}

void AStar::BeginPlay()
{
    Super::BeginPlay();
    UpdateStarLight();
}

void AStar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 处理自转
    if (bEnableRotation && RotationSpeed != 0.0f)
    {
        // 计算本帧的旋转角度
        float RotationAngle = RotationSpeed * DeltaTime;

        // 创建旋转四元数
        FQuat RotationQuat = FQuat(RotationAxis, FMath::DegreesToRadians(RotationAngle));

        // 应用旋转
        FRotator CurrentRotation = GetActorRotation();
        FQuat CurrentQuat = CurrentRotation.Quaternion();
        FQuat NewQuat = RotationQuat * CurrentQuat;

        SetActorRotation(NewQuat);
    }
}

void AStar::UpdateStarLight()
{
    if (LightComponent)
    {
        LightComponent->SetLightColor(StarColor);
        LightComponent->SetIntensity(Luminosity * 10000.0f);
    }
}

void AStar::SetRotationSpeed(float NewSpeed)
{
    RotationSpeed = NewSpeed;
}

void AStar::SetRotationAxis(FVector NewAxis)
{
    // 归一化旋转轴
    RotationAxis = NewAxis.GetSafeNormal();
    if (RotationAxis.IsNearlyZero())
    {
        // 如果提供的轴为零向量，使用默认Z轴
        RotationAxis = FVector(0.0f, 0.0f, 1.0f);
    }
}
