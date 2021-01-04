// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"

AShield::AShield()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	bShieldParticles = false;

	ShieldState = EShieldState::ESS_Pickup;

	DamageReduction = .9f;
}

void AShield::BeginPlay()
{
	Super::BeginPlay();

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AShield::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AShield::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AShield::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if ((ShieldState == EShieldState::ESS_Pickup) && OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(this);
		}
	}
}

void AShield::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AShield::Equip(AMain* Char)
{
	if (Char)
	{
		SetInstigator(Char->GetController());

		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* ShieldEquipSocket = Char->GetMesh()->GetSocketByName("ShieldEquipSocket");

		if (ShieldEquipSocket)
		{
			ShieldEquipSocket->AttachActor(this, Char->GetMesh());
			bRotate = false;
			Char->SetEquippedShield(this);
			Char->SetActiveOverlappingItem(nullptr);
		}

		if (OnEquipSound)
		{
			UGameplayStatics::PlaySound2D(this, OnEquipSound);
		}

		if (!bShieldParticles)
		{
			IdleParticleSystemComponent->Deactivate();
		}

		ShieldOwner = Char;
	}
}

void AShield::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Shield bash?

	//if (OtherActor)
	//{
	//	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	//	if (Enemy)
	//	{

			//if (Enemy->HitParticles)
			//{
			//	const USkeletalMeshSocket* ShieldSocket = SkeletalMesh->GetSocketByName("ShieldSocket");
			//	if (ShieldSocket)
			//	{
			//		FVector SocketLocation = ShieldSocket->GetSocketLocation(SkeletalMesh);
			//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.f), false);
			//	}
			//}
			//if (Enemy->HitSound)
			//{
			//	UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			//}
			//if (DamageTypeClass)
			//{
			//	UGameplayStatics::ApplyDamage(Enemy, DamageReduction, WeaponInstigator, this, DamageTypeClass);
			//}

	//	}
	//}
}

void AShield::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AShield::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AShield::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShield::Sheathe()
{

	const USkeletalMeshSocket* SheatheSocket = ShieldOwner->GetMesh()->GetSocketByName("OffHandSheathe");

	SheatheSocket->AttachActor(this, ShieldOwner->GetMesh());

}

void AShield::UnSheathe()
{

	const USkeletalMeshSocket* LeftHandSocket = ShieldOwner->GetMesh()->GetSocketByName("ShieldEquipSocket");

	LeftHandSocket->AttachActor(this, ShieldOwner->GetMesh());
}

void AShield::SpawnBlockEmitter()
{
	const USkeletalMeshSocket* ShieldSocket = SkeletalMesh->GetSocketByName("ShieldSocket");
	if (ShieldSocket)
	{
		FVector SocketLocation = ShieldSocket->GetSocketLocation(SkeletalMesh);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlockParticles, SocketLocation, FRotator(0.f), false);
	}
}
