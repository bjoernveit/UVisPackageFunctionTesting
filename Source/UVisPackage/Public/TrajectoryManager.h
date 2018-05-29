#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Tickable.h"
#include "TrajectoryManager.generated.h"

UENUM()
enum class TrajectoryType : uint8
{
	Line,
	Square,
	Sphere,
	Cylinder
};

USTRUCT()
struct TraceInformation
{
	GENERATED_BODY()

	UPROPERTY()
		UParticleSystemComponent* PSC;
	UPROPERTY()
		FColor StartColor;
	UPROPERTY()
		FColor EndColor;
	UPROPERTY()
		double AliveFor;
	UPROPERTY()
		double TotalTraceTime;
	UPROPERTY()
		double TimeUntilFade;
};


UCLASS()
class UVISPACKAGE_API UTrajectoryManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:



	UTrajectoryManager()
	{
		TrajectoryTypeToAssetPath.Empty();
		TrajectoryTypeToAssetPath.Add(TrajectoryType::Line, TEXT("ParticleSystem'/UVisPackage/Particles/P_Ribbon.P_Ribbon'"));
		TrajectoryTypeToAssetPath.Add(TrajectoryType::Square, TEXT("ParticleSystem'/UVisPackage/Particles/P_Cube_Trail.P_Cube_Trail'"));
		TrajectoryTypeToAssetPath.Add(TrajectoryType::Sphere, TEXT("ParticleSystem'/UVisPackage/Particles/P_Sphere_Trail.P_Sphere_Trail'"));
		TrajectoryTypeToAssetPath.Add(TrajectoryType::Cylinder, TEXT("ParticleSystem'/UVisPackage/Particles/P_Cylinder_Trail.P_Cylinder_Trail'"));
	}

	void CreateTrajectory(USceneComponent* ComponentToFollow, TrajectoryType Type, FColor Color,
		double TimeToFollow = -1.f, double TimeUntilFade = -1.f);

	void CreateMulticolorTrajectory(USceneComponent* ComponentToFollow, TrajectoryType Type,
		FColor StartColor, FColor EndColor,
		double TimeToFollow = -1.f, double TimeUntilFade = -1.f);

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

private:


	UPROPERTY()
		TMap<TrajectoryType, FString> TrajectoryTypeToAssetPath;

	UPROPERTY()
		TMap<USceneComponent*, TraceInformation> ActiveTraces;

	UParticleSystemComponent* CreateParticleComponent(TrajectoryType Type);

	FColor GetCurrentColor(TraceInformation TraceInf);

};
