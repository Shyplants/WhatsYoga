#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonHelper.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UJsonHelper : public UObject
{
	GENERATED_BODY()

public:
	static FString CreateEventJson(const FString& eventType);
	static void ParseEventJson(const FString& JsonString);
	
};
