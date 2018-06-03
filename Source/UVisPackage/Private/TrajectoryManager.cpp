#include "TrajectoryManager.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"



UTrajectoryManager::UTrajectoryManager()
{
	ConstructorHelpers::FObjectFinder<UParticleSystem> PSLine(TEXT("ParticleSystem'/UVisPackage/Particles/P_Ribbon.P_Ribbon'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> PSCube(TEXT("ParticleSystem'/UVisPackage/Particles/P_Cube_Trail.P_Cube_Trail'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> PSSphere(TEXT("ParticleSystem'/UVisPackage/Particles/P_Sphere_Trail.P_Sphere_Trail'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> PSCylinder(TEXT("ParticleSystem'/UVisPackage/Particles/P_Cylinder_Trail.P_Cylinder_Trail'"));

	TrajectoryTypeToPS.Empty();
	TrajectoryTypeToPS.Add(ETrajectoryType::Line, PSLine.Object);
	TrajectoryTypeToPS.Add(ETrajectoryType::Square, PSCube.Object);
	TrajectoryTypeToPS.Add(ETrajectoryType::Sphere, PSSphere.Object);
	TrajectoryTypeToPS.Add(ETrajectoryType::Cylinder, PSCylinder.Object);
}

void UTrajectoryManager::CreateTrajectory(USceneComponent* ComponentToFollow, ETrajectoryType Type, FColor Color,
	double TimeToFollow, double TimeUntilFade)
{
	CreateMulticolorTrajectory(ComponentToFollow, Type, Color, Color, TimeToFollow, TimeUntilFade);
}

void UTrajectoryManager::CreateMulticolorTrajectory(USceneComponent* ComponentToFollow, ETrajectoryType Type,
	FColor StartColor, FColor EndColor, double TimeToFollow, double TimeUntilFade) 
{
	FTraceInformation TraceInf;
	TraceInf.PSC = CreateParticleComponent(Type, ComponentToFollow->GetOwner());
	
	TraceInf.TotalTraceTime = TimeToFollow;
	TraceInf.TimeUntilFade = TimeUntilFade;
	TraceInf.AliveFor = 0.f;
	TraceInf.StartColor = StartColor;
	TraceInf.EndColor = EndColor;

	TraceInf.PSC->SetColorParameter(TEXT("Color"), StartColor);
	TraceInf.PSC->bAutoActivate = false;	
	TraceInf.PSC->AttachToComponent(ComponentToFollow,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		ComponentToFollow->GetFName());
	TraceInf.PSC->RegisterComponent();
	TraceInf.PSC->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator::ZeroRotator);
	TraceInf.PSC->ActivateSystem();
	
	ActiveTraces.Add(ComponentToFollow, TraceInf);
}

void UTrajectoryManager::Tick(float DeltaTime)
{
	TMap<USceneComponent*, FTraceInformation> NewActiveTraces;
	for (auto Entry : ActiveTraces)
	{
		FTraceInformation TraceInf = Entry.Value;
		USceneComponent* Component = Entry.Key;
		TraceInf.AliveFor += DeltaTime;
		TraceInf.PSC->SetColorParameter(TEXT("Color"), GetCurrentColor(TraceInf));
		if (TraceInf.AliveFor > TraceInf.TotalTraceTime && TraceInf.TotalTraceTime > 0.f)
		{
			TraceInf.PSC->Deactivate();
			TraceInf.PSC->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			ActiveTraces.Remove(Component);
		} 
		else
		{
			NewActiveTraces.Add(Component, TraceInf);
		}

	}

	ActiveTraces = NewActiveTraces;
}

bool UTrajectoryManager::IsTickable() const
{
	return true;
}

bool UTrajectoryManager::IsTickableInEditor() const
{
	return false;
}

bool UTrajectoryManager::IsTickableWhenPaused() const
{
	return false;
}

TStatId UTrajectoryManager::GetStatId() const
{
	return TStatId();
}

UParticleSystemComponent* UTrajectoryManager::CreateParticleComponent(ETrajectoryType Type, AActor* Owner)
{
	UParticleSystemComponent* PSC = NewObject<UParticleSystemComponent>(Owner, TEXT("TrajectoryPSC"));
	PSC->SetTemplate(TrajectoryTypeToPS.FindChecked(Type));	
	return PSC;
}

FColor UTrajectoryManager::GetCurrentColor(FTraceInformation& TraceInf)
{
	if (TraceInf.StartColor == TraceInf.EndColor)
		return TraceInf.StartColor;

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
