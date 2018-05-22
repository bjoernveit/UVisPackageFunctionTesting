// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		FVector LocationPoint;

	UPROPERTY(EditAnywhere)
		FVector LineStart;

	UPROPERTY(EditAnywhere)
		FVector LineEnd;

	UPROPERTY(EditAnywhere)
		FVector SphereCenter;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> TracedActors;

	UPROPERTY(EditAnywhere)
		FColor Color;
private:
	struct TraceInformation
	{
		FColor StartColor;
		FColor EndColor;
		double AliveFor;
		FVector LastLocation;
		double TotalTraceTime;
		double TraceLifetime;
	};

	TMap<AActor*, TraceInformation> MapActorToTraceInformation;

	void TraceMulticolor(FColor StartColor, FColor EndColor, AActor* ActorToBeTraced, double TraceForSeconds, double TraceLifetime);

	FColor GetCurrentColor(TraceInformation TraceInf);
	void LogTraceInformation(TraceInformation TraceInf);
};
