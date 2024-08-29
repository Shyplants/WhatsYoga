#include "JsonHelper.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

FString UJsonHelper::CreateEventJson(const FString& eventType)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField(TEXT("event"), eventType);

	FString Timestamp = FDateTime::Now().ToIso8601();
	JsonObject->SetStringField(TEXT("timestamp"), Timestamp);

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
}

void UJsonHelper::ParseEventJson(const FString& JsonString)
{
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		FString EventType;
		float Value;
		FString Timestamp;

		if (JsonObject->TryGetStringField(TEXT("event"), EventType) &&
			JsonObject->TryGetNumberField(TEXT("value"), Value) &&
			JsonObject->TryGetStringField(TEXT("event"), Timestamp))
		{
			UE_LOG(LogTemp, Log, TEXT("Event: %s"), *EventType);
			UE_LOG(LogTemp, Log, TEXT("Value: %f"), Value);
			UE_LOG(LogTemp, Log, TEXT("Timestamp: %s"), *Timestamp);
		}
	}
}
