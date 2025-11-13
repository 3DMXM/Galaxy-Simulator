// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GalaxyCameraPawn.generated.h"

/**
 * 自由相机Pawn用于星系观察
 */
UCLASS()
class GALAXYSIMULATOR_API AGalaxyCameraPawn : public APawn
{
    GENERATED_BODY()

public:
    AGalaxyCameraPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USceneComponent *RootScene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent *CameraComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float FastMovementMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float MouseSensitivity;
};
