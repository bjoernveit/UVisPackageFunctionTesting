#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VisualMarker.h"
#include "TrajectoryManager.generated.h"

UENUM()
enum class ETrajectoryType : uint8
{
	Line,
	Square,
	Sphere,
	Cylinder
};

UCLASS()
class UVISPACKAGE_API UTrajectoryManager : public UObject
{
	GENERATED_BODY()

public:

	/* Make sure to provoid this class with a proper outer object, so it can call getWorld() */
	UTrajectoryManager();
	~UTrajectoryManager() {};

	AVisualMarker* SpawnTrajectoryFromPoints(TArray<FVector>& Points, FColor Color);

	AVisualMarker* SpawnTrajectoryFromPoints(TArray<FVector>& Points, FColor ColorStart, FColor ColorEnd);
	bool AddTrajectoryToActor(AActor& Actor, TArray<FVector>& Points, FColor Color);
	bool AddTrajectoryToActor(AActor& Actor, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd);

	
	UPROPERTY()
		TArray<FLinearColor> PointsAsColor;
	UPROPERTY()
		TArray<FColor> ColorOfPoint;

private:

	bool AddTrajectoryToActorInternal(AActor& Actor, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd);
};
