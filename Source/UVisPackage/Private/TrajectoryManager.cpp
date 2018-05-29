#include "TrajectoryManager.h"
#include "ConstructorHelpers.h"


void UTrajectoryManager::CreateTrajectory(USceneComponent* ComponentToFollow, TrajectoryType Type, FColor Color,
	double TimeToFollow, double TimeUntilFade)
{
	CreateMulticolorTrajectory(ComponentToFollow, Type, Color, Color, TimeToFollow, TimeUntilFade);
}

void UTrajectoryManager::CreateMulticolorTrajectory(USceneComponent* ComponentToFollow, TrajectoryType Type,
	FColor StartColor, FColor EndColor, double TimeToFollow, double TimeUntilFade)
{
	TraceInformation TraceInf;
	TraceInf.PSC = CreateParticleComponent(Type);
	TraceInf.TotalTraceTime = TimeToFollow;
	TraceInf.TimeUntilFade = TimeUntilFade;
	TraceInf.AliveFor = 0.f;
	TraceInf.StartColor = StartColor;
	TraceInf.EndColor = EndColor;
	
	TraceInf.PSC->SetColorParameter(TEXT("Color"), StartColor); 
	TraceInf.PSC->bAutoActivate = false;
	TraceInf.PSC->AttachToComponent(ComponentToFollow, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
	ActiveTraces.Add(ComponentToFollow, TraceInf);
	TraceInf.PSC->Activate();
}

void UTrajectoryManager::Tick(float DeltaTime)
{
	for(auto Entry: ActiveTraces)
	{
		TraceInformation TraceInf = Entry.Value;
		USceneComponent* Component = Entry.Key;

		TraceInf.PSC->SetColorParameter(TEXT("Color"), GetCurrentColor(TraceInf));
		//TODO: TraceInf.PSC->Set(TEXT("Rotation"), );

		if (TraceInf.AliveFor > TraceInf.TotalTraceTime)
		{
			TraceInf.PSC->Deactivate();
			TraceInf.PSC->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			ActiveTraces.Remove(Component);
		}

	}
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

UParticleSystemComponent* UTrajectoryManager::CreateParticleComponent(TrajectoryType Type)
{

	ConstructorHelpers::FObjectFinder<UParticleSystem> PS(**TrajectoryTypeToAssetPath.Find(Type));
	UParticleSystemComponent* PSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyPSC"));
	PSC->SetTemplate(PS.Object);

	PSC->bAutoActivate = false;
	PSC->SetHiddenInGame(false);

	return PSC;
}

FColor UTrajectoryManager::GetCurrentColor(TraceInformation TraceInf)
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
