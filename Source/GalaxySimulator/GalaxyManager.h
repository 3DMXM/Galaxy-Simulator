// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GalaxyManager.generated.h"

/**
 * 星系管理器 - 管理所有天体及其交互
 */
UCLASS()
class GALAXYSIMULATOR_API AGalaxyManager : public AActor
{
    GENERATED_BODY()

public:
    AGalaxyManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 天体列表
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Galaxy")
    TArray<class ACelestialBody *> CelestialBodies;

    // 模拟参数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    bool bEnableGravitySimulation; // 是否启用引力模拟

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    float SimulationSpeed; // 模拟速度倍率

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    bool bPauseSimulation; // 暂停模拟

    // 函数
    UFUNCTION(BlueprintCallable, Category = "Galaxy Manager")
    ACelestialBody *SpawnCelestialBody(TSubclassOf<ACelestialBody> BodyClass, FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Galaxy Manager")
    void RemoveCelestialBody(ACelestialBody *Body);

    UFUNCTION(BlueprintCallable, Category = "Galaxy Manager")
    void RegisterCelestialBody(ACelestialBody *Body);

    UFUNCTION(BlueprintCallable, Category = "Galaxy Manager")
    void UpdateGravity(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Galaxy Manager")
    TArray<ACelestialBody *> GetAllCelestialBodies() const { return CelestialBodies; }

    UFUNCTION(BlueprintCallable, Category = "Galaxy Manager")
    void ClearGalaxy();

    UFUNCTION(BlueprintCallable, Category = "Galaxy Manager")
    void CreateSolarSystem();

private:
    void ApplyGravitationalForces(float DeltaTime);
};
