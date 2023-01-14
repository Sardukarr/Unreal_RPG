// Copyright by Witold Studencki 2022


#include "Components/MainAIController.h"
#include "Characters/Enemies/Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

AMainAIController::AMainAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AMainAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Enemy = Cast<AEnemy>(InPawn);
	if (BotBehaviorTree)
	{
		RunBehaviorTree(BotBehaviorTree);
	}
	if (AIPerception)
	{
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &AMainAIController::PerceptionUpdate);
	}
//	GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(FKey(FName("")), this);


//	GetBlackboardComponent()->SetValue(FKey(FName("")), this);
}

void AMainAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AMainAIController::PerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	if (Enemy->GetEnemyState() <= EEnemyState::EES_Dead) return;
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->ActorHasTag(FName("PlayerCharacter")))
		{
			//State = EEnemyState::EES_Chasing;
			//GetWorldTimerManager().ClearTimer(PatrolTimer);
			//GetCharacterMovement()->MaxWalkSpeed = 300.f;
			Enemy->CombatTarget = Actor;
			//MoveToTarget(CombatTarget);
			UE_LOG(LogTemp, Warning, TEXT("Seen Pawn, now Chasing"));
		}
	}

}

AActor* AMainAIController::FindPlayerInActors(const TArray<AActor*>& UpdatedActors, FActorPerceptionBlueprintInfo& PerceptionInfo, bool& bIsFound)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->ActorHasTag(FName("PlayerCharacter")))
		{
			//FActorPerceptionBlueprintInfo& info = FActorPerceptionBlueprintInfo();
			AIPerception->GetActorsPerception(Actor, PerceptionInfo);
			UE_LOG(LogTemp, Warning, TEXT("Found Player"));
			bIsFound = true;
			return Actor;
		}
	}
	bIsFound = false;
	return nullptr;
}
