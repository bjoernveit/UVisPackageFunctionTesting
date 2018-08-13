#include "TrajectoryManager.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GPUPointCloudRendererComponent.h"
#include "Engine/World.h"
#include "StaticHelpers.h"


UTrajectoryManager::UTrajectoryManager()
{
}


AVisualMarker* UTrajectoryManager::SpawnTrajectoryFromPoints(TArray<FVector>& Points, FColor Color)
{
	return SpawnTrajectoryFromPoints(Points, Color, Color);
}

AVisualMarker* UTrajectoryManager::SpawnTrajectoryFromPoints(TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd)
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: This object needs to have  an outer object, that GetWorld() can be successfully be called on."), *FString(__FUNCTION__));
		return nullptr;
	}

	AVisualMarker* TrajectoryActor;
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.OverrideLevel = GetWorld()->GetCurrentLevel();

	if (IsInGameThread())
	{
		TrajectoryActor = GetWorld()->SpawnActor<AVisualMarker>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	else
	{

		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			// Setup BaseActor and PCRComponent
			TrajectoryActor = GetWorld()->SpawnActor<AVisualMarker>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	}

	return AddTrajectoryToActor(*TrajectoryActor, Points, ColorBegin, ColorEnd) ? TrajectoryActor : nullptr;
}

bool UTrajectoryManager::AddTrajectoryToActor(AActor& Marker, TArray<FVector>& Points, FColor Color)
{
	return AddTrajectoryToActor(Marker, Points, Color, Color);
}


bool UTrajectoryManager::AddTrajectoryToActor(AActor& Actor, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd)
{

	if (IsInGameThread())
	{
		return AddTrajectoryToActorInternal(Actor, Points, ColorBegin, ColorEnd);
	}
	else
	{
		bool bSuccess;
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			bSuccess = AddTrajectoryToActorInternal(Actor, Points, ColorBegin, ColorEnd);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		return bSuccess;
	}
}



bool UTrajectoryManager::AddTrajectoryToActorInternal(AActor& Actor, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd)
{

	UGPUPointCloudRendererComponent* PointCloudRendererComponent = NewObject<UGPUPointCloudRendererComponent>(&Actor);
	
	// Prepare point data
	// This is a potential bottleneck on the gamethread, but switching back and forth,
	// caused some unexpected nullpointer problems.
	int i = 0;
	for (auto Point : Points)
	{
		i++;
		PointsAsColor.Add(FLinearColor(Point.Z, Point.X, Point.Y, Point.Z));
		double Fraction = i / static_cast<double>(Points.Num());
		ColorOfPoint.Add(StaticHelpers::GetInBetweenColor(ColorBegin, ColorEnd, Fraction));
	}
	
	// Pass data to PCR
	PointCloudRendererComponent->SetDynamicProperties(1, 1, 10);

	PointCloudRendererComponent->SetInputAndConvert1(PointsAsColor, ColorOfPoint);

	//Attach and register
	PointCloudRendererComponent->bTickInEditor = true;
	PointCloudRendererComponent->SetupAttachment(Actor.GetRootComponent());
	PointCloudRendererComponent->RegisterComponent();
	return true;
}

