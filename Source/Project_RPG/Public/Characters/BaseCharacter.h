// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "CharacterTypes.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class PROJECT_RPG_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	void SpawnHitParticles(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);

protected:

	virtual void BeginPlay() override;
	virtual void Attack();
	UFUNCTION(BlueprintCallable)
	virtual bool CanAttack();

	UFUNCTION(BlueprintNativeEvent)
	 void Die();

	virtual void Die_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	/**
	* Animation montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Animations)
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = Animations)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Animations)
	TArray<FName> DeathMontageSections;


	void DirectionalHit(const FVector& ImpactPoint);
	float PlayMontage(UAnimMontage* montage, const FName& sectionName = FName(NAME_None), bool bOverride = true);
	/*
	* Components
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Sounds)
	USoundBase* HitReactSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	/**
* States
*/
	UPROPERTY(VisibleInstanceOnly)
		ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
