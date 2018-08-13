// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Tags.h"
#include "ROSBridgeHandler.h"
#include "VisualMarker.h"
#include <ROSPublisherBaseClass.h>
#include "UVisManager.generated.h"


UCLASS()
class UROSVISPACKAGE_API UVisManager : public UROSPublisherBaseClass
{
	GENERATED_BODY()

public:
	UVisManager(){};
	~UVisManager(){};

	void SetupServiceServers();

	void Init(FString DefaultNamespace) override;

	TMap<FString, AActor*> IdToActorMap;
private:
	UWorld * World;
	FString Namespace;
};
