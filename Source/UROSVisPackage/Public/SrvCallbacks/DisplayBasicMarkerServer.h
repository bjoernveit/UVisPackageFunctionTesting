#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "DisplayBasicMarker.h"
#include "UVisManager.h"


class FROSDisplayBasicMarkerServer final : public FROSBridgeSrvServer
{
private:
	FROSDisplayBasicMarkerServer(){};
	
	FThreadSafeBool ServiceSuccess;

	UVisManager* Controller;
	
public:

	FROSDisplayBasicMarkerServer(FString Namespace, FString Name,
		UVisManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_vis_msgs/DisplayBasicMarker"))
	{
		Controller = InController;
	};

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};

