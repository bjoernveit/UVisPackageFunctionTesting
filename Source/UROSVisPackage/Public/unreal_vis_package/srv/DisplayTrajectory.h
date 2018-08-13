#pragma once
#include "ROSBridgeSrv.h"
#include "Point.h"
#include "ColorRGBA.h"




class UROSBRIDGE_API FROSDisplayTrajectorySrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSDisplayTrajectorySrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		TArray<geometry_msgs::Point> Points;
		std_msgs::ColorRGBA ColorBegin;
		std_msgs::ColorRGBA ColorEnd;
		FString MarkerId;


	public:
		Request() {}

		Request(TArray<geometry_msgs::Point> InPoints, std_msgs::ColorRGBA InColorBegin, std_msgs::ColorRGBA InColorEnd, FString InMarkerId)
		{
			Points = InPoints;
			ColorBegin = InColorBegin;
			ColorEnd = InColorEnd;
			MarkerId = InMarkerId;
		}

		TArray<geometry_msgs::Point> GetPoints()
		{
			return Points;
		}

		std_msgs::ColorRGBA GetColorBegin()
		{
			return ColorBegin;
		}

		std_msgs::ColorRGBA GetColorEnd()
		{
			return ColorEnd;
		}

		FString GetMarkerId()
		{
			return MarkerId;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Points.Empty();
			TArray<TSharedPtr<FJsonValue>> PointsPtrArray = JsonObject->GetArrayField(TEXT("points"));
			for (auto &ptr : PointsPtrArray)
			{
				geometry_msgs::Point Entry;
				Entry.FromJson(ptr->AsObject());
				Points.Add(Entry);
			}

			ColorBegin.FromJson(JsonObject->GetObjectField("color_begin"));
			ColorEnd.FromJson(JsonObject->GetObjectField("color_end"));
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
			return "FROSDisplayTrajectorySrv:Request {points size = " + FString::FromInt(Points.Num()) +
				", color_begin = " + ColorBegin.ToString() +
				", color_end = " + ColorEnd.ToString() +
				", marker_id = " + MarkerId + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			TArray<TSharedPtr<FJsonValue>> PointsPtrArray;
			for (auto &Entry : Points)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				PointsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("points", PointsPtrArray);

			Object->SetObjectField(TEXT("color_begin"), ColorBegin.ToJsonObject());
			Object->SetObjectField(TEXT("color_end"), ColorEnd.ToJsonObject());
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
			return "FROSDisplayTrajectorySrv:Response {marker_id = " + MarkerId + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("marker_id"), MarkerId);
			return Object;
		}

	};

};