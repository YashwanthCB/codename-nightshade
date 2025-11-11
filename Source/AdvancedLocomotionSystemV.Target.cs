

using UnrealBuildTool;
using System.Collections.Generic;

public class AdvancedLocomotionSystemVTarget : TargetRules
{
	public AdvancedLocomotionSystemVTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "AdvancedLocomotionSystemV" } );
	}
}
