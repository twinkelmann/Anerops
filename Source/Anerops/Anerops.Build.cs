// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class Anerops : ModuleRules
{
	public Anerops(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        LoadRealSense(Target);
	}

    public void LoadRealSense(TargetInfo Target)
    {
        string platform = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "win32";
        string rssdkDir = "C:/Program Files (x86)/Intel/RSSDK/";

        //the library
        PublicAdditionalLibraries.Add(rssdkDir + "lib/" + platform + "/libpxc.lib");

        //the basic includes
        PublicIncludePaths.Add(rssdkDir + "include");
    }
}
