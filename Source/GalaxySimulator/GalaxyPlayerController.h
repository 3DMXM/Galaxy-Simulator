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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float FastMoveMultiplier; // Shift加速倍率

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float OrbitRotationSpeed; // 围绕目标旋转速度

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float OrbitDistance; // 围绕目标的距离

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    bool bOrbitMode; // 是否处于轨道相机模式

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
    void OnRightMouseReleased();
    void OnMiddleMousePressed();
    void OnMiddleMouseReleased();
    void OnShiftPressed();
    void OnShiftReleased();

    FVector2D GetMousePositionOnScreen() const;
    ACelestialBody *TraceForCelestialBody();

    // 相机状态
    bool bIsRightMouseDown;
    bool bIsMiddleMouseDown;
    bool bIsShiftDown;
    FVector2D LastMousePosition;
    FVector OrbitPivotPoint; // 轨道旋转中心点
};
