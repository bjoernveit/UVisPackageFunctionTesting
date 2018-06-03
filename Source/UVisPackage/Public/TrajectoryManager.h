#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Tickable.h"
#include "TrajectoryManager.generated.h"

UENUM()
enum class ETrajectoryType : uint8
{
	Line,
	Square,
	Sphere,
	Cylinder
};
inline uint8 GetTypeHash(const ETrajectoryType A)
{
	return (uint8)A;
}

USTRUCT()
struct FTraceInformation
{
	GENERATED_BODY()

	UPROPERTY()
		UParticleSystemComponent* PSC;
	UPROPERTY()
		FColor StartColor;
	UPROPERTY()
		FColor EndColor;
	UPROPERTY()
		double AliveFor;
	UPROPERTY()
		double TotalTraceTime;
	UPROPERTY()
		double TimeUntilFade;

	FTraceInformation()
	{
		PSC = nullptr;
		StartColor = FColor::Black;
		EndColor = FColor::Black;
		AliveFor = 0;
		TotalTraceTime = -1.f;
		TimeUntilFade = -1.f;
	}
};


UCLASS()
class UVISPACKAGE_API UTrajectoryManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UTrajectoryManager();

	void CreateTrajectory(USceneComponent* ComponentToFollow, ETrajectoryType Type, FColor Color,
		double TimeToFollow = -1.f, double TimeUntilFade = -1.f);

	void CreateMulticolorTrajectory(USceneComponent* ComponentToFollow, ETrajectoryType Type,
		FColor StartColor, FColor EndColor,
		double TimeToFollow = -1.f, double TimeUntilFade = -1.f);

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	
	UPROPERTY(EditAnywhere)
		TMap<ETrajectoryType, UParticleSystem*> TrajectoryTypeToPS;

	UPROPERTY()
		TMap<USceneComponent*, FTraceInformation> ActiveTraces;
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent * DefaultPSC;
	private:
	UParticleSystemComponent* CreateParticleComponent(ETrajectoryType Type, AActor* Owner);

	FColor GetCurrentColor(struct FTraceInformation& TraceInf);

};
