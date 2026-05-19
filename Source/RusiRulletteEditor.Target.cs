using UnrealBuildTool;
using System.Collections.Generic;

public class RusiRulletteEditorTarget : TargetRules
{
	public RusiRulletteEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("RusiRullette");
	}
}
