// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseEquipable.h"
#include "Weapon.generated.h"

/**
 * 
 */
class UBoxComponent;
class USoundBase;

UCLASS()
class PROJECT_RPG_API AWeapon : public ABaseEquipable
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void OnEquip(USceneComponent* InParent) override;

	virtual void OnPickup(USceneComponent* InParent, AActor* NewOwner, APawn* NewInstigator) override;

	virtual void AttachToSocket(USceneComponent* InParent, const FName& InSocketName) override;

	virtual void OnUnequip(USceneComponent* InParent) override;


	UFUNCTION(BlueprintImplementableEvent)
	void CreateForceFields(const FVector& FieldLocation);

	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;


	virtual	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ScabbardSocketName;
	

//private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;


public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
