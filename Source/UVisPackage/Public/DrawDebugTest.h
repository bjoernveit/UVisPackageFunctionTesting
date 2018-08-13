#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "TrajectoryManager.h"
#include "BasicMarkerSpawner.h"
#include "ProbabilityFunctPointVisualizer.h"
#include "DrawDebugTest.generated.h"


UCLASS()
class UVISPACKAGE_API ADrawDebugTest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADrawDebugTest();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float AliveFor;

	UTrajectoryManager* TrajectoryManager;
	UBasicMarkerSpawner* MarkerSpawner;
	UProbabilityFunctPointVisualizer* PropVisualizer;
	double MaxVal;

	UPROPERTY(EditAnywhere)
		TArray<FVector> Points;
	UPROPERTY(EditAnywhere)
		FColor Color; 
	UPROPERTY(EditAnywhere)
		FColor ColorEnd;
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> TracedActors;

private:
	TArray<FVector> GenerateGaussionPoints(int NumberOfPoints, double Mu, double Sig);

	static double Gaussion(double X, double Mu, double Sig);
};
