#include "WYGameInstance.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UWYGameInstance::UWYGameInstance()
{
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
