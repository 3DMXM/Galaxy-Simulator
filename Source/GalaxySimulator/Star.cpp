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

void AStar::UpdateStarLight()
{
    if (LightComponent)
    {
        LightComponent->SetLightColor(StarColor);
        LightComponent->SetIntensity(Luminosity * 10000.0f);
    }
}
