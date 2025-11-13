// Copyright Epic Games, Inc. All Rights Reserved.

#include "GalaxyGameMode.h"
#include "GalaxyPlayerController.h"
#include "GalaxyCameraPawn.h"

AGalaxyGameMode::AGalaxyGameMode()
{
    // 设置默认Pawn和控制器类
    DefaultPawnClass = AGalaxyCameraPawn::StaticClass();
    PlayerControllerClass = AGalaxyPlayerController::StaticClass();
}
