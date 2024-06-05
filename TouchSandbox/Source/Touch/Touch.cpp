// Copyright Epic Games, Inc. All Rights Reserved.

#include "Touch.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE( FTouch, Touch );
 
void FTouch::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("*** vDuneCore STARTUP ***"));
}


void FTouch::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("*** vDuneCore SHUTDOWN ***"));
}
