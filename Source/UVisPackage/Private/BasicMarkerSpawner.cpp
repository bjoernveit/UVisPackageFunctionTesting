#include "BasicMarkerSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "VisualMarker.h"
#include "GPUPointCloudRendererComponent.h"


UBasicMarkerSpawner::UBasicMarkerSpawner()
{
	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/UVisPackage/Meshes/BasicMarkerMeshes/BasicShapeMaterial.BasicShapeMaterial'")));

	MapTypeToMeshPath.Add(EBasicMarkerType::Cube, "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Cube.SM_Cube'");
	MapTypeToMeshPath.Add(EBasicMarkerType::Sphere, "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Sphere.SM_Sphere'");
	MapTypeToMeshPath.Add(EBasicMarkerType::Cylinder, "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Cylinder.SM_Cylinder'");
	MapTypeToMeshPath.Add(EBasicMarkerType::Cone, "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Cone.SM_Cone'");
	MapTypeToMeshPath.Add(EBasicMarkerType::Arrow, "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Arrow.SM_Arrow'");
}

AVisualMarker* UBasicMarkerSpawner::SpawnVisualMarker(EBasicMarkerType Type, FVector Location, FRotator Rotation,
	FColor Color)
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: This object needs to have  an outer object, that GetWorld() can be successfully be called on."), *FString(__FUNCTION__));
		return nullptr;
	}

	AVisualMarker* BaseActor;
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	if (IsInGameThread())
	{
		// Spawn Marker Base Actor
		//BaseActor = World->SpawnActor<AVisualMarker>(SpawnParams);
		BaseActor = World->SpawnActor<AVisualMarker>();
		BaseActor->AddActorWorldTransform(FTransform(Rotation, Location));
	}
	else
	{
		// Run on Gamethread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			// Spawn Marker Base Actor
			BaseActor = GetWorld()->SpawnActor<AVisualMarker>(SpawnParams);
			BaseActor->AddActorWorldTransform(FTransform(Rotation, Location));

		}, TStatId(), nullptr, ENamedThreads::GameThread);

		// wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	}

	if (AddVisualToActor(*BaseActor, Type, Location, Rotation, Color))
		return BaseActor;

	BaseActor->Destroy();
	return nullptr;

}

bool UBasicMarkerSpawner::AddVisualToActor(AActor& Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color)
{
	//Make sure actual Code is run on Gamethread
	if (IsInGameThread())
	{
		return AddVisualToActorInternal(Actor, Type, Location, Rotation, Color);
	}
	else
	{
		bool bSuccess;
		//Run on Gamethread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			bSuccess = AddVisualToActorInternal(Actor, Type, Location, Rotation, Color);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		return bSuccess;
	}

}


bool UBasicMarkerSpawner::AddVisualToActorInternal(AActor& Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color)
{

	if (Type == EBasicMarkerType::Point)
	{
		return AddPointVisualToActor(Actor, Location, Rotation, Color);
	}

	UStaticMeshComponent* MeshComponent;

	// Attach StaticMeshComponent
	MeshComponent = NewObject<UStaticMeshComponent>(&Actor);
	MeshComponent->AttachToComponent(Actor.GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	// Asign Mesh to Component
	UStaticMesh* Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, **MapTypeToMeshPath.Find(Type)));
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Basic Mesh could not be loaded from Path: %s"), *FString(__FUNCTION__), **MapTypeToMeshPath.Find(Type));
		return false;
	}
	MeshComponent->SetStaticMesh(Mesh);

	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
	DynMaterial->SetVectorParameterValue("Color", Color);
	MeshComponent->SetMaterial(0, DynMaterial);
	MeshComponent->SetWorldLocationAndRotation(Location, Rotation);
	MeshComponent->RegisterComponent();
	return true;
}


bool UBasicMarkerSpawner::AddPointVisualToActor(AActor& Actor, FVector Location, FRotator Rotation, FColor Color)
{
	UGPUPointCloudRendererComponent* PointCloudRendererComponent = NewObject<UGPUPointCloudRendererComponent>(&Actor);

	// Prepare point data
	TArray<FLinearColor> Points;
	TArray<FColor> Colors;
	Points.Add(FLinearColor(Location.Z, Location.X, Location.Y, Location.Z));
	Colors.Add(Color);

	// Pass data to PCR
	PointCloudRendererComponent->SetDynamicProperties(1, 1, 1);

	PointCloudRendererComponent->SetInputAndConvert1(Points, Colors);

	//Attach and register
	PointCloudRendererComponent->bTickInEditor = true;
	PointCloudRendererComponent->SetupAttachment(Actor.GetRootComponent());
	PointCloudRendererComponent->RegisterComponent();
	return true;
}
