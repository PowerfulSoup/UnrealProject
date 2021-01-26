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
#include "Components/SphereComponent.h"
#include "Tool.h"
#include "Bow.h"
#include "Interactable.h"

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
	bIsZoomed = false;

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
	RunningSpeed = 245.f; //325,650 default
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
	InterpSpeed = 30.f;
	bInterpToEnemy = false;
	bHasCombatTarget = false;

	//Puzzles
	bIsPushing = false;
	KeyCount = 0;

	//tools
	BombCount = 5;
	BombCountMax = 25;
	ArrowAmmo = 12;
	ArrowAmmoMax = 50;
	
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());
	Health = 75.f;

	CurrentToolMontage = WorldMontage;
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

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
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

	PlayerInputComponent->BindAction("PutAwayItem", IE_Pressed, this, &AMain::PutAwayEquipment);
}

void AMain::MoveForward(float Value)
{
	bMovingForward = false;
	if ((Controller != nullptr) && (Value != 0.0f) && (!bIsZoomed) &&(!bBlocking) && (!bAttacking) && (MovementStatus != EMovementStatus::EMS_Dead))
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
	bMovingRight = false;
	if ((Controller != nullptr) && (Value != 0.0f) && (!bIsZoomed) && (!bAttacking) && (!bBlocking) && (MovementStatus != EMovementStatus::EMS_Dead) && (MovementStatus != EMovementStatus::EMS_Dead))
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
	if (MovementStatus == EMovementStatus::EMS_Dead ) return;

	switch (EquipmentStatus)
	{
	case EEquipmentStatus::EES_Unarmed:
		if (EquippedWeapon == nullptr)
		{
			break;
		}
		PutAwayEquipment();
		break;

	case EEquipmentStatus::EES_SwordAndShield:
		if (EquippedWeapon && !bBlocking)
		{
			Attack();
		}
		break;

	case EEquipmentStatus::EES_Tool:
		if (CurrentActiveTool)
		{
			if (CurrentActiveTool->ToolName == "GrapplingHook" || ArrowAmmoCheck() && CurrentActiveTool->ToolName == "Bow" || BombCountCheck() && CurrentActiveTool->ToolName == "Bombs")
			{
				PrimaryToolFunction();
			}
		}
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

	switch (EquipmentStatus)
	{
	case EEquipmentStatus::EES_Unarmed:
		break;
	case EEquipmentStatus::EES_SwordAndShield:
		if (EquippedShield && !bAttacking)
		{
			Block();
		}
		break;

	case EEquipmentStatus::EES_Tool:
		if (CurrentActiveTool)
		{
			SecondaryToolFunction();
		}
		break;

	case EEquipmentStatus::EES_MAX:
		break;
	default:
		break;
	}
}

void AMain::RMBUp()
{
	bRMBDown = false;
}

void AMain::AttackEnd()
{


	if (!bLMBDown)
	{
		bAttacking = false; 
		SetInterpToEnemy(false);
		GetMesh()->GetAnimInstance()->StopSlotAnimation(.25f, "Combat");
	}
}

void AMain::BlockEnd()
{
	if (bRMBDown) 
	{
		bShouldLoopBlock = true;
		Block();
	}

	else {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		SetInterpToEnemy(false);

		if (AnimInstance && CombatMontage) 
		{
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Block_End"), CombatMontage);
		}
		bBlocking = false;
		bShouldLoopBlock = false;
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

			if (AnimInstance)
			{
				if (bShiftKeyDown)
				{
					AnimInstance->Montage_Play(SwordMontage, 1.f);
					AnimInstance->Montage_JumpToSection(FName("OverheadAttack"), SwordMontage);

					JumpAttack();
				}
				else
				{
					AnimInstance->Montage_Play(SwordMontage, 1.f);
					AnimInstance->Montage_JumpToSection(FName("Attack1"), SwordMontage);
				}
			}	
	}
}

void AMain::JumpAttackMoveForward(float Value)
{

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	AddMovementInput(Direction, Value);
}

void AMain::Block()
{
	{
		if (EquipmentStatus != EEquipmentStatus::EES_Unarmed && !bBlocking && !bAttacking && MovementStatus != EMovementStatus::EMS_Dead) {

			bBlocking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance && CombatMontage ) //&& bShouldLoopBlock
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

void AMain::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	if (bIsZoomed)
	{
		ToggleZoomCamera();
	}
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bBlocking)
	{
		DamageAmount *= (1.f - EquippedShield->DamageReduction);
		EquippedShield->SpawnBlockEmitter();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.7f);
			AnimInstance->Montage_JumpToSection(FName("Block_Impact"), CombatMontage);
		}
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
	UE_LOG(LogTemp, Warning, TEXT("overloaded combat target"));
	SetHasCombatTarget(false);
	SetCombatTarget(nullptr);
}


