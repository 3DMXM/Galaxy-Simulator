// Copyright Epic Games, Inc. All Rights Reserved.

#include "GalaxyCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

AGalaxyCameraPawn::AGalaxyCameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // 创建根组件
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // 创建相机组件
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    // 默认参数
    MovementSpeed = 2000.0f;
    FastMovementMultiplier = 3.0f;
    MouseSensitivity = 1.0f;
}

void AGalaxyCameraPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AGalaxyCameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGalaxyCameraPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
