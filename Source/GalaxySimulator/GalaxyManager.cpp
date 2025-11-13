// Copyright Epic Games, Inc. All Rights Reserved.

#include "GalaxyManager.h"
#include "CelestialBody.h"
#include "Star.h"
#include "OrbitComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AGalaxyManager::AGalaxyManager()
{
    PrimaryActorTick.bCanEverTick = true;

    bEnableGravitySimulation = false; // 默认使用轨道系统而非物理模拟
    SimulationSpeed = 1.0f;
    bPauseSimulation = false;
}

void AGalaxyManager::BeginPlay()
{
    Super::BeginPlay();

    // 查找世界中已存在的天体
    TArray<AActor *> FoundBodies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), FoundBodies);

    for (AActor *Actor : FoundBodies)
    {
        ACelestialBody *Body = Cast<ACelestialBody>(Actor);
        if (Body)
        {
            RegisterCelestialBody(Body);
        }
    }
}

void AGalaxyManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bPauseSimulation)
    {
        return;
    }

    if (bEnableGravitySimulation)
    {
        UpdateGravity(DeltaTime * SimulationSpeed);
    }
}

ACelestialBody *AGalaxyManager::SpawnCelestialBody(TSubclassOf<ACelestialBody> BodyClass, FVector Location, FRotator Rotation)
{
    if (!BodyClass || !GetWorld())
    {
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ACelestialBody *NewBody = GetWorld()->SpawnActor<ACelestialBody>(BodyClass, Location, Rotation, SpawnParams);

    if (NewBody)
    {
        RegisterCelestialBody(NewBody);
    }

    return NewBody;
}

void AGalaxyManager::RemoveCelestialBody(ACelestialBody *Body)
{
    if (Body)
    {
        CelestialBodies.Remove(Body);
        Body->Destroy();
    }
}

void AGalaxyManager::RegisterCelestialBody(ACelestialBody *Body)
{
    if (Body && !CelestialBodies.Contains(Body))
    {
        CelestialBodies.Add(Body);
    }
}

void AGalaxyManager::UpdateGravity(float DeltaTime)
{
    ApplyGravitationalForces(DeltaTime);
}

void AGalaxyManager::ApplyGravitationalForces(float DeltaTime)
{
    // N体引力模拟
    for (int32 i = 0; i < CelestialBodies.Num(); i++)
    {
        ACelestialBody *Body1 = CelestialBodies[i];
        if (!Body1)
            continue;

        FVector TotalForce = FVector::ZeroVector;

        // 计算所有其他天体对当前天体的引力
        for (int32 j = 0; j < CelestialBodies.Num(); j++)
        {
            if (i == j)
                continue;

            ACelestialBody *Body2 = CelestialBodies[j];
            if (!Body2)
                continue;

            FVector Force = Body1->CalculateGravitationalForce(Body2);
            TotalForce += Force;
        }

        // 应用力：F = ma, a = F/m
        FVector Acceleration = TotalForce / FMath::Max(Body1->Mass, 0.001f);
        Body1->Velocity += Acceleration * DeltaTime;

        // 更新位置
        FVector NewLocation = Body1->GetActorLocation() + Body1->Velocity * DeltaTime;
        Body1->SetActorLocation(NewLocation);
    }
}

void AGalaxyManager::ClearGalaxy()
{
    for (ACelestialBody *Body : CelestialBodies)
    {
        if (Body)
        {
            Body->Destroy();
        }
    }
    CelestialBodies.Empty();
}

void AGalaxyManager::CreateSolarSystem()
{
    // 创建一个简单的太阳系示例
    if (!GetWorld())
        return;

    // 创建太阳
    AStar *Sun = Cast<AStar>(SpawnCelestialBody(AStar::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator));
    if (Sun)
    {
        Sun->BodyName = TEXT("太阳");
        Sun->Mass = 333000.0f;
        Sun->Radius = 696000.0f;
        Sun->SetBodyScale(Sun->Radius);
    }

    // 创建地球
    ACelestialBody *Earth = SpawnCelestialBody(ACelestialBody::StaticClass(), FVector(14960.0f, 0, 0), FRotator::ZeroRotator);
    if (Earth)
    {
        Earth->BodyName = TEXT("地球");
        Earth->Mass = 1.0f;
        Earth->Radius = 6371.0f;
        Earth->ParentBody = Sun;
        Earth->MaterialType = EPlanetMaterialType::Ocean;
        Earth->SetBodyScale(Earth->Radius);

        // 添加轨道组件
        UOrbitComponent *OrbitComp = NewObject<UOrbitComponent>(Earth);
        if (OrbitComp)
        {
            OrbitComp->RegisterComponent();
            OrbitComp->SemiMajorAxis = 149600.0f;
            OrbitComp->Eccentricity = 0.0167f;
            OrbitComp->OrbitalPeriod = 365.25f;
            OrbitComp->TimeScale = 100.0f;
        }
    }
}
