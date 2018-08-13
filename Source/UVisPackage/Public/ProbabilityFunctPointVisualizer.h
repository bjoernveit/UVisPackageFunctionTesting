#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ProbabilityFunctPointVisualizer.generated.h"


UCLASS()
class UVISPACKAGE_API UProbabilityFunctPointVisualizer : public UObject
{
	GENERATED_BODY()

public:
	UProbabilityFunctPointVisualizer();
	~UProbabilityFunctPointVisualizer();

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
		TArray<FLinearColor> PointsAsColor;
	UPROPERTY()
		TArray<FColor> ColorOfPoint;


	bool VisualizeProbabilityFunctionPoints(TArray<FVector>& Points, FVector PlaneBase, FVector PlaneNormal, double MaxDistance,
		FColor ColorTop = FColor::Red, FColor ColorBottom = FColor::Blue);
};
