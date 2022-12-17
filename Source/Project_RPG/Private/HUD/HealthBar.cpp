// Copyright by Witold Studencki 2022


#include "HUD/HealthBar.h"
#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"


void UHealthBar::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}