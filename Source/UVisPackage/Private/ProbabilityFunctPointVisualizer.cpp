// Fill out your copyright notice in the Description page of Project Settings.

#include "ProbabilityFunctPointVisualizer.h"
#include "GPUPointCloudRendererComponent.h"
#include "Components/SceneComponent.h"
#include "StaticHelpers.h"
#include "VisualMarker.h"

UProbabilityFunctPointVisualizer::UProbabilityFunctPointVisualizer()
{
}

UProbabilityFunctPointVisualizer::~UProbabilityFunctPointVisualizer()
{
}

bool UProbabilityFunctPointVisualizer::VisualizeProbabilityFunctionPoints(TArray<FVector>& Points, FVector PlaneBase,
	FVector PlaneNormal, double MaxDistance, FColor ColorTop, FColor ColorBottom)
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: World needs to be set frist"), *FString(__FUNCTION__));
		return false;
	}


	// Setup BaseActor and PCRComponent
	auto VisualMarker = World->SpawnActor<AVisualMarker>(FVector::ZeroVector, FRotator::ZeroRotator);
	auto PointCloudRendererComponent = NewObject<UGPUPointCloudRendererComponent>(VisualMarker);


	// Setup point data

	for (auto Point : Points)
	{
		float Distance = FVector::PointPlaneDist(Point, PlaneBase, PlaneNormal);

		if (Distance > MaxDistance)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]: Point %s, had higher distance then Maxdistance"), *FString(__FUNCTION__), *Point.ToString());
			return false;
		}

		PointsAsColor.Add(FLinearColor(Point.Z, Point.X, Point.Y, Point.Z));
		ColorOfPoint.Add(StaticHelpers::GetInBetweenColor(ColorBottom, ColorTop, Distance / MaxDistance));
	}

	// Pass data to PCR
	PointCloudRendererComponent->SetDynamicProperties(1, 1, 10);
	PointCloudRendererComponent->SetInputAndConvert1(PointsAsColor, ColorOfPoint);

	//Attach and register
	PointCloudRendererComponent->SetupAttachment(VisualMarker->GetRootComponent());
	PointCloudRendererComponent->RegisterComponent();
	return true;

}
