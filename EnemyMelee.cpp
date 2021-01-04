// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMelee.h"
#include "Main.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"


AEnemyMelee::AEnemyMelee()
{

}

void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyMelee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyMelee::SetEnemyMovementStatus(EEnemyMovementStatus Status)
{
	Super::SetEnemyMovementStatus(Status);
}

EEnemyMovementStatus AEnemyMelee::GetEnemyMovementStatus()
{
	Super::GetEnemyMovementStatus();
	return EEnemyMovementStatus();
}

void AEnemyMelee::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::AggroSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AEnemyMelee::AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::AggroSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AEnemyMelee::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AEnemyMelee::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::CombatSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AEnemyMelee::MoveToTarget(AMain* Target)
{
	Super::MoveToTarget(Target);
}

void AEnemyMelee::WeaponHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::WeaponHitBoxOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AEnemyMelee::WeaponHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::WeaponHitBoxOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AEnemyMelee::ActivateCollision()
{
	Super::ActivateCollision();
}

void AEnemyMelee::DeactivateCollision()
{
	Super::DeactivateCollision();
}

void AEnemyMelee::Attack()
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

void AEnemyMelee::AttackCombo()
{
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
				AnimInstance->Montage_JumpToSection(FName("Combo"), CombatMontage);
			}

		}
	}
}

void AEnemyMelee::AttackEnd()
{
	Super::AttackEnd();
		bAttacking = false;
		if (bOverlappingCombatSphere)
		{
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);

			switch (DecideNextCombatAction())
			{
			case 1://Attack Again
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
				break;

			case 2://Attack Again
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
				break;
		
			case 3: //Attack Again w/Combo
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyMelee::AttackCombo, AttackTime);
				break;

			//case 4: //Strafe
			//	Strafe();
			//	break;

			//case 5: //Strafe
			//	Strafe();
			//	break;

			default:
				break;
			}
		}
}

void AEnemyMelee::Strafe()
{
	Super::Strafe();
}

int32 AEnemyMelee::DecideNextCombatAction()
{
	Super::DecideNextCombatAction();
	int32 NextAction = FMath::RandRange(1, 3);
	return NextAction;
}

float AEnemyMelee::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void AEnemyMelee::Die(AActor* Causer)
{
	Super::Die(Causer);
}

void AEnemyMelee::DeathEnd()
{
	Super::DeathEnd();
}

bool AEnemyMelee::Alive()
{
	Super::Alive();
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemyMelee::Disappear()
{
	Super::Disappear();
}

FRotator AEnemyMelee::GetLookAtRotationYaw(FVector Target)
{
	Super::GetLookAtRotationYaw(Target);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AEnemyMelee::FeedDoor()
{
	Super::FeedDoor();
}
