// Copyright 2016 Gamemakin LLC. All Rights Reserved.


#include "FlytekVoiceSDK.h"
#include "Paths.h"
#include "IPluginManager.h"
#include "WindowsPlatformProcess.h"
#include "MessageDialog.h"
#include "speech_recognizer.h"


#define LOCTEXT_NAMESPACE "FFlytekVoiceSDKModule"

const FString LoginParams = TEXT("appid = 58d087b0, work_dir = .");

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
#if 1
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("FlytekVoiceSDK")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/msc_x64.dll"));
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/msc_x64.dll"));
#endif
#endif

	/*DllHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (DllHandle)
	{
		// Call the test function in the third party library that opens a message box
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load voice sdk third party library"));
	}
	*/

	// Add on the relative location of the third party dll and load it
	FString DllLibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	
	DllLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/iat_record_sample.dll"));
#else
	DllLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/iat_record_sample.dll"));
#endif
#endif

	DllHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (DllHandle)
	{
		// Call the test function in the third party library that opens a message box
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load iat_record_sample third party library"));
	}

#endif	
	//VoiceSDKLogin(FString(), FString(), LoginParams);
}

void FFlytekVoiceSDKModule::ShutdownModule()
{
	if (DllHandle)
	{
		FPlatformProcess::FreeDllHandle(DllHandle);
	}
	DllHandle = nullptr;
}

void FFlytekVoiceSDKModule::VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params)
{
	//TCHAR_TO_ANSI
	
	auto Result = sr_login(TCHAR_TO_ANSI(*UserName), TCHAR_TO_ANSI(*Password), TCHAR_TO_ANSI(*Params));
	if (Result == 0)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogin Successful ! "))
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Error, TEXT("VoiceSDKLogin Faild ! Error code : %d"), Result)
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFlytekVoiceSDKModule, FlytekVoiceSDK)
DEFINE_LOG_CATEGORY(LogFlytekVoiceSDK);