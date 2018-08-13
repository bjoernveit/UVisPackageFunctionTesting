// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugTest.h"
#include "DrawDebugHelpers.h"


// Sets default values
ADrawDebugTest::ADrawDebugTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Color = FColor::Green;
	ColorEnd = FColor::Red;
}

// Called when the game starts or when spawned
void ADrawDebugTest::BeginPlay()
{
	Super::BeginPlay();

	TrajectoryManager = NewObject<UTrajectoryManager>(this);
	MarkerSpawner = NewObject<UBasicMarkerSpawner>(this);
	PropVisualizer = NewObject<UProbabilityFunctPointVisualizer>(this);
	PropVisualizer->World = GetWorld();
	//MarkerSpawner->SpawnMarker(EBasicMarkerType::Arrow, FVector(0, 0, 200), FRotator(0, 0, 180), Color);

	TArray<FVector> GPoints = GenerateGaussionPoints(128 * 128, 0, 8);
	PropVisualizer->VisualizeProbabilityFunctionPoints(GPoints, FVector(0, 0, 0), FVector(0, 0, 1), MaxVal, FColor::Blue, FColor::Red);

}

// Called every frame
void ADrawDebugTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!(AliveFor > 100))
	{
		AliveFor += 1;
		Points.Add(TracedActors.HeapTop()->GetActorLocation());
		//DrawInterface.DrawPoint(TracedActors.HeapTop()->GetActorLocation(), FColor::Magenta, 5, 1);
		if (AliveFor == 100)
		{
			//Points = GenerateGaussionPoints(16 * 16, 0, 8);
			TrajectoryManager->SpawnTrajectoryFromPoints(Points, Color, ColorEnd);
		}
	}
}

TArray<FVector> ADrawDebugTest::GenerateGaussionPoints(int NumberOfPoints, double Mu, double Sig)
{
	TArray<FVector> GaussionPoints;
	MaxVal = 0;
	int Steps = FMath::Sqrt(NumberOfPoints);
	int f = 1;
	for(int x = -(f*Steps); x < (f*Steps);  x = x + f*2)
	{
		double XG = Gaussion(x, Mu, Sig);
		for(int y = -(f * Steps); y < (f * Steps);  y = y + f*2)
		{ 
			double YG = Gaussion(y, Mu, Sig);
			int Z = (XG + YG) * 10;
			GaussionPoints.Add(FVector(x, y, Z));
			MaxVal = MaxVal < Z ? Z : MaxVal;
		}
	}

	return GaussionPoints;

}

double ADrawDebugTest::Gaussion(double X, double Mu, double Sig)
{
	return FMath::Exp(-FMath::Pow((X - Mu), 2.0) / (2.0 * FMath::Pow(Sig, 2.0)));
}


