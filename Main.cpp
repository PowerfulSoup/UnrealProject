// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "MainPlayerController.h"
#include "Shield.h"
#include "Block.h"
#include "LockedDoor.h"
#include "Tool.h"



// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 250.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.f;
	CameraBoom->CameraLagMaxDistance = 50.f;


	GetCapsuleComponent()->SetCapsuleSize(36.f, 102.f, true);
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//CameraVariables
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Configure Character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .1f;

	//initialize enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;
	EquipmentStatus = EEquipmentStatus::EES_Unarmed;

	//Initialize Stats
	Health = 75.f;
	MaxHealth = 100.f;
	Stamina = 200.f;
	MaxStamina = 200.f;
	Coins = 0.f;
	RunningSpeed = 375.f; //650 default
	SprintingSpeed = 750.f; //950 default
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	//Movement
	bShiftKeyDown = false;
	bMovingForward = false;
	bMovingRight = false;
	bEButtonDown = false;

	//Combat
	bLMBDown = false;
	bRMBDown = false;
	bAttacking = false;
	bBlocking = false;
	InterpSpeed = 15.f;
	bInterpToEnemy = false;
	bHasCombatTarget = false;

	//Puzzles
	bIsPushing = false;
	KeyCount = 0;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());


}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:

		if (bShiftKeyDown) //shift key down
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
			if (bMovingForward || bMovingRight)
			{
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else
			{
				SetMovementStatus(EMovementStatus::EMS_Normal);

			}
		}
		else //shift key up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:

		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0.f;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				if (bMovingForward || bMovingRight)
				{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);

				}
			}
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else// Shift key up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

	default:
		break;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AMain::RMBDown);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &AMain::RMBUp);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMain::EButtonDown);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AMain::EButtonUp);

	PlayerInputComponent->BindAction("Tool1", IE_Pressed, this, &AMain::EquipToolSlotOne);

	PlayerInputComponent->BindAction("Tool2", IE_Pressed, this, &AMain::EquipToolSlotTwo);

	PlayerInputComponent->BindAction("PutAwayItem", IE_Pressed, this, &AMain::PutAwayEquipment);

}

void AMain::MoveForward(float Value)
{
	bMovingForward = false;
	if ((Controller != nullptr) && (Value != 0.0f) && (!bBlocking) && (!bAttacking) && (MovementStatus != EMovementStatus::EMS_Dead))
	{
		//Find out which way is fwd
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);

			bMovingForward = true;
	}
}

void AMain::MoveRight(float Value)
{
	//Find out which way is right

	bMovingRight = false;
	if ((Controller != nullptr) && (Value != 0.0f) && (!bAttacking) && (!bBlocking) && (MovementStatus != EMovementStatus::EMS_Dead) && (MovementStatus != EMovementStatus::EMS_Dead))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

		bMovingRight = true;
	}
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMain::LMBDown()
{
	bLMBDown = true;
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	switch (EquipmentStatus)
	{
	case EEquipmentStatus::EES_Unarmed:

		break;

	case EEquipmentStatus::EES_SwordAndShield:
		if (EquippedWeapon)
		{
			Attack();
		}
		break;

	case EEquipmentStatus::EES_Tool:
		break;
	case EEquipmentStatus::EES_MAX:
		break;
	default:
		break;
	}



}

void AMain::LMBUp()
{
	bLMBDown = false;
}

void AMain::RMBDown() 
{
	bRMBDown = true;
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	if (EquippedShield)
	{
		Block();
	}
}

void AMain::RMBUp()
{
	bRMBDown = false;
}

void AMain::AttackEnd()
{
	bAttacking = false; 
	SetInterpToEnemy(false);

	if (bLMBDown)
	{
		Attack();
	}
}

void AMain::BlockEnd()
{
	if (bRMBDown) {

		Block();
	}

	else {

		//Play the animation to go from blocking stance to normal
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		SetInterpToEnemy(false);

		if (AnimInstance && CombatMontage) {

			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Block_End"), CombatMontage);

		}

		bBlocking = false;
	}
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

void AMain::SetEquippedShield(AShield* ShieldToSet)
{
	if (EquippedShield)
	{
		EquippedShield->Destroy();
	}
	EquippedShield = ShieldToSet;
}

void AMain::Attack()
{
	if (!bAttacking && !bBlocking && MovementStatus != EMovementStatus::EMS_Dead)
	{
		bAttacking = true; 
		SetInterpToEnemy(true); 

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			//int32 Selection = FMath::RandRange(0, 1);
			//switch (Selection)
			//{
			//case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.1f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
			//	break;
			//case 1:
			//	AnimInstance->Montage_Play(CombatMontage, 2.1f);
			//	AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
			//	break;
			//default:
			//	;
				//break;
			//}
		}

	}
}