void AMain::UpdateCombatTarget(AActor* Target)
{
	AEnemy* Enemy = Cast<AEnemy>(Target);
	if (Enemy)
	{
		SetHasCombatTarget(true);
		SetCombatTarget(Enemy);
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
	else if (CurrentInteractable)
	{
		CurrentInteractable->InteractFunction(this);
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
		if (CurrentActiveTool)
		{
			CurrentActiveTool->SetActorHiddenInGame(true);
		}
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
		if (CurrentActiveTool)
		{
			
			CurrentActiveTool->SetActorHiddenInGame(false);
		}
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


void AMain::EquipToolSlotOne()
{
	if (ToolToUse) 
	{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (EquipmentStatus != EEquipmentStatus::EES_Tool)
		{
			switch (EquipmentStatus)
			{
			case EEquipmentStatus::EES_Unarmed:
					AnimInstance->Montage_Play(WorldMontage, 1.f);
					AnimInstance->Montage_JumpToSection(FName("Equip_OffHand"), WorldMontage);

				break;

			case EEquipmentStatus::EES_SwordAndShield:
				PutAwayEquipment();

				break;

			case EEquipmentStatus::EES_Tool:
				AnimInstance->Montage_Play(WorldMontage, 1.f);
				AnimInstance->Montage_JumpToSection(FName("UnEquip_OffHand"), WorldMontage);
				CurrentActiveTool->Destroy();
				CurrentActiveTool = nullptr;
				SetEquipmentStatus(EEquipmentStatus::EES_Unarmed);
				SetCurrentToolMontage(WorldMontage);

				break;
			case EEquipmentStatus::EES_MAX:
				break;
			default:
				break;
			}
		}
		else
		{
			AnimInstance->Montage_Play(WorldMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("UnEquip_OffHand"), WorldMontage);
			CurrentActiveTool->Destroy();
			CurrentActiveTool = nullptr;
			SetEquipmentStatus(EEquipmentStatus::EES_Unarmed);
			SetCurrentToolMontage(WorldMontage);
		}
	}
}

void AMain::SetCurrentActiveTool(ATool* Tool)
{
	CurrentActiveTool = Tool;
}

void AMain::SetToolSlotOne(UClass* Tool)
{
	ToolToUse = Tool->GetClass();//to use
}

void AMain::PutAwayEquipment()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		switch (EquipmentStatus)
		{
		case EEquipmentStatus::EES_Unarmed:
			DisableInput(MainPlayerController);

			AnimInstance->Montage_Play(WorldMontage, 1.8f);
			AnimInstance->Montage_JumpToSection(FName("Equip"), WorldMontage);

			SetEquipmentStatus(EEquipmentStatus::EES_SwordAndShield);
			

			break;
		case EEquipmentStatus::EES_SwordAndShield:
			DisableInput(MainPlayerController);

			AnimInstance->Montage_Play(WorldMontage, 1.8f);
			AnimInstance->Montage_JumpToSection(FName("UnEquip"), WorldMontage);

			SetEquipmentStatus(EEquipmentStatus::EES_Unarmed);

			break;

		case EEquipmentStatus::EES_Tool:
			break;

		case EEquipmentStatus::EES_MAX:
			break;
		default:
			break;
		}
	}

	if (bIsZoomed)
	{
		ToggleZoomCamera();
	}
}

void AMain::PrimaryToolFunction()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CurrentToolMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("PrimaryBegin"), CurrentToolMontage);
	}
}

void AMain::SecondaryToolFunction()
{
	if (CurrentActiveTool->ToolName == "Bow" || CurrentActiveTool->ToolName == "GrapplingHook")
	{
		ToggleZoomCamera();
	}
	else 
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(CurrentToolMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("SecondaryBegin"), CurrentToolMontage);
		}
	}

}

void AMain::SetCombatTarget(AEnemy* Target)
{
	CombatTarget = Target;
}

void AMain::SetHasCombatTarget(bool HasTarget)///GET RID OF??!?!
{
	UE_LOG(LogTemp, Warning, TEXT("Setcombattarget"));

	bHasCombatTarget = HasTarget;
}

void AMain::TriggerToUpdateCombatTarget(AActor* Target)
{
	if (Target)
	{
		UpdateCombatTarget(Target);
	}
	else 
	{
		UpdateCombatTarget();
	}
}

void AMain::SetCurrentToolMontage(UAnimMontage* MontageToSet)
{
	CurrentToolMontage = MontageToSet;
}

void AMain::ToggleZoomCamera()
{
	if (!bIsZoomed)
	{
		CameraBoom->TargetArmLength = 0.f;
		CameraBoom->SocketOffset = FVector(25.f, 15.f, 75.f);
		bUseControllerRotationYaw = true;
		bUseControllerRotationPitch = true;

		bIsZoomed = true;
	}
	else
	{
		CameraBoom->TargetArmLength = 250.f;
		CameraBoom->SocketOffset = FVector(0.f, 65.f, 55.f);
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;

		float CurrentYaw = GetActorRotation().Yaw;
		float CurrentRoll = GetActorRotation().Roll;
		SetActorRotation(FRotator(0.f, CurrentYaw, CurrentRoll));

		bIsZoomed = false;
	}
}

void AMain::SpawnTool()
{
	ATool* SpawnedTool;
	SpawnedTool = GetWorld()->SpawnActor<ATool>(ToolToUse->GetDefaultObject()->GetClass(), FActorSpawnParameters());

	SpawnedTool->SetToolOwner(this);
	SpawnedTool->SetInstigator(MainPlayerController);

	SpawnedTool->Equip(this);
	SetCurrentActiveTool(SpawnedTool);
	SetCurrentToolMontage(CurrentActiveTool->ToolMontage);
}

void AMain::UpdateBowAmmo(int32 Amount)
{
	if (Amount + ArrowAmmo >= ArrowAmmoMax)
	{
		ArrowAmmo = ArrowAmmoMax;
	}
	else
	{
		ArrowAmmo += Amount;
	}
}

void AMain::UpdateBombAmmo(int32 Amount)
{
	if (Amount + BombCount >= BombCountMax)
	{
		BombCount = BombCountMax;
	}
	else
	{
		BombCount += Amount;
	}
}

bool AMain::ArrowAmmoCheck()
{
	if (ArrowAmmo - 1 < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool AMain::BombCountCheck()
{
	if (BombCount - 1 < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void AMain::RecallGrapplingHook()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CurrentToolMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Pull"), CurrentToolMontage);
	}
}

void AMain::SetCurrentInteractable(AInteractable* ItemToInteract)
{
	CurrentInteractable = ItemToInteract;
}
