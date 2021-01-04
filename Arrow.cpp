// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"

AArrow::AArrow()
{


	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
	ArrowMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowMovement"));

	ArrowMovement->ProjectileGravityScale = 0.02f;
	ArrowMovement->Velocity = FVector(2700.f,0.f, 0.f);

	Damage = 10.f;

	
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.f);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AArrow::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::CombatOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AArrow::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::CombatOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
