#include "DisplayTrajectoryServer.h"
#include "Ids.h"
#include "TrajectoryManager.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDisplayTrajectoryServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDisplayTrajectorySrv::Request> Request =
		MakeShareable(new FROSDisplayTrajectorySrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDisplayTrajectoryServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSDisplayTrajectorySrv::Request> DisplayTrajectoryRequest =
		StaticCastSharedPtr<FROSDisplayTrajectorySrv::Request>(Request);


	//Run on Gamethread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		TrajectoryManager = NewObject<UTrajectoryManager>(Controller);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	
	const FString Id = DisplayTrajectoryRequest->GetMarkerId().IsEmpty() ? FIds::NewGuidInBase64() : DisplayTrajectoryRequest->GetMarkerId();
	const FColor ColorBegin = DisplayTrajectoryRequest->GetColorBegin().GetColor();
	const FColor ColorEnd = DisplayTrajectoryRequest->GetColorEnd().GetColor();
	TArray<FVector> Points;
	for(auto GeometryPoint : DisplayTrajectoryRequest->GetPoints())
	{
		Points.Add(GeometryPoint.GetVector());
	}

	//AVisualMarker** VisualMarker = Controller->IdToMarkerMap.Find(Id);
	AActor** VisualMarker = Controller->IdToActorMap.Find(Id);

	if (VisualMarker == nullptr)
	{
		// Marker does not exist => create Marker
		AVisualMarker* NewMarker = TrajectoryManager->SpawnTrajectoryFromPoints(Points, ColorBegin, ColorEnd);
		//Controller->IdToMarkerMap.Add(Id, NewMarker);
		Controller->IdToActorMap.Add(Id, NewMarker);
	}
	else
	{
		//Marker does exist => Add Visualisation to the Actor
		TrajectoryManager->AddTrajectoryToActor(**VisualMarker, Points, ColorBegin, ColorEnd);		
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSDisplayTrajectorySrv::Response(Id));
}