void AMain::Block()
{
	{
		//if Player was not already blocking or attacking
		if (EquipmentStatus != EEquipmentStatus::EES_Unarmed && !bBlocking && !bAttacking && MovementStatus != EMovementStatus::EMS_Dead) {
			bBlocking = true;
			SetInterpToEnemy(true);

			//Play the "Going To Block" Animation
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance && CombatMontage) 
			{

				AnimInstance->Montage_Play(CombatMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("Block_Begin"), CombatMontage);

			}
		}
		//If already blocking,
		else {

			//Play the "Blocking Idle" animation
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance && CombatMontage) {
				AnimInstance->Montage_Play(CombatMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("Block_Idle"), CombatMontage);
			}
		}
	}
}

void AMain::DecrementHealth(float Amount)
{


}
	
void AMain::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMain::Jump()
{
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}

void AMain::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMain::IncrementHealth(float Amount)
{
	if (Health + Amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::SetEquipmentStatus(EEquipmentStatus Status)
{
	EquipmentStatus = Status;
}

void AMain::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

void AMain::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bBlocking)
	{
		DamageAmount *= (1.f - EquippedShield->DamageReduction);

	}
		if (Health - DamageAmount <= 0.f)
		{
			Health = 0.f;
			Die();

			if (DamageCauser)
			{
				AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
				if (Enemy)
				{
					Enemy->bHasValidTarget = false;
				}
			}
		}
		else
		{
			Health -= DamageAmount;
		}
		return DamageAmount;
}

void AMain::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
			//Put in reset camera here
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
					
				}
			}
		}
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}

FRotator AMain::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AMain::EButtonDown()
{
	bEButtonDown = true;
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
		AShield* Shield = Cast<AShield>(ActiveOverlappingItem);
		if (Shield)
		{
			Shield->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if (LockedDoorInRange)
	{
		UnlockDoor();
	}
}

void AMain::EButtonUp()
{
	bEButtonDown = false;

}

void AMain::BeginPushing()
{
	bIsPushing = true;

	switch (EquipmentStatus)
	{
	case EEquipmentStatus::EES_Unarmed:
		break;

	case EEquipmentStatus::EES_SwordAndShield:
		if (EquippedWeapon)
		{
			EquippedWeapon->SetActorHiddenInGame(true);
			EquippedShield->SetActorHiddenInGame(true);
		}
		break;

	case EEquipmentStatus::EES_Tool:
		break;
	case EEquipmentStatus::EES_MAX:
		break;
	default:
		break;
	}

}

void AMain::StopPushing()
{
	bIsPushing = false;

	switch (EquipmentStatus)
	{
	case EEquipmentStatus::EES_Unarmed:
		break;

	case EEquipmentStatus::EES_SwordAndShield:
		if (EquippedWeapon)
		{
			EquippedWeapon->SetActorHiddenInGame(false);
			EquippedShield->SetActorHiddenInGame(false);
		}

		break;

	case EEquipmentStatus::EES_Tool:
		break;
	case EEquipmentStatus::EES_MAX:
		break;
	default:
		break;
	}

}

void AMain::ChangeKeyCount(int Amount)
{
	KeyCount += Amount;
}

void AMain::UnlockDoor()
{
	if (KeyCount > 0)
	{
		LockedDoorInRange->UnlockDoor();
		ChangeKeyCount(-1);
	}
}

void AMain::EquipToolSlotOne()
{
	if (EquipmentStatus != EEquipmentStatus::EES_Tool)
	{
		ToolInSlotOne->Equip(this);
		SetCurrentActiveTool(ToolInSlotOne);
		SetEquipmentStatus(EEquipmentStatus::EES_Tool);

	}
}

void AMain::EquipToolSlotTwo()
{
	if (EquipmentStatus != EEquipmentStatus::EES_Tool)
	{
		ToolInSlotTwo->Equip(this);
		SetCurrentActiveTool(ToolInSlotTwo);
		SetEquipmentStatus(EEquipmentStatus::EES_Tool);
	}
}

void AMain::SetCurrentActiveTool(ATool* Tool)
{
	CurrentActiveTool = Tool;
}

void AMain::SetToolSlotOne(ATool* Tool)
{
	ToolInSlotOne = Tool;
}

void AMain::SetToolSlotTwo(ATool* Tool)
{
	ToolInSlotTwo = Tool;
}

void AMain::PutAwayEquipment()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && WorldMontage)
	{

		switch (EquipmentStatus)
		{
		case EEquipmentStatus::EES_Unarmed:
			AnimInstance->Montage_Play(WorldMontage, 1.8f);
			AnimInstance->Montage_JumpToSection(FName("Equip"), WorldMontage);

			SetEquipmentStatus(EEquipmentStatus::EES_SwordAndShield);
			

			break;
		case EEquipmentStatus::EES_SwordAndShield:
			AnimInstance->Montage_Play(WorldMontage, 1.8f);
			AnimInstance->Montage_JumpToSection(FName("UnEquip"), WorldMontage);

			SetEquipmentStatus(EEquipmentStatus::EES_Unarmed);

			break;
		case EEquipmentStatus::EES_Tool:

			CurrentActiveTool->Destroy();

			SetEquipmentStatus(EEquipmentStatus::EES_Unarmed);

			break;
		case EEquipmentStatus::EES_MAX:
			break;
		default:
			break;
		}
	}
}
