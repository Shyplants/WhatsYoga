#pragma once

#include "CoreMinimal.h"


class WHATSYOGA_API JsonHelper
{
public:
	static FString CreateEventJson(const FString& eventType);
	static void ParseEventJson(const FString& JsonString);	
};
