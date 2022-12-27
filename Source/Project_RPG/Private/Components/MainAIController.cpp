// Copyright by Witold Studencki 2022


#include "Components/MainAIController.h"
#include "Characters/Enemies/Enemy.h"
#include "BehaviorTree/BehaviorTree.h"

void AMainAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Enemy = Cast<AEnemy>(InPawn);
	if (BotBehaviorTree)
	{
		RunBehaviorTree(BotBehaviorTree);
	}

}
