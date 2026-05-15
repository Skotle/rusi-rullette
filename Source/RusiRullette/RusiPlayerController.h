#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RusiPlayerController.generated.h"

UCLASS()
class RUSIRULLETTE_API ARusiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
