#pragma once
#include "ROSBridgeSrv.h"


class UROSBRIDGE_API FROSRemoveMarkerSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSRemoveMarkerSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		FString MarkerId;


	public:
		Request() {}

		Request(FString InMarkerId)
		{
			MarkerId = InMarkerId;
		}

		FString GetMarkerId()
		{
			return MarkerId;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			MarkerId = JsonObject->GetStringField("marker_id");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return "FROSRemoveMarkerSrv:Request {marker_id = " + MarkerId + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("marker_id"), MarkerId);
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		bool Success;


	public:
		Response() {}

		Response(bool InSuccess)
		{
			Success = InSuccess;
		}

		bool GetSuccess()
		{
			return Success;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Success = JsonObject->GetBoolField("success");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return "FROSRemoveMarkerSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};