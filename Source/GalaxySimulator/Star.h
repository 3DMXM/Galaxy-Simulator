// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "Star.generated.h"

/**
 * 恒星类 - 发光的天体
 */
UCLASS()
class GALAXYSIMULATOR_API AStar : public ACelestialBody
{
    GENERATED_BODY()

public:
    AStar();

    // 恒星特有属性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties")
    float Luminosity; // 光度

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties")
    float Temperature; // 温度 (开尔文)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties")
    FLinearColor StarColor; // 恒星颜色

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UPointLightComponent *LightComponent;

    // 自转属性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties|Rotation")
    float RotationSpeed; // 自转速度 (度/秒)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties|Rotation")
    FVector RotationAxis; // 自转轴 (归一化向量)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties|Rotation")
    bool bEnableRotation; // 是否启用自转

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Star")
    void UpdateStarLight();

    UFUNCTION(BlueprintCallable, Category = "Star")
    void SetRotationSpeed(float NewSpeed);

    UFUNCTION(BlueprintCallable, Category = "Star")
    void SetRotationAxis(FVector NewAxis);
};
