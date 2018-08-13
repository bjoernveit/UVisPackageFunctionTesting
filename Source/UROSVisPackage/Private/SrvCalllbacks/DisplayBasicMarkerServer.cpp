#include "DisplayBasicMarkerServer.h"
#include "BasicMarkerSpawner.h"
#include "Ids.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDisplayBasicMarkerServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDisplayBasicMarkerSrv::Request> Request =
		MakeShareable(new FROSDisplayBasicMarkerSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDisplayBasicMarkerServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSDisplayBasicMarkerSrv::Request> DisplayMarkerRequest =
		StaticCastSharedPtr<FROSDisplayBasicMarkerSrv::Request>(Request);

	UBasicMarkerSpawner* MarkerSpawner;

	//Run on Gamethread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		MarkerSpawner = NewObject<UBasicMarkerSpawner>(Controller);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);


	const FString Id = DisplayMarkerRequest->GetMarkerId().IsEmpty() ? FIds::NewGuidInBase64() : DisplayMarkerRequest->GetMarkerId();

	//AVisualMarker** VisualMarker = Controller->IdToMarkerMap.Find(Id);
	AActor** VisualMarker = Controller->IdToActorMap.Find(Id);

	const auto MarkerType = EBasicMarkerType(DisplayMarkerRequest->GetMarkerType());
	const FVector Location = DisplayMarkerRequest->GetPose().GetPosition().GetVector();
	const FRotator Rotation = DisplayMarkerRequest->GetPose().GetOrientation().GetQuat().Rotator();
	const FColor Color = DisplayMarkerRequest->GetColor().GetColor();

	if (VisualMarker == nullptr)
	{
		// Marker does not exist => create Marker
		AVisualMarker* NewMarker = MarkerSpawner->SpawnVisualMarker(MarkerType, Location, Rotation, Color);


		//Controller->IdToMarkerMap.Add(Id, NewMarker);
		Controller->IdToActorMap.Add(Id, NewMarker);
	}
	else
	{
		MarkerSpawner->AddVisualToActor(**VisualMarker, MarkerType, Location, Rotation, Color);
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSDisplayBasicMarkerSrv::Response(Id));
}
