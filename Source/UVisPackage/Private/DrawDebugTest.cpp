// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugTest.h"
#include "DrawDebugHelpers.h"


// Sets default values
ADrawDebugTest::ADrawDebugTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrajectoryManager = NewObject<UTrajectoryManager>();
}

// Called when the game starts or when spawned
void ADrawDebugTest::BeginPlay()
{
	Super::BeginPlay();

	for (auto Actor : TracedActors)
	{
		TrajectoryManager->CreateMulticolorTrajectory(Actor->GetRootComponent(), TrajectoryType::Cylinder, FColor::Red, FColor::Green);
	}

} 

// Called every frame
void ADrawDebugTest::Tick(float DeltaTime)
{
}