#pragma once
#include "CoreMinimal.h"
#include "DisplayTrajectory.h"
#include "UVisManager.h"
#include "ROSBridgeSrvServer.h"
#include "TrajectoryManager.h"

class FROSDisplayTrajectoryServer final : public FROSBridgeSrvServer
{
private:
	FROSDisplayTrajectoryServer(){};

	FThreadSafeBool ServiceSuccess;

	
	UVisManager* Controller;
	UTrajectoryManager* TrajectoryManager;

public:
	FROSDisplayTrajectoryServer(FString Namespace, FString Name,
		UVisManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_vis_msgs/DisplayTrajectory"))
	{
		Controller = InController;
	};

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};


