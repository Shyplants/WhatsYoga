#include "SocketClientAsyncNodes.h"

UTCPConnectAsyncNode* UTCPConnectAsyncNode::socketClientTCPConnectionAsyncNode(FString domainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilters, ESocketClientTCPSeparator messageWrapping, bool disableNaglesAlgorithm)
{
	UTCPConnectAsyncNode* instance = NewObject<UTCPConnectAsyncNode>();

	instance->domainOrIP = domainOrIP;
	instance->ipType = ipType;
	instance->port = port;
	instance->receiveFilters = receiveFilters;
	instance->messageWrapping = messageWrapping;
	instance->disableNagleAlgorithm = disableNaglesAlgorithm;

	instance->AddToRoot();

	return instance;
}

void UTCPConnectAsyncNode::Activate()
{
	USocketClientBPLibrary::GetSocketClientTarget()->connectSocketClientTCPNonStatic(domainOrIP, ipType, port, receiveFilters,
		messageWrapping, connectionID, this, disableNagleAlgorithm);
}

void UTCPConnectAsyncNode::triggerConnectionEvent(bool success, FString clientConnectionID, FString serverMessage)
{
	if (success)
	{
		OnConnect.Broadcast(serverMessage, clientConnectionID, "", TArray<uint8>());
	}
	else
	{
		OnDisconnect.Broadcast(serverMessage, clientConnectionID, "", TArray<uint8>());
	}
}

void UTCPConnectAsyncNode::triggerMessageEvent(TArray<uint8> byteDataArray, FString clientConnectionID, FString serverMessage)
{
	OnServerMessage.Broadcast("", clientConnectionID, serverMessage, TArray<uint8>());
}