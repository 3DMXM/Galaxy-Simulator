// Copyright Epic Games, Inc. All Rights Reserved.

#include "GalaxyPlayerController.h"
#include "GalaxyManager.h"
#include "CelestialBody.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AGalaxyPlayerController::AGalaxyPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    CameraMoveSpeed = 1000.0f;
    CameraRotationSpeed = 50.0f;
    CameraZoomSpeed = 500.0f;
    FastMoveMultiplier = 3.0f;
    OrbitRotationSpeed = 100.0f;
    OrbitDistance = 5000.0f;
    bOrbitMode = false;

    GalaxyManager = nullptr;
    SelectedBody = nullptr;
    MainUIWidget = nullptr;

    // 初始化鼠标状态
    bIsRightMouseDown = false;
    bIsMiddleMouseDown = false;
    bIsShiftDown = false;
    LastMousePosition = FVector2D::ZeroVector;
    OrbitPivotPoint = FVector::ZeroVector;
}

void AGalaxyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 查找GalaxyManager
    TArray<AActor *> FoundManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGalaxyManager::StaticClass(), FoundManagers);

    if (FoundManagers.Num() > 0)
    {
        GalaxyManager = Cast<AGalaxyManager>(FoundManagers[0]);
    }
    else
    {
        // 如果不存在，创建一个
        GalaxyManager = GetWorld()->SpawnActor<AGalaxyManager>();
    }

    // 创建UI
    if (MainUIWidgetClass)
    {
        MainUIWidget = CreateWidget<UUserWidget>(this, MainUIWidgetClass);
        if (MainUIWidget)
        {
            MainUIWidget->AddToViewport();
        }
    }

    // 设置输入模式
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);
}

void AGalaxyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        // 相机移动
        InputComponent->BindAxis("MoveForward", this, &AGalaxyPlayerController::MoveForward);
        InputComponent->BindAxis("MoveRight", this, &AGalaxyPlayerController::MoveRight);
        InputComponent->BindAxis("MoveUp", this, &AGalaxyPlayerController::MoveUp);

        // 相机旋转
        InputComponent->BindAxis("CameraYaw", this, &AGalaxyPlayerController::RotateCameraYaw);
        InputComponent->BindAxis("CameraPitch", this, &AGalaxyPlayerController::RotateCameraPitch);

        // 缩放
        InputComponent->BindAxis("ZoomCamera", this, &AGalaxyPlayerController::ZoomCamera);

        // 鼠标点击
        InputComponent->BindAction("MouseClick", IE_Pressed, this, &AGalaxyPlayerController::OnMouseClick);
        InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AGalaxyPlayerController::OnRightMouseClick);
        InputComponent->BindAction("RightMouseClick", IE_Released, this, &AGalaxyPlayerController::OnRightMouseReleased);
        InputComponent->BindAction("MiddleMouseClick", IE_Pressed, this, &AGalaxyPlayerController::OnMiddleMousePressed);
        InputComponent->BindAction("MiddleMouseClick", IE_Released, this, &AGalaxyPlayerController::OnMiddleMouseReleased);
        InputComponent->BindAction("SpeedBoost", IE_Pressed, this, &AGalaxyPlayerController::OnShiftPressed);
        InputComponent->BindAction("SpeedBoost", IE_Released, this, &AGalaxyPlayerController::OnShiftReleased);
    }
}

void AGalaxyPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 处理中键拖动围绕选中物体旋转
    if (bIsMiddleMouseDown && SelectedBody)
    {
        float MouseX, MouseY;
        GetInputMouseDelta(MouseX, MouseY);

        if (FMath::Abs(MouseX) > 0.01f || FMath::Abs(MouseY) > 0.01f)
        {
            APawn *ControlledPawn = GetPawn();
            if (ControlledPawn)
            {
                // 获取选中物体的位置作为旋转中心
                FVector TargetLocation = SelectedBody->GetActorLocation();
                FVector CameraLocation = ControlledPawn->GetActorLocation();

                // 计算相机到目标的距离
                float Distance = FVector::Dist(CameraLocation, TargetLocation);
                OrbitDistance = Distance;

                // 计算旋转
                FRotator CurrentRotation = GetControlRotation();
                CurrentRotation.Yaw += MouseX * OrbitRotationSpeed * DeltaTime;
                CurrentRotation.Pitch += MouseY * OrbitRotationSpeed * DeltaTime;
                CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch, -89.0f, 89.0f);

                // 应用旋转
                SetControlRotation(CurrentRotation);

                // 计算新的相机位置（围绕目标旋转）
                FVector NewLocation = TargetLocation - CurrentRotation.Vector() * Distance;
                ControlledPawn->SetActorLocation(NewLocation);
            }
        }
    }
}

void AGalaxyPlayerController::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        FVector Direction = GetControlRotation().Vector();
        Direction.Z = 0;
        Direction.Normalize();

        APawn *ControlledPawn = GetPawn();
        if (ControlledPawn)
        {
            float Speed = CameraMoveSpeed * (bIsShiftDown ? FastMoveMultiplier : 1.0f);
            ControlledPawn->AddActorWorldOffset(Direction * Value * Speed * GetWorld()->GetDeltaSeconds());
        }
    }
}

void AGalaxyPlayerController::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);

        APawn *ControlledPawn = GetPawn();
        if (ControlledPawn)
        {
            float Speed = CameraMoveSpeed * (bIsShiftDown ? FastMoveMultiplier : 1.0f);
            ControlledPawn->AddActorWorldOffset(Direction * Value * Speed * GetWorld()->GetDeltaSeconds());
        }
    }
}

void AGalaxyPlayerController::MoveUp(float Value)
{
    if (Value != 0.0f)
    {
        APawn *ControlledPawn = GetPawn();
        if (ControlledPawn)
        {
            float Speed = CameraMoveSpeed * (bIsShiftDown ? FastMoveMultiplier : 1.0f);
            ControlledPawn->AddActorWorldOffset(FVector::UpVector * Value * Speed * GetWorld()->GetDeltaSeconds());
        }
    }
}

void AGalaxyPlayerController::ZoomCamera(float Value)
{
    if (Value != 0.0f)
    {
        APawn *ControlledPawn = GetPawn();
        if (ControlledPawn)
        {
            FVector Direction = GetControlRotation().Vector();
            ControlledPawn->AddActorWorldOffset(Direction * Value * CameraZoomSpeed * GetWorld()->GetDeltaSeconds());
        }
    }
}

void AGalaxyPlayerController::RotateCameraYaw(float Value)
{
    if (Value != 0.0f && bIsRightMouseDown)
    {
        AddYawInput(Value * CameraRotationSpeed * GetWorld()->GetDeltaSeconds());
    }
}

void AGalaxyPlayerController::RotateCameraPitch(float Value)
{
    if (Value != 0.0f && bIsRightMouseDown)
    {
        AddPitchInput(Value * CameraRotationSpeed * GetWorld()->GetDeltaSeconds());
    }
}

void AGalaxyPlayerController::OnMouseClick()
{
    ACelestialBody *ClickedBody = TraceForCelestialBody();
    if (ClickedBody)
    {
        SelectCelestialBody(ClickedBody);
    }
}

void AGalaxyPlayerController::OnRightMouseClick()
{
    bIsRightMouseDown = true;
}

void AGalaxyPlayerController::OnRightMouseReleased()
{
    bIsRightMouseDown = false;
}

void AGalaxyPlayerController::OnMiddleMousePressed()
{
    bIsMiddleMouseDown = true;

    // 如果有选中物体，进入轨道模式
    if (SelectedBody)
    {
        bOrbitMode = true;
        OrbitPivotPoint = SelectedBody->GetActorLocation();
    }
}

void AGalaxyPlayerController::OnMiddleMouseReleased()
{
    bIsMiddleMouseDown = false;
    bOrbitMode = false;
}

void AGalaxyPlayerController::OnShiftPressed()
{
    bIsShiftDown = true;
}

void AGalaxyPlayerController::OnShiftReleased()
{
    bIsShiftDown = false;
}

ACelestialBody *AGalaxyPlayerController::TraceForCelestialBody()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        return Cast<ACelestialBody>(HitResult.GetActor());
    }

    return nullptr;
}

void AGalaxyPlayerController::CreateNewCelestialBody(TSubclassOf<ACelestialBody> BodyClass, FVector Location)
{
    if (GalaxyManager && BodyClass)
    {
        ACelestialBody *NewBody = GalaxyManager->SpawnCelestialBody(BodyClass, Location, FRotator::ZeroRotator);
        if (NewBody)
        {
            SelectCelestialBody(NewBody);
        }
    }
}

void AGalaxyPlayerController::DeleteSelectedBody()
{
    if (GalaxyManager && SelectedBody)
    {
        GalaxyManager->RemoveCelestialBody(SelectedBody);
        SelectedBody = nullptr;
    }
}

void AGalaxyPlayerController::SelectCelestialBody(ACelestialBody *Body)
{
    SelectedBody = Body;

    // 可以在这里触发UI更新等事件
    if (SelectedBody)
    {
        UE_LOG(LogTemp, Log, TEXT("Selected: %s"), *SelectedBody->BodyName);
    }
}

void AGalaxyPlayerController::UpdateSelectedBodyProperties(float NewMass, float NewRadius, EPlanetMaterialType NewMaterialType)
{
    if (SelectedBody)
    {
        SelectedBody->Mass = NewMass;
        SelectedBody->MaterialType = NewMaterialType;
        SelectedBody->SetBodyScale(NewRadius);
        SelectedBody->UpdateMaterial();
    }
}
