// Fill out your copyright notice in the Description page of Project Settings.


#include "KillsFedDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Main.h"
#include "Enemy.h"

// Sets default values
AKillsFedDoor::AKillsFedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LockTriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("LockTriggerVolume"));
	RootComponent = LockTriggerVolume;
	LockTriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LockTriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LockTriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LockTriggerVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	EnemyVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyVolume"));
	EnemyVolume->SetupAttachment(LockTriggerVolume);

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(LockTriggerVolume);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(LockTriggerVolume);;

	bIsTripped = false;
	Kills = 0;
}

// Called when the game starts or when spawned
void AKillsFedDoor::BeginPlay()
{
	Super::BeginPlay();

	LockTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AKillsFedDoor::OnOverlapBegin);
	LockTriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AKillsFedDoor::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();
}

// Called every frame
void AKillsFedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillsFedDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && !bIsTripped)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			MainPlayerController = GetWorld()->GetFirstPlayerController();
			GetOverlappingActors(RelatedEnemies, AEnemy::StaticClass());
			for (AActor* enemy : RelatedEnemies)
			{

				KillRequirement++;
			}
			bIsTripped = true;
			RaiseDoor();
		}
	}
}

void AKillsFedDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AKillsFedDoor::UpdateDoorLocation(float Z)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += Z;
	Door->SetWorldLocation(NewLocation);
}

void AKillsFedDoor::UpdateKillCount()
{
	Kills++;
	if (Kills == KillRequirement)
	{
		LowerDoor();
	}
}