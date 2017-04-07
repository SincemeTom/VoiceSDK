// Copyright 2016 Gamemakin LLC. All Rights Reserved.


#include "FlytekVoiceSDK.h"
#include "Paths.h"
#include "qisr.h"

#define LOCTEXT_NAMESPACE "FFlytekVoiceSDKModule"


FFlytekVoiceSDKModule::FFlytekVoiceSDKModule()
{

}
FFlytekVoiceSDKModule::~FFlytekVoiceSDKModule()
{

}
void FFlytekVoiceSDKModule::StartupModule()
{
	if (IsAvailable())
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("%s"), TEXT("Module Started"));
	}

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("FlytekVoiceSDK")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VR7663SDK/Win64/VR7663Dll.dll"));
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VR7663SDK/Win32/VR7663Dll.dll"));
#endif
#endif

	VR7663DllHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (VR7663DllHandle)
	{
		// Call the test function in the third party library that opens a message box
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load vr platform sdk third party library"));
	}

}

void FFlytekVoiceSDKModule::ShutdownModule()
{
	
}

int32 FFlytekVoiceSDKModule::VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params)
{
	return 0;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFlytekVoiceSDKModule, FlytekVoiceSDK)
DEFINE_LOG_CATEGORY(LogFlytekVoiceSDK);