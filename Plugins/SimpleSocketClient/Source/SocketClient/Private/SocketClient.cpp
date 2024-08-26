#include "SocketClient.h"

#define LOCTEXT_NAMESPACE "FSocketClientModule"

bool FSocketClientModule::isShuttingDown;

void FSocketClientModule::StartupModule()
{
	// This code will execute after your module is loaded into memory
}

void FSocketClientModule::ShutdownModule()
{
	isShuttingDown = true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSocketClientModule, SocketClient)