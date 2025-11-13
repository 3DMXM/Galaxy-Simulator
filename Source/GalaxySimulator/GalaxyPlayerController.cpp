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

    GalaxyManager = nullptr;
    SelectedBody = nullptr;
    MainUIWidget = nullptr;
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
    }
}

void AGalaxyPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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
            ControlledPawn->AddActorWorldOffset(Direction * Value * CameraMoveSpeed * GetWorld()->GetDeltaSeconds());
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
            ControlledPawn->AddActorWorldOffset(Direction * Value * CameraMoveSpeed * GetWorld()->GetDeltaSeconds());
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
            ControlledPawn->AddActorWorldOffset(FVector::UpVector * Value * CameraMoveSpeed * GetWorld()->GetDeltaSeconds());
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
    if (Value != 0.0f)
    {
        AddYawInput(Value * CameraRotationSpeed * GetWorld()->GetDeltaSeconds());
    }
}

void AGalaxyPlayerController::RotateCameraPitch(float Value)
{
    if (Value != 0.0f)
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
    // 右键可以用于取消选择或其他操作
    SelectCelestialBody(nullptr);
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
