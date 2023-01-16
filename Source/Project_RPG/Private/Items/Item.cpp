// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Project_RPG/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/MainCharacter.h"
#include "NiagaraComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	
	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMeshComponent"));
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	
	SparksEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	ItemSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
	RootComponent = ItemMesh;

	ItemSkeletalMesh->SetupAttachment(GetRootComponent());
	
	SparksEffect->SetupAttachment(GetRootComponent());
	Sphere->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	ItemMesh->SetSimulatePhysics(true);
	GetItemMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);


	if(GEngine)
	{

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	}
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}
float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

UPrimitiveComponent* AItem::GetItemMesh()
{
	if (bIsMeshSkeletal)
	{
		return ItemSkeletalMesh;
	}
	else
	{
		return ItemMesh;
	}
}

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
		if (Player)
		{
			Player->SetOverlappingItem(this);
		}
	}	
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		Player->SetOverlappingItem(nullptr);
	}
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	//if (ItemState == EItemState::EIS_Hovering)
	{
	//	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
	//AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	//AddActorWorldRotation(FRotator(0.f, Amplitude* DeltaTime, 0.f));
}

