// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Main.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


ABoss::ABoss()
{
	IntroCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Intro Camera"));
	IntroCamera->SetupAttachment(GetRootComponent());

	AuraParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AuraParticles"));
	AuraParticleSystemComponent->SetupAttachment(GetRootComponent());

	WeaponParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponParticles"));
	WeaponParticles->SetupAttachment(GetMesh(), FName("EnemyWeaponSocket"));

	UtilityHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UtilityHitBox"));
	UtilityHitBox->SetupAttachment(GetMesh(), FName("hand_l"));

	AttackSpeed = 1.1f;

	NumberOfUnderlingsToSpawn = 1;
}


void ABoss::BeginPlay()
{
	Super::BeginPlay();
	AuraParticleSystemComponent->Deactivate();
	WeaponParticles->Deactivate();
	BossState = EBossState::EBS_Normal;

	UtilityHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABoss::UtilityHitBoxOnOverlapBegin);
	UtilityHitBox->OnComponentEndOverlap.AddDynamic(this, &ABoss::UtilityHitBoxOnOverlapEnd);
	UtilityHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UtilityHitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	UtilityHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	UtilityHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoss::SetEnemyMovementStatus(EEnemyMovementStatus Status)
{
	Super::SetEnemyMovementStatus(Status);
}

EEnemyMovementStatus ABoss::GetEnemyMovementStatus()
{
	Super::GetEnemyMovementStatus();
	return EEnemyMovementStatus();
}

void ABoss::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::AggroSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void ABoss::AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::AggroSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ABoss::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABoss::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::CombatSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ABoss::MoveToTarget(AMain* Target)
{

		Super::MoveToTarget(Target);
	
}

void ABoss::WeaponHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::WeaponHitBoxOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABoss::WeaponHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::WeaponHitBoxOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ABoss::ActivateCollision()
{
	Super::ActivateCollision();
}

void ABoss::DeactivateCollision()
{
	Super::DeactivateCollision();
}

void ABoss::Attack()
{
	Super::Attack();
	if (Alive() && bHasValidTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Begin"));


		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
			UE_LOG(LogTemp, Warning, TEXT("Set to Attacking"));

		}
		if (!bAttacking)
		{
			bAttacking = true;
			UE_LOG(LogTemp, Warning, TEXT("battacking=true"));


			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				switch (DecideNextCombatAction())
				{
				case 1://Attack Again
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);

					UE_LOG(LogTemp, Warning, TEXT("Attack 1"));

					break;

				case 2://Attack Again
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);

					UE_LOG(LogTemp, Warning, TEXT("attack2"));

					break;

				case 3: //Attack Again 
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack3"), CombatMontage);

					UE_LOG(LogTemp, Warning, TEXT("atack3"));

					break;

				case 4: //Attack Again 
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack4"), CombatMontage);
					break;

				case 5://Attack Again
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);

					UE_LOG(LogTemp, Warning, TEXT("Attack 1"));

					break;

				case 6://Attack Again
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);

					UE_LOG(LogTemp, Warning, TEXT("attack2"));

					break;

				case 7: //Attack Again 
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack3"), CombatMontage);

					UE_LOG(LogTemp, Warning, TEXT("atack3"));

					break;

				case 8: //Attack Again 
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack4"), CombatMontage);
					break;

				case 9: //Attack Again 
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack3"), CombatMontage);

					UE_LOG(LogTemp, Warning, TEXT("atack3"));

					break;

				case 10: //Attack Again 
					AnimInstance->Montage_Play(CombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack4"), CombatMontage);
					break;

				default:
					break;
				}

			}

		}
	}
}

void ABoss::AttackEnd()
{
	Super::AttackEnd();
	bAttacking = false;
	UE_LOG(LogTemp, Warning, TEXT("attackend"));

	if (bOverlappingCombatSphere)
	{
		UE_LOG(LogTemp, Warning, TEXT("attackendoverlap true"));

		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);

		switch (DecideNextCombatAction())
		{
		case 1://Attack Again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			break;

		case 2://Attack Again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			break;

		case 3: //Attack Again 
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			break;

		case 4: //Attack Again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

			break;

		case 5: //Attack Again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			
			break;

		case 6: //Attack Again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

			break;

		case 7: //Attack Again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			
			break;

		case 8: //Summon
			SummonUnderlings();
			break;

		case 9: //Attack again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			break;

		case 10: //Attack again
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			break;

		default:
			break;
		}
	}
}

