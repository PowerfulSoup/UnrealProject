// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyAnimInstance.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
#include "KillsFedDoor.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->InitSphereRadius(700.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	bOverlappingCombatSphere = false;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	HitBox->SetupAttachment(GetMesh(), FName("EnemyWeaponSocket"));

	Health = 100.f;
	MaxHealth = 100.f;
	Damage = 20.f;

	AttackMinTime = .5f;
	AttackMaxTime = 2.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	DeathDelay = 2.f;

	bHasValidTarget = false;
	bHooked = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AggroSphereOnOverlapBegin);
	AggroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AggroSphereOnOverlapEnd);
	AggroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
	CombatSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);


	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::WeaponHitBoxOnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::WeaponHitBoxOnOverlapEnd);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bAttacking && bHasValidTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, 15.f);

		SetActorRotation(InterpRotation);
	}

	if (bHooked)
	{
		AIController->StopMovement();
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpConstantTo(CurrentLocation, LocationToGrappleTo, DeltaTime, 2200.f);
		SetActorLocation(Interp);
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bHasValidTarget = false;
			//if (Main->CombatTarget == this)
			//{
			//	Main->SetCombatTarget(nullptr); ///COMMNENTED OUT FOR TS
			//}
			//Main->SetHasCombatTarget(false); 
			//Main->UpdateCombatTarget();

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if(AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				bHasValidTarget = true;
				if (AIController)
				{
					AIController->StopMovement();
				}
				//AIController->StopMovement();
				//Main->SetCombatTarget(this);
				//Main->SetHasCombatTarget(true); ////COMMENTED OUT FOR TS
				//Main->UpdateCombatTarget();

				CombatTarget = Main;
				bOverlappingCombatSphere = true;
				//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking); // I swapped this out from being switched within the Attack function every time its called

				float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

				if (bHooked)
				{
					bHooked = false;
				}
			}
	}

}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp)
	{
		AMain* Main = Cast<AMain>(OtherActor);

		if (Main)
		{
			bOverlappingCombatSphere = false; ////////////COMMENTED OUT FOR TS
			MoveToTarget(Main);
			CombatTarget = nullptr;
			bHasValidTarget = false;


			//if (Main->CombatTarget == this)
			//{
			//	Main->SetCombatTarget(nullptr);
			//	Main->bHasCombatTarget = false;
			//	Main->UpdateCombatTarget();
			//}

			if (Main->MainPlayerController)
			{
				USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (MainMesh)
				{
					Main->MainPlayerController->RemoveEnemyHealthBar();
				}
			}
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AEnemy::MoveToTarget(AMain* Target)
{
	if (AIController)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(Target);
			MoveRequest.SetAcceptanceRadius(10.0f);

			FNavPathSharedPtr NavPath;

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget); // NEW

			AIController->MoveTo(MoveRequest, &NavPath);

			//	AIController->MoveToActor(Target, CombatSphere->GetScaledSphereRadius());
		}

}

void AEnemy::WeaponHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				if (Main->HitParticles)
				{
					const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
					if (TipSocket)
					{
						FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.f), false);
					}
				}
				if (Main->HitSound)
				{
					UGameplayStatics::PlaySound2D(this, Main->HitSound);
				}
				if (DamageTypeClass)
				{
					UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
				}
			}
	}
}

void AEnemy::WeaponHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::ActivateCollision()
{
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, SwingSound);
	}
}

void AEnemy::DeactivateCollision()
{
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	//if (Alive() && bHasValidTarget)
	//{
	//	if (AIController)
	//	{
	//		AIController->StopMovement();
	//		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
	//	}
	//	if (!bAttacking)
	//	{
	//		bAttacking = true;
	//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//		if (AnimInstance)
	//		{
	//			AnimInstance->Montage_Play(CombatMontage, 1.f);
	//			AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
	//		}

	//	}
	//}

	
}

void AEnemy::AttackEnd()
{
	//bAttacking = false;
	//if (bOverlappingCombatSphere)
	//{
	//	float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);

	//	switch (DecideNextCombatAction())
	//	{
	//	case 1://Attack Again
	//		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	//		break;

	//	case 2://Attack Again
	//		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	//		break;
	//		
	//	case 3: //Attack Again
	//		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	//		break;

	//	//case 4: //Strafe
	//	//	Strafe();
	//	//	break;

	//	//case 5: //Strafe
	//	//	Strafe();
	//	//	break;

	//	default:
	//		break;
	//	}

	//}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0;
		Die(DamageCauser);
	}
	else
	{
		Health -= DamageAmount;

	}

	return DamageAmount;
}

void AEnemy::Die(AActor* Causer)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, .70f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}

	if (DoorToFeed)
	{
		FeedDoor();
	}

	CombatTarget->UpdateCombatTarget(nullptr);
	bAttacking = false;

	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AggroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	AMain* Main = Cast<AMain>(CombatTarget); // Changed parater from Causer to Combat Target
	if (Main)
	{

		Main->UpdateCombatTarget();
	}

}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	DropLoot();
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	Destroy();
}

void AEnemy::FeedDoor()
{
	DoorToFeed->UpdateKillCount();
}

FRotator AEnemy::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

int32 AEnemy::DecideNextCombatAction()
{
	int32 NextAction = FMath::RandRange(1, 100);
	NextAction /= 10;
	FMath::RoundToInt(NextAction);
	return NextAction;
}

void AEnemy::Strafe()
{
	DecideNextCombatAction();
}

void AEnemy::GetGrappled(FVector Location)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Grappled"), CombatMontage);
	}
	LocationToGrappleTo = Location;
	bHooked = true;
}
