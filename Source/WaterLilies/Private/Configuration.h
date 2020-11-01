#pragma once

#include "CoreMinimal.h"

UENUM()
enum InteractionMode { Creation, Scaling, Coloring };

class Configuration
{
public:
	Configuration();
	/** Get the current mode */
	InteractionMode GetMode();
	/** Set the next mode */
	void SetMode();
private:
	InteractionMode ActiveMode;
};
