// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Main.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"


ABomb::ABomb()
{
	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//ProjectileMovement->ProjectileGravityScale = 0.f;
	//ProjectileMovement->bShouldBounce = true;

	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SkeletalMesh->SetMassOverrideInKg(FName("SM_Gruntling_BombDrop_Internal"), 5000.f, true);

	ExplosionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionRadius"));
	ExplosionRadius->SetupAttachment(GetRootComponent());
	ExplosionRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ExplosionRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ExplosionRadius->SetSphereRadius(175.f);

	Damage = 50.f;

	ToolName = "Bombs";

	bIsEquippedInRightHand = true;
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();


}

void ABomb::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && bIsActive)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			Detonate();
		}
	}
}

void ABomb::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABomb::PrimaryFunction()///THROW
{
	Super::PrimaryFunction();

	
	const FVector Rotation = ToolOwner->GetActorForwardVector();


	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SkeletalMesh->AddImpulse(Rotation,"SM_Gruntling_BombDrop_Internal",true);
	//SkeletalMesh->AddForce(Rotation, "SM_Gruntling_BombDrop_Internal", true);
	bIsActive = true;

	BeginCountDown();
}

void ABomb::SecondaryFunction()//DROP
{
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	BeginCountDown();
}

void ABomb::Equip(AMain* Char)
{
	Super::Equip(Char);
}

void ABomb::SetInstigator(AController* Inst)
{
	Super::SetInstigator(Inst);
}

void ABomb::BeginCountDown()
{
	GetWorld()->GetTimerManager().SetTimer(FuseTimer, this, &ABomb::Detonate, 4.f, false);

}

void ABomb::Detonate()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles,GetActorLocation());

	TArray<AActor*> CharactersToDamage;
	ExplosionRadius->GetOverlappingActors(CharactersToDamage,ACharacter::StaticClass());

	for (AActor* Char : CharactersToDamage)
	{
		UGameplayStatics::ApplyDamage(Char, Damage, ToolInstigator, this, DamageTypeClass);
	}

	Destroy();
}
