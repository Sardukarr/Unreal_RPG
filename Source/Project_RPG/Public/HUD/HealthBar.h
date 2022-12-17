// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UHealthBar : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
private:
	UPROPERTY()
	class UHealthBarWidget* HealthBarWidget;
};
