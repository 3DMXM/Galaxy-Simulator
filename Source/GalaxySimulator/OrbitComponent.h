// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitComponent.generated.h"

/**
 * 轨道组件 - 处理天体的轨道运动
 * 使用开普勒轨道参数
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GALAXYSIMULATOR_API UOrbitComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    // 轨道参数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float SemiMajorAxis; // 半长轴 (单位: 千米)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float Eccentricity; // 离心率 (0 = 圆形, 0-1 = 椭圆)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float Inclination; // 轨道倾角 (度)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float ArgumentOfPeriapsis; // 近日点幅角 (度)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float LongitudeOfAscendingNode; // 升交点经度 (度)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float OrbitalPeriod; // 轨道周期 (天)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float CurrentAnomaly; // 当前真近点角 (度)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    bool bEnableOrbit; // 是否启用轨道运动

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Parameters")
    float TimeScale; // 时间缩放 (加速模拟)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization")
    bool bDrawOrbitPath; // 是否绘制轨道路径

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization")
    int32 OrbitSegments; // 轨道路径段数

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization")
    FColor OrbitColor; // 轨道颜色

    // 父天体引用
    UPROPERTY()
    class ACelestialBody *ParentBody;

    // 函数
    UFUNCTION(BlueprintCallable, Category = "Orbit")
    void SetOrbitParameters(float InSemiMajorAxis, float InEccentricity, float InInclination);

    UFUNCTION(BlueprintCallable, Category = "Orbit")
    FVector CalculateOrbitPosition(float Anomaly);

    UFUNCTION(BlueprintCallable, Category = "Orbit")
    void DrawOrbit();

private:
    float AccumulatedTime;

    FVector CalculatePositionFromAnomaly(float Anomaly);
};
