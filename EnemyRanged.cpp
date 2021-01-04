// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRanged.h"
#include "Components/SphereComponent.h"
#include "Main.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "EnemyProjectile.h"
#include "Engine/World.h"


AEnemyRanged::AEnemyRanged()
{
	PrimaryActorTick.bCanEverTick = true;

	RangedAggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Ranged Aggro Sphere"));
	RangedAggroSphere->SetupAttachment(GetRootComponent());
	//RangedAggroSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//RangedAggroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	bOverlappingRangedSphere = false;
}

void AEnemyRanged::BeginPlay()
{
	Super::BeginPlay();

	RangedAggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyRanged::RangedAggroSphereOnOverlapBegin);
	RangedAggroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyRanged::RangedAggroSphereOnOverlapEnd);
	RangedAggroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
}


void AEnemyRanged::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyRanged::RangedAggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bHasValidTarget = true;
			CombatTarget = Main;
			bOverlappingRangedSphere = true;

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Shooting);

			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyRanged::RangedAttack, AttackTime);
		}
	}
}

void AEnemyRanged::RangedAggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bOverlappingRangedSphere = false;
			MoveToTarget(Main);
			CombatTarget = nullptr;
			bHasValidTarget = false;

			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}


void AEnemyRanged::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyRanged::SetEnemyMovementStatus(EEnemyMovementStatus Status)
{
	Super::SetEnemyMovementStatus(Status);
}

EEnemyMovementStatus AEnemyRanged::GetEnemyMovementStatus()
{
	Super::GetEnemyMovementStatus();
	return EEnemyMovementStatus();
}

void AEnemyRanged::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::AggroSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,SweepResult);
}

void AEnemyRanged::AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::AggroSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bHasValidTarget = true;
			CombatTarget = Main;
			bOverlappingRangedSphere = true;

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Shooting);

			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyRanged::RangedAttack, AttackTime);
		}
	}
}

void AEnemyRanged::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AEnemyRanged::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::CombatSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AEnemyRanged::MoveToTarget(AMain* Target)
{
	Super::MoveToTarget(Target);
}

void AEnemyRanged::WeaponHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::WeaponHitBoxOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AEnemyRanged::WeaponHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::WeaponHitBoxOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AEnemyRanged::ActivateCollision()
{
	Super::ActivateCollision();
}

void AEnemyRanged::DeactivateCollision()
{
	Super::DeactivateCollision();
}

void AEnemyRanged::Attack()
{
	Super::Attack();
	if (Alive() && bHasValidTarget)
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.f);
				AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
			}

		}
	}
}

void AEnemyRanged::RangedAttack()
{
	if (Alive() && bHasValidTarget)
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Shooting);
		}
		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.f);
				AnimInstance->Montage_JumpToSection(FName("Shoot"), CombatMontage);
			}

		}
	}

}

void AEnemyRanged::SpawnEnemyProjectile()
{
	FRotator Direction = GetMesh()->GetSocketRotation("EnemyWeaponSocket");
	FVector Location = GetMesh()->GetSocketLocation("EnemyWeaponSocket");
	FActorSpawnParameters SpawnInfo;

	AEnemyProjectile* SpawnedEnemyProj = GetWorld()->SpawnActor<AEnemyProjectile>(EnemyProjectileToSpawn->GetDefaultObject()->GetClass(), Location, Direction, SpawnInfo);
	SpawnedEnemyProj->SetProjectileInstigator(GetController());
}

void AEnemyRanged::AttackEnd()
{
	Super::AttackEnd();

	bAttacking = false;
	float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);

	switch (EnemyMovementStatus)
	{
	case EEnemyMovementStatus::EMS_Idle:
		break;
	case EEnemyMovementStatus::EMS_MoveToTarget:
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

		break;
	case EEnemyMovementStatus::EMS_Attacking:
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

		break;

	case EEnemyMovementStatus::EMS_Shooting:
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyRanged::RangedAttack, AttackTime);

		break;

	case EEnemyMovementStatus::EMS_Dead:
		break;
	case EEnemyMovementStatus::EMS_MAX:
		break;
	default:
		break;
	}
}

void AEnemyRanged::Strafe()
{
	Super::Strafe();
}

int32 AEnemyRanged::DecideNextCombatAction()
{
	Super::DecideNextCombatAction();
	int32 NextAction = FMath::RandRange(1, 3);
	return NextAction;
}

float AEnemyRanged::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void AEnemyRanged::Die(AActor* Causer)
{
	Super::Die(Causer);
}

void AEnemyRanged::DeathEnd()
{
	Super::DeathEnd();
}

bool AEnemyRanged::Alive()
{
	Super::Alive();
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemyRanged::Disappear()
{
	Super::Disappear();
}

FRotator AEnemyRanged::GetLookAtRotationYaw(FVector Target)
{
	Super::GetLookAtRotationYaw(Target);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AEnemyRanged::FeedDoor()
{
	Super::FeedDoor();
}
