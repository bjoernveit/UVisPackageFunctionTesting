#pragma once
#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VisualMarker.h"
#include "BasicMarkerSpawner.generated.h"


UENUM()
enum class EBasicMarkerType : uint8
{
	Point,
	Cube,
	Sphere,
	Cylinder,
	Cone,
	Arrow
};


UCLASS()
class UVISPACKAGE_API UBasicMarkerSpawner : public UObject
{
	GENERATED_BODY()

public:
	/* Make sure to provoid this class with a proper outer object, so it can call getWorld() */
	UBasicMarkerSpawner();

	AVisualMarker* SpawnVisualMarker(EBasicMarkerType Type, FVector Location, FRotator Rotation,
	                                 FColor Color = FColor::White);

	bool AddVisualToActor(AActor& Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color);

private:
	TMap<EBasicMarkerType, FString> MapTypeToMeshPath;
	UMaterialInterface* Material;
	bool AddPointVisualToActor(AActor& Actor, FVector Location, FRotator Rotation, FColor Color);
	bool AddVisualToActorInternal(AActor & Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color);

};


