#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TrajectoryManager.h"
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
	UTrajectoryManager* TrajectoryManager;
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> TracedActors;

};
