#pragma once
#include "ROSBridgeSrv.h"
#include "Pose.h"
#include "ColorRGBA.h"




class UROSBRIDGE_API FROSDisplayBasicMarkerSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSDisplayBasicMarkerSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:

		uint8 MarkerType;
		geometry_msgs::Pose Pose;
		std_msgs::ColorRGBA Color;
		double Scale;
		FString MarkerId;


	public:
		Request() {}

		Request(geometry_msgs::Pose InPose, std_msgs::ColorRGBA InColor, uint8 InScale, FString InMarkerId)
		{
			Pose = InPose;
			Color = InColor;
			Scale = InScale;
			MarkerId = InMarkerId;
		}

		uint8 GetMarkerType()
		{
			return MarkerType;
		}

		geometry_msgs::Pose GetPose()
		{
			return Pose;
		}

		std_msgs::ColorRGBA GetColor()
		{
			return Color;
		}

		double GetScale()
		{
			return Scale;
		}

		FString GetMarkerId()
		{
			return MarkerId;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			MarkerType = JsonObject->GetNumberField("marker_type");
			Pose.FromJson(JsonObject->GetObjectField("pose"));
			Color.FromJson(JsonObject->GetObjectField("color"));
			Scale = JsonObject->GetNumberField("scale");
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
			return "FROSDisplayBasicMarkerSrv:Request {maker_type = " + FString::FromInt(MarkerType) + 
				", pose = " + Pose.ToString() +
				", color = " + Color.ToString() +
				", scale = " + FString::FromInt(Scale) +
				", marker_id = " + MarkerId + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetNumberField("marker_type", MarkerType);
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			Object->SetObjectField(TEXT("color"), Color.ToJsonObject());
			Object->SetNumberField(TEXT("scale"), Scale);
			Object->SetStringField(TEXT("marker_id"), MarkerId);
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		FString MarkerId;


	public:
		Response() {}

		Response(FString InMarkerId)
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

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return "FROSDisplayBasicMarkerSrv:Response {marker_id = " + MarkerId + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("marker_id"), MarkerId);
			return Object;
		}

	};

};