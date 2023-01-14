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
class UAIPerceptionComponent;
struct FActorPerceptionBlueprintInfo;
UCLASS()
class PROJECT_RPG_API AMainAIController : public AAIController
{

	GENERATED_BODY()

public:

	AMainAIController();

	UPROPERTY(EditAnywhere)
	UAIPerceptionComponent* AIPerception;

	UFUNCTION(BlueprintPure)
	FORCEINLINE AEnemy* GetEnamy() const { return Enemy; }


	UFUNCTION(BlueprintCallable)
	void PerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION(BlueprintPure)
	AActor* FindPlayerInActors(const TArray<AActor*>& UpdatedActors, FActorPerceptionBlueprintInfo& PerceptionInfo, bool& bIsFound);

protected:

	void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	AEnemy* Enemy;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior")
	UBehaviorTree* BotBehaviorTree;

	
};
