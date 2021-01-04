// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Main.h"
#include "Item.h"

// Sets default values
AItemChest::AItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChestBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest Base"));
	ChestBase->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComponent = ChestBase;

	ChestLid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest Lid"));
	ChestLid->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ChestLid->SetupAttachment(ChestBase);

	OpenVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Open Volume"));
	OpenVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OpenVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OpenVolume->SetupAttachment(ChestBase);

}

// Called when the game starts or when spawned
void AItemChest::BeginPlay()
{
	Super::BeginPlay();
	InitialLidLocation = ChestLid->GetComponentLocation();
	InitialLidRotation = ChestLid->GetComponentRotation();

	OpenVolume->OnComponentBeginOverlap.AddDynamic(this, &AItemChest::OnOverlapBegin);
	OpenVolume->OnComponentEndOverlap.AddDynamic(this, &AItemChest::OnOverlapEnd);
}

// Called every frame
void AItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemChest::OpenItemChest(AMain* Char)
{
	OpenItemChestStart();
}

void AItemChest::UpdateLidLocation(float Value)
{
	FVector NewLocation = InitialLidLocation;
	NewLocation.X -= Value;
	ChestLid->SetWorldLocation(NewLocation);
}

void AItemChest::UpdateLidRotation(float Value)
{
	FRotator NewRotation = InitialLidRotation;
	NewRotation.Pitch -= Value;
	ChestLid->SetWorldRotation(NewRotation);
}

void AItemChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Char = Cast<AMain>(OtherActor);
		if(Char)
		{
			Char->ItemChestInRange = this;
		}
	}
}

void AItemChest::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Char = Cast<AMain>(OtherActor);
		if (Char)
		{
			Char->ItemChestInRange = nullptr;
		}
	}
}

void AItemChest::SpawnChestContents()
{
	FActorSpawnParameters SpawnInfo;
	FRotator Direction = GetActorRotation();
	FVector Location = GetActorLocation()+FVector(0.f,0.f,100.f);

	AItem* ItemToSpawn = GetWorld()->SpawnActor<AItem>(ChestContents->GetDefaultObject()->GetClass(), Location, Direction, SpawnInfo);
}
