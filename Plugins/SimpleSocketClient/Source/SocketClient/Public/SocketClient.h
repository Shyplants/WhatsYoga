#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Async/Async.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/FileManager.h"
#include "Containers/Queue.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/Base64.h"
#include "Misc/SecureHash.h"
#include "Misc/AES.h"
#include "Modules/ModuleManager.h"
#include "Runtime/Launch/Resources/Version.h"
#include "IPAddressAsyncResolve.h"
#include "SocketClient.generated.h"


class FSocketClientTCPReceiveDataThread;
class FSocketClientTCPSendDataThread;

class UTCPConnectAsyncNode;

USTRUCT()
struct FSocketClientPluginSession
{
	GENERATED_USTRUCT_BODY()

	int64 addToCleanerTime = 0;
	FString clientID = FString();
	FSocket* socket = nullptr;

	FSocketClientTCPSendDataThread* tcpSendThread = nullptr;
	FSocketClientTCPReceiveDataThread* tcpReceiveThread = nullptr;
};


UENUM(BlueprintType)
enum class EReceiveFilterClient : uint8
{
	E_SAB 	UMETA(DisplayName = "Message And Bytes"),
	E_S		UMETA(DisplayName = "Message"),
	E_B		UMETA(DisplayName = "Bytes")

};


UENUM(BlueprintType)
enum class ESocketClientIPType : uint8
{
	E_ipv4	UMETA(DisplayName = "IPv4"),
	E_ipv6 	UMETA(DisplayName = "IPv6")
};

UENUM(BlueprintType)
enum class ESocketPlatformClient : uint8
{
	E_SSC_SYSTEM		UMETA(DisplayName = "System"),
	E_SSC_DEFAULT 		UMETA(DisplayName = "Auto"),
	E_SSC_WINDOWS		UMETA(DisplayName = "WINDOWS"),
	E_SSC_MAC			UMETA(DisplayName = "MAC"),
	E_SSC_IOS			UMETA(DisplayName = "IOS"),
	E_SSC_UNIX			UMETA(DisplayName = "UNIX"),
	E_SSC_ANDROID		UMETA(DisplayName = "ANDROID"),
	E_SSC_PS4			UMETA(DisplayName = "PS4"),
	E_SSC_XBOXONE		UMETA(DisplayName = "XBOXONE"),
	E_SSC_HTML5			UMETA(DisplayName = "HTML5"),
	E_SSC_SWITCH		UMETA(DisplayName = "SWITCH")

};


UENUM(BlueprintType)
enum class ESocketClientTCPSeparator : uint8
{
	E_None 				UMETA(DisplayName = "None"),
	E_ByteSeparator		UMETA(DisplayName = "Separate via one Byte"),
	E_StringSeparator	UMETA(DisplayName = "Separate via String"),
	E_LengthSeparator	UMETA(DisplayName = "Separate by Length")

};

#ifndef __SocketClientCleanerThread
#define __SocketClientCleanerThread
#include "SocketClientCleanerThread.h"
#endif

#ifndef __SocketClientAsyncNodes
#define __SocketClientAsyncNodes
#include "SocketClientAsyncNodes.h"
#endif

#ifndef __SocketClientBPLibrary
#define __SocketClientBPLibrary
#include "SocketClientBPLibrary.h"
#endif

#ifndef __SocketClientTCP
#define __SocketClientTCP
#include "SocketClientTCP.h"
#endif


class FSocketClientModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static bool isShuttingDown;
};