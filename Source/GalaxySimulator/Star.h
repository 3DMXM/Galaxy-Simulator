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

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Star")
    void UpdateStarLight();
};
