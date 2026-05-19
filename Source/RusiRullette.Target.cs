using UnrealBuildTool;
using System.Collections.Generic;

public class RusiRulletteTarget : TargetRules
{
	public RusiRulletteTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("RusiRullette");
	}
}
