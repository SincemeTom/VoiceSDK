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

            string IFlytekrecordSDKDllName = "iat_record_sample.dll";
            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add(IFlytekSDKDllName);

            string RecordBasePath = System.IO.Path.Combine(ModuleDirectory, "../../../Binaries/ThirdParty/IFlytekSDK", "Win64");
            string IFlytekrecordSDKDllPath = System.IO.Path.Combine(BasePath, IFlytekrecordSDKDllName);

            RuntimeDependencies.Add(new RuntimeDependency(IFlytekrecordSDKDllPath));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
		{
			// // Add the import library
			// PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
			// PublicAdditionalLibraries.Add("xdinputexp.lib");

			// string XdInputDllName = "xdinputexp.dll";
			// // Delay-load the DLL, so we can load it from the right place first
			// PublicDelayLoadDLLs.Add(XdInputDllName);
			
			// string BasePath = System.IO.Path.Combine(ModuleDirectory, "../../../Binaries/ThirdParty/IFlytekSDK", "Win32");
			// string XdInputDllPath = System.IO.Path.Combine(BasePath, XdInputDllName);
			// RuntimeDependencies.Add(new RuntimeDependency(XdInputDllPath));
		}
	}
}
