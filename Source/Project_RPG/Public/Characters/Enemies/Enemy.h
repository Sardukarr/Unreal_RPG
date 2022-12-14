// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
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


	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;
protected:
	/**
	* Animations Montages
	*/

	UPROPERTY(EditAnywhere, Category = Animations)
	UAnimMontage* HitMontage;

	void PlayMontage(UAnimMontage* montage,const FName& sectionName = FName(NAME_None), bool bOverride = true);


	virtual void BeginPlay() override;

public:	

};
