// Copyright by Witold Studencki 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Project/MiscEnums.h"
#include "AnimActionsInterface.generated.h"


UINTERFACE(MinimalAPI)
class UAnimActionsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_RPG_API IAnimActionsInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlaySoundNotify(const ESoundNotifyType& Type);

};
