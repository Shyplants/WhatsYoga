#include "WYGameInstance.h"
#include "GameMode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

UWYGameInstance::UWYGameInstance()
{
	ContentIndex = 0;
}

void UWYGameInstance::Init()
{
	Super::Init();

	const FString SavedDir = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	// Config File Parse
	{
		FString ConfigFileName(TEXT("Config.txt"));
		FString ConfigFilePath = FPaths::Combine(SavedDir, ConfigFileName);
		FPaths::MakeStandardFilename(ConfigFilePath);

		FString ConfigInString;
		FFileHelper::LoadFileToString(ConfigInString, *ConfigFilePath);

		TArray<FString> ConfigOptions;
		ConfigInString.ParseIntoArray(ConfigOptions, TEXT("?"));

		for (FString& ConfigOption : ConfigOptions)
		{
			TArray<FString> KeyAndValue;
			ConfigOption.ParseIntoArray(KeyAndValue, TEXT("="));

			ConfigOptionsMap.Add(*KeyAndValue[0], *KeyAndValue[1]);
		}

		if (ConfigOptionsMap.Contains(TEXT("IP")))
		{
			IP = ConfigOptionsMap[TEXT("IP")];
		}

		if (ConfigOptionsMap.Contains(TEXT("Port")))
		{
			Port = FCString::Atoi(*ConfigOptionsMap[TEXT("Port")]);
		}
	}

	TCPConnect();
}

void UWYGameInstance::HandleReceivedTCPMessage(FString Message)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		FString Event;
		FString Content;
		FString Score;
		FString Timestamp;

		FString stringValue;
		float floatValue;

		if (JsonObject->TryGetStringField(TEXT("event"), Event) &&
			JsonObject->TryGetStringField(TEXT("content"), Content) &&
			JsonObject->TryGetStringField(TEXT("timestamp"), Timestamp))
		{
			if (Event == TEXT("start"))
			{

			}

			else if (Event == TEXT("work"))
			{
				if (JsonObject->TryGetNumberField(TEXT("score"), floatValue))
				{
					SetGaugePercent(floatValue);
				}
			}

			else if (Event == TEXT("end"))
			{

			}

			else if (Event == TEXT("player_info"))
			{
				if (JsonObject->TryGetStringField(TEXT("player_name"), stringValue))
				{
					UE_LOG(LogTemp, Log, TEXT("Name received : %s"), *stringValue);
					SetPlayerName(stringValue);
				}

				if (JsonObject->TryGetStringField(TEXT("content"), stringValue))
				{
					SetContentName(stringValue);
					SetContentIndex();
				}

			}
		}
	}
}

FString UWYGameInstance::GetLocalIPAddress()
{
	bool bCanBind;
	TSharedRef<FInternetAddr> LocalIP = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);

	return LocalIP->ToString(false);
}

void UWYGameInstance::SetGaugePercent(float Percent)
{
	check(Percent >= 0.0f && Percent <= 100.0f);

	AMainGameMode* MainGameMode = CastChecked<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameMode->SetGaugePercent(Percent);
}

void UWYGameInstance::SetContentIndex()
{
	TArray<FString> ContentNames = { TEXT("upper_body") , TEXT("lower_body"), TEXT("body") };

	for (int32 i = 0; i < ContentNames.Num(); ++i)
	{
		if (!ContentName.Compare(ContentNames[i]))
		{
			ContentIndex = i;
			break;
		}
	}
}
