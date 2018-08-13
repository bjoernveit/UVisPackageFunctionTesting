#include "RemoveMarkerServer.h"
#include "RemoveMarker.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSRemoveMarkerServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSRemoveMarkerSrv::Request> Request =
		MakeShareable(new FROSRemoveMarkerSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSRemoveMarkerServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSRemoveMarkerSrv::Request> RemoveRequest =
		StaticCastSharedPtr<FROSRemoveMarkerSrv::Request>(Request);

	AActor* VisualMarkerActor;

	bool bSuccess = false;
	//if(Controller->IdToMarkerMap.RemoveAndCopyValue(RemoveRequest->GetMarkerId(), VisualMarkerActor))
	if (Controller->IdToActorMap.RemoveAndCopyValue(RemoveRequest->GetMarkerId(), VisualMarkerActor))
	{
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			bSuccess = VisualMarkerActor->Destroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);


		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		
	}

	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSRemoveMarkerSrv::Response(bSuccess));
}
