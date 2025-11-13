// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"

// 天体类型枚举
UENUM(BlueprintType)
enum class ECelestialBodyType : uint8
{
    Star UMETA(DisplayName = "恒星"),
    Planet UMETA(DisplayName = "行星"),
    Moon UMETA(DisplayName = "卫星"),
    Asteroid UMETA(DisplayName = "小行星")
};

// 星体材质类型
UENUM(BlueprintType)
enum class EPlanetMaterialType : uint8
{
    Rocky UMETA(DisplayName = "岩石"),
    Ice UMETA(DisplayName = "冰"),
    Lava UMETA(DisplayName = "熔岩"),
    Gas UMETA(DisplayName = "气态"),
    Desert UMETA(DisplayName = "沙漠"),
    Ocean UMETA(DisplayName = "海洋")
};

/**
 * 基础天体类 - 所有星体的基类
 */
UCLASS()
class GALAXYSIMULATOR_API ACelestialBody : public AActor
{
    GENERATED_BODY()

public:
    ACelestialBody();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 可视化组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent *MeshComponent;

    // 基础属性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Properties")
    ECelestialBodyType BodyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Properties")
    EPlanetMaterialType MaterialType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Properties")
    FString BodyName;

    // 物理属性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Properties|Physics")
    float Mass; // 质量 (单位: 地球质量)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Properties|Physics")
    float Radius; // 半径 (单位: 千米)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Properties|Physics")
    FVector Velocity; // 速度向量

    // 轨道属性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Properties|Orbit")
    ACelestialBody *ParentBody; // 父天体 (围绕哪个天体运行)

    // 函数
    UFUNCTION(BlueprintCallable, Category = "Celestial Body")
    void SetBodyScale(float NewRadius);

    UFUNCTION(BlueprintCallable, Category = "Celestial Body")
    void UpdateMaterial();

    UFUNCTION(BlueprintCallable, Category = "Celestial Body")
    FVector CalculateGravitationalForce(ACelestialBody *OtherBody) const;

    // 获取引力常数 (简化版本，用于游戏)
    static constexpr float GravitationalConstant = 1.0f;
};
