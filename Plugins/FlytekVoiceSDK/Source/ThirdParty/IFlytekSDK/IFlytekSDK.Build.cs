// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class IFlytekSDK : ModuleRules
{
	public IFlytekSDK(TargetInfo Target)
	{
		Type = ModuleType.External;
		
		PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
			PublicAdditionalLibraries.Add("msc_x64.lib");

            string IFlytekSDKDllName = "msc_x64.dll";
            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add(IFlytekSDKDllName);

            string BasePath = System.IO.Path.Combine(ModuleDirectory, "../../../Binaries/ThirdParty/IFlytekSDK", "Win64");
            string IFlytekSDKDllPath = System.IO.Path.Combine(BasePath, IFlytekSDKDllName);

            RuntimeDependencies.Add(new RuntimeDependency(IFlytekSDKDllPath));
            
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
            PublicAdditionalLibraries.Add("iat_record_sample.lib");

           
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
		{
			
		}
	}
}
