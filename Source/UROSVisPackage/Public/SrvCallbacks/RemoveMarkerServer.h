#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "UVisManager.h"

class FROSRemoveMarkerServer final : public FROSBridgeSrvServer
{
private:
	FROSRemoveMarkerServer(){};
	
	FThreadSafeBool ServiceSuccess;

	UVisManager* Controller;

public:
	FROSRemoveMarkerServer(FString Namespace, FString Name, 
		UVisManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_vis_msgs/RemoveMarker"))
	{
		Controller = InController;
	};

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
