#include "Configuration.h"

Configuration::Configuration()
{
	ActiveMode = Creation;
}

InteractionMode Configuration::GetMode()
{
	return ActiveMode;
}

void Configuration::SetMode()
{
	if (ActiveMode == Creation)
	{
		ActiveMode = Scaling;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Current Mode: Scaling"));
	}
	else if (ActiveMode == Scaling)
	{
		ActiveMode = Coloring;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Current Mode: Coloring"));
	}
	else
	{
		ActiveMode = Creation;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Current Mode: Creation"));
	}
}
