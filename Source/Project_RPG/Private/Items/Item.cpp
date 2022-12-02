// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Project_RPG/DebugMacros.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));
	if(GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("before check")));

	UWorld* World = GetWorld();
	FVector Forward = GetActorForwardVector();
	FVector Location = GetActorLocation();
	DRAW_SPHERE(Location)
	//DRAW_LINE(Location, Location + Forward * 100.f)
	DRAW_VECTOR(Location, Location + Forward * 100.f);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

