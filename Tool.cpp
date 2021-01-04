// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/World.h"




// Sets default values
ATool::ATool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(SkeletalMesh);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetupAttachment(SkeletalMesh);

	IdleParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleSystemComponent->SetupAttachment(GetRootComponent());

	Damage = 0.f;

}

// Called when the game starts or when spawned
void ATool::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATool::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ATool::OnOverlapEnd);
	

}

// Called every frame
void ATool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATool::PrimaryFunction()
{

}

void ATool::SecondaryFunction()
{

}

void ATool::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			//if (!Main->ToolInSlotOne)
			//{
				//Main->SetToolSlotOne(this->GetClass());
			//}
		}

}

void ATool::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ATool::Equip(AMain* Char)
{
	if (Char)
	{
		if (bIsEquippedInRightHand)
		{
			SetInstigator(Char->GetController());
			SkeletalMesh->SetSimulatePhysics(false);

			const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");

			if (RightHandSocket)
			{
				RightHandSocket->AttachActor(this, Char->GetMesh());
				Char->SetCurrentActiveTool(this);
			}

			Char->SetEquipmentStatus(EEquipmentStatus::EES_Tool);
			SetToolOwner(Char);
		}
		else 
		{
			SetInstigator(Char->GetController());
			SkeletalMesh->SetSimulatePhysics(false);

			const USkeletalMeshSocket* LeftHandSocket = Char->GetMesh()->GetSocketByName("LeftHandSocket");

			if (LeftHandSocket)
			{
				LeftHandSocket->AttachActor(this, Char->GetMesh());
				Char->SetCurrentActiveTool(this);
			}

			Char->SetEquipmentStatus(EEquipmentStatus::EES_Tool);
			SetToolOwner(Char);
		}

	}
}

void ATool::Sheathe()
{

}

void ATool::UnSheathe()
{

}

void ATool::SetToolOwner(AMain* Char)
{
	ToolOwner = Char;
}
