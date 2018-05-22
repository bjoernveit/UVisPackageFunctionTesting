// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugTest.h"
#include "DrawDebugHelpers.h"


// Sets default values
ADrawDebugTest::ADrawDebugTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADrawDebugTest::BeginPlay()
{
	Super::BeginPlay();

	for (auto Actor : TracedActors)
	{
		TraceMulticolor(FColor::Red, FColor::Green, Actor, 5.f, 3.f);
	}

}

// Called every frame
void ADrawDebugTest::Tick(float DeltaTime)
{
	TMap<AActor*, TraceInformation> NewMap;

	for (auto KeyValuePair : MapActorToTraceInformation)
	{
		AActor* ActorToBeTraced = KeyValuePair.Key;
		TraceInformation TraceInf = KeyValuePair.Value;

		FVector CurrentLocation = ActorToBeTraced->GetActorLocation();

		DrawDebugLine(GetWorld(), TraceInf.LastLocation, CurrentLocation,
			GetCurrentColor(TraceInf), false, TraceInf.TraceLifetime, 0, 5.f);

		TraceInf.LastLocation = CurrentLocation;
		TraceInf.AliveFor = TraceInf.AliveFor + DeltaTime;

		LogTraceInformation(TraceInf);

		//Check if Trace is done
		if (TraceInf.AliveFor < TraceInf.TotalTraceTime)
		{
			NewMap.Add(ActorToBeTraced, TraceInf);
		}
	}

	MapActorToTraceInformation = NewMap;
}

void ADrawDebugTest::TraceMulticolor(FColor StartColor, FColor EndColor, AActor* ActorToBeTraced, double TraceForSeconds,
	double TraceLifetime)
{
	TraceInformation TraceInf;
	TraceInf.StartColor = StartColor;
	TraceInf.EndColor = EndColor;
	TraceInf.LastLocation = ActorToBeTraced->GetActorLocation();
	TraceInf.TotalTraceTime = TraceForSeconds;
	TraceInf.AliveFor = 0;
	TraceInf.TraceLifetime = TraceLifetime;

	LogTraceInformation(TraceInf);

	MapActorToTraceInformation.Add(ActorToBeTraced, TraceInf);
}

FColor ADrawDebugTest::GetCurrentColor(TraceInformation TraceInf)
{
	if (TraceInf.AliveFor < TraceInf.TotalTraceTime)
	{
		double Fraction = TraceInf.AliveFor / TraceInf.TotalTraceTime;
		uint8 NewA = TraceInf.StartColor.A + ((TraceInf.EndColor.A - TraceInf.StartColor.A) *  Fraction);
		uint8 NewR = TraceInf.StartColor.R + ((TraceInf.EndColor.R - TraceInf.StartColor.R) *  Fraction);
		uint8 NewG = TraceInf.StartColor.G + ((TraceInf.EndColor.G - TraceInf.StartColor.G) *  Fraction);
		uint8 NewB = TraceInf.StartColor.B + ((TraceInf.EndColor.B - TraceInf.StartColor.B) *  Fraction);

		return FColor(NewR, NewG, NewB, NewA);
	}
	return TraceInf.EndColor;

}

void ADrawDebugTest::LogTraceInformation(TraceInformation TraceInf)
{
	UE_LOG(LogTemp, Warning, TEXT("TraceInformation:{ StartColor: %s, EndColor: %s, LastLocation: %s, TotalTraceTime: %s, AliveFor: %s, TraceLifetime: %s}"),
		*TraceInf.StartColor.ToString(), *TraceInf.EndColor.ToString(), *TraceInf.LastLocation.ToString(),
		*FString::SanitizeFloat(TraceInf.TotalTraceTime), *FString::SanitizeFloat(TraceInf.AliveFor), *FString::SanitizeFloat(TraceInf.TraceLifetime));
}