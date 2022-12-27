// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBar;
class AAIController;
class UPawnSensingComponent;


UCLASS()
class PROJECT_RPG_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionalHit(const FVector& ImpactPoint);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;


	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	/**
	* AI
	*/

	UPROPERTY(EditAnywhere)
	AAIController* EnemyController;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

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

	void CheckPatrolTarget();

	void CheckCombatTarget();


	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;


	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
	void MoveToTarget(AActor* Target, float AcceptanceRadious=50.f);

	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
	AActor* ChooseRandomPatrolTarget();

	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
	AActor* ChooseNextPatrolTarget();


protected:

	UPROPERTY(VisibleAnywhere)
	UHealthBar* HealthBarWidget;


	/**
	* Animations Montages
	*/

	UPROPERTY(EditAnywhere, Category = Animations)
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = Animations)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = Animations)
	TArray<FName> DeathMontageSections;

	void PlayMontage(UAnimMontage* montage,const FName& sectionName = FName(NAME_None), bool bOverride = true);

	

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState State = EEnemyState::EES_Idle;
//private:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	void Die();
	bool InTargetRange(AActor* Target, double Radius);

public:	

};