void ABoss::Strafe()
{
	Super::Strafe();
	UE_LOG(LogTemp, Warning, TEXT("strafe"));

}

int32 ABoss::DecideNextCombatAction()
{
	Super::DecideNextCombatAction();
	int32 NextAction = FMath::RandRange(1, 10);
	UE_LOG(LogTemp, Warning, TEXT("Deciding Combat action"));

	return NextAction;
}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (BossState != EBossState::EBS_Transitioning && EnemyMovementStatus!= EEnemyMovementStatus::EMS_Dead)
	{
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


		CheckTransitionRequirements();
		return DamageAmount;
	}
	else
	{
		return 0.f;
	}
}

void ABoss::Die(AActor* Causer)
{
	//for (AEnemy* Enemy : UnderlingArray)
	//{
	//	Enemy->Die(this);
	//}
	CombatTarget->bHasCombatTarget = false;
	AuraParticleSystemComponent->Deactivate();
	WeaponParticles->Deactivate();

	Super::Die(Causer);
}

void ABoss::DeathEnd()
{
	Super::DeathEnd();
}

bool ABoss::Alive()
{
	Super::Alive();
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void ABoss::Disappear()
{
	Super::Disappear();
}

FRotator ABoss::GetLookAtRotationYaw(FVector Target)
{
	Super::GetLookAtRotationYaw(Target);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void ABoss::FeedDoor()
{
	Super::FeedDoor();
}

void ABoss::GetGrappled(FVector Location)
{
	Super::GetGrappled(Location);
}

void ABoss::IntroAnimationStart()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(StageTransitionMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Introduction"), StageTransitionMontage);
	}
}

void ABoss::TransitionStage()
{
	if (AIController)
	{
		AIController->StopMovement();
		//SetBossState(EBossState::EBS_PissedOff);
		bAttacking = true;

		AttackSpeed = 1.6f;
		AttackMinTime = .2f;
		AttackMaxTime = 1.f;
		CombatMontage = PissedOffMontage;
		//GetCharacterMovement()->MaxWalkSpeed = 400.f;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Transition True"));

			AnimInstance->Montage_Play(StageTransitionMontage, 1.4f);
			AnimInstance->Montage_JumpToSection(FName("Transition"), StageTransitionMontage);
		}
	}
}

void ABoss::CheckTransitionRequirements()
{
	float HealthThreshold = Health / MaxHealth;
	UE_LOG(LogTemp, Warning, TEXT("Transition check"));

	if (HealthThreshold <= .50f && BossState != EBossState::EBS_PissedOff)
	{
		AIController->StopMovement();

		SetBossState(EBossState::EBS_Transitioning);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->StopAllMontages(.5f);
		}
		TransitionStage();

	}
}

void ABoss::TurnOnParticles()
{
	//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
	SetBossState(EBossState::EBS_PissedOff);

	AuraParticleSystemComponent->Activate();
	WeaponParticles->Activate();
}

void ABoss::SummonUnderlings()
{
	if (Alive())
	{
		int32 NumberOfUnderlingsSpawned = 0;
		while (NumberOfUnderlingsSpawned <= NumberOfUnderlingsToSpawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("underling summoned!"));
			FVector Location = (GetActorLocation());
			Location.X += FMath::FRandRange(20.f, 75.f);
			Location.Y += FMath::FRandRange(20.f, 75.f);
			FRotator Direction = FRotator(0.f, 0.f, 0.f);
			FActorSpawnParameters SpawnInfo;

			AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(UnderlingToSpawn->GetDefaultObject()->GetClass(), Location, Direction, SpawnInfo);

			NumberOfUnderlingsSpawned++;
			UnderlingArray.Add(SpawnedEnemy);
		}
	}
}

void ABoss::UtilityHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, 5.f, AIController, this, DamageTypeClass);
				Main->Staggered();
			}
		}
	}
}

void ABoss::UtilityHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABoss::ActivateUtilityHitBoxCollision()
{
	UtilityHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABoss::DeactivateUtilityHitBoxCollision()
{
	UtilityHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
