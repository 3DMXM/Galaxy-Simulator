// Copyright Epic Games, Inc. All Rights Reserved.

#include "CelestialBody.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

ACelestialBody::ACelestialBody()
{
	PrimaryActorTick.bCanEverTick = true;

	// 创建网格组件
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// 加载默认球体网格
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMesh.Object);
	}

	// 默认属性
	BodyType = ECelestialBodyType::Planet;
	MaterialType = EPlanetMaterialType::Rocky;
	BodyName = TEXT("Unnamed Body");
	Mass = 1.0f; // 1个地球质量
	Radius = 6371.0f; // 地球半径
	Velocity = FVector::ZeroVector;
	ParentBody = nullptr;

	// 设置初始缩放
	SetBodyScale(Radius);
}

void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	UpdateMaterial();
}

void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACelestialBody::SetBodyScale(float NewRadius)
{
	Radius = NewRadius;
	// 将实际半径转换为虚幻引擎的缩放
	// 假设基础球体直径为100单位，我们希望1000km = 100单位
	float ScaleFactor = (NewRadius / 1000.0f) * 0.1f;
	FVector NewScale(ScaleFactor, ScaleFactor, ScaleFactor);
	MeshComponent->SetWorldScale3D(NewScale);
}

void ACelestialBody::UpdateMaterial()
{
	// 这里可以根据MaterialType设置不同的材质
	// 目前只是占位符，实际材质需要在蓝图或后续实现中设置
	if (MeshComponent)
	{
		// 可以在这里加载和应用不同的材质
		// 例如: MeshComponent->SetMaterial(0, LoadedMaterial);
	}
}

FVector ACelestialBody::CalculateGravitationalForce(ACelestialBody* OtherBody) const
{
	if (!OtherBody || OtherBody == this)
	{
		return FVector::ZeroVector;
	}

	// 计算方向向量
	FVector Direction = OtherBody->GetActorLocation() - GetActorLocation();
	float Distance = Direction.Size();

	if (Distance < 1.0f)
	{
		return FVector::ZeroVector;
	}

	Direction.Normalize();

	// F = G * (m1 * m2) / r^2
	float ForceMagnitude = GravitationalConstant * (Mass * OtherBody->Mass) / (Distance * Distance);
	
	return Direction * ForceMagnitude;
}
