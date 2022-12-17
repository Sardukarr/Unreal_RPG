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
private:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	void Die();


public:	

};
