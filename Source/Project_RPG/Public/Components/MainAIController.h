// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainAIController.generated.h"

/**
 * 
 */


class AEnemy;
class UBehaviorTree;
UCLASS()
class PROJECT_RPG_API AMainAIController : public AAIController
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE AEnemy* GetEnamy() const { return Enemy; }

protected:

	void OnPossess(APawn* InPawn) override;


	AEnemy* Enemy;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior")
	UBehaviorTree* BotBehaviorTree;

	
};
