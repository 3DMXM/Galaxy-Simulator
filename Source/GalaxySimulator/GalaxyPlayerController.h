// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GalaxyPlayerController.generated.h"

/**
 * 星系模拟器玩家控制器
 */
UCLASS()
class GALAXYSIMULATOR_API AGalaxyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AGalaxyPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 引用
    UPROPERTY(BlueprintReadWrite, Category = "Galaxy")
    class AGalaxyManager *GalaxyManager;

    UPROPERTY(BlueprintReadWrite, Category = "Galaxy")
    class ACelestialBody *SelectedBody;

    // 相机控制
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraMoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraRotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraZoomSpeed;

    // UI相关
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> MainUIWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget *MainUIWidget;

    // 创建天体功能
    UFUNCTION(BlueprintCallable, Category = "Galaxy Controller")
    void CreateNewCelestialBody(TSubclassOf<ACelestialBody> BodyClass, FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Galaxy Controller")
    void DeleteSelectedBody();

    UFUNCTION(BlueprintCallable, Category = "Galaxy Controller")
    void SelectCelestialBody(ACelestialBody *Body);

    UFUNCTION(BlueprintCallable, Category = "Galaxy Controller")
    void UpdateSelectedBodyProperties(float NewMass, float NewRadius, EPlanetMaterialType NewMaterialType);

    UFUNCTION(BlueprintCallable, Category = "Galaxy Controller")
    ACelestialBody *GetSelectedBody() const { return SelectedBody; }

private:
    // 输入处理
    void MoveForward(float Value);
    void MoveRight(float Value);
    void MoveUp(float Value);
    void ZoomCamera(float Value);
    void RotateCameraYaw(float Value);
    void RotateCameraPitch(float Value);

    void OnMouseClick();
    void OnRightMouseClick();

    FVector2D GetMousePositionOnScreen() const;
    ACelestialBody *TraceForCelestialBody();
};
