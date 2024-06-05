// Copyright Epic Games, Inc. All Rights Reserved.

#include "vDUNE_Avatar.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE( FvDuneAvatar, vDuneAvatar );
 
void FvDuneAvatar::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("*** vDuneCore STARTUP ***"));
}


void FvDuneAvatar::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("*** vDuneCore SHUTDOWN ***"));
}
