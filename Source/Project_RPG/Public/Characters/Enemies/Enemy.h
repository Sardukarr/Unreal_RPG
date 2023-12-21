// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBar;
class AMainAIController;
class UBlackboardComponent;
//class UPawnSensingComponent;
class UAIPerceptionComponent;

UCLASS()
class PROJECT_RPG_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FHitResult& Hit, AActor* Hitter) override;


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;
	/**
	* Combat
	*/


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* CombatTarget;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	/**
	* AI
	*/

	UPROPERTY(EditAnywhere)
	AMainAIController* EnemyAIController;

	/*UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;*/

	void ToggleHealthBar(bool show);

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float ChaseSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float PatrolSpeed = 125.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float WaitMax = 10.f;

	/**
	* Navigation
	*/

	// Current patrol target
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	void PatrolTimerFinished();

	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
	void CheckPatrolTarget();

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	void CheckCombatTarget();

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	void Chase();

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	float StartAttack();

	virtual bool CanAttack() override;

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	void LoseInterest();

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	bool IsInsideRadius(double Radius);

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	bool IsChasing();

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	bool IsAttacking();

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	bool IsDead();

	UFUNCTION(BlueprintCallable, Category = "AI Combat")
	bool IsBusy();



	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
	void MoveToTarget(AActor* Target, float AcceptanceRadious=50.f);

	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
	AActor* ChooseRandomPatrolTarget();

	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
	AActor* ChooseNextPatrolTarget();


protected:

	UPROPERTY(VisibleAnywhere)
	UHealthBar* HealthBarWidget;


	virtual void BeginPlay() override;
	//virtual void Die() override;
	virtual void Die_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EEnemyState State = EEnemyState::EES_Idle;
//private:


	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* Blackboard;


	bool InTargetRange(AActor* Target, double Radius);

public:	
	FORCEINLINE EEnemyState GetEnemyState() const { return State; }
	FORCEINLINE UAttributeComponent* GetEnemyAttributes() const { return Attributes; }
};
