// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(Display = "Normal"),
	EMS_Sprinting UMETA(Display = "Sprinting"),
	EMS_Climbing UMETA(Display = "Climbing"),
	EMS_Dead UMETA(Display = "Dead"),

	EMS_MAX UMETA(Display = "Default MAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "Below Minimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ESS_ExhaustedRecovering"),

	ESS_MAX UMETA(DisplayName = "Default MAX")
};

UENUM(BlueprintType)
enum class EEquipmentStatus : uint8
{
	EES_Unarmed UMETA(DisplayName = "Unarmed"),
	EES_SwordAndShield UMETA(DisplayName = "SwordAndShield"),
	EES_Tool UMETA(DisplayName = "Tool"),

	EES_MAX UMETA(DisplayName = "Default MAX")
};

UCLASS()
class RPG_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	//Components and Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//Turn and Look Rates to scale turning functions for Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		bool bIsZoomed;

	//Enums
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EEquipmentStatus EquipmentStatus;

	//Player Stats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int32 ArrowAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int32 ArrowAmmoMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int32 BombCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int32 BombCountMax;

	//Movement Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	bool bShiftKeyDown;


	//Puzzle Variables

	UPROPERTY(BlueprintReadOnly)
		bool bEButtonDown;

	UPROPERTY(BlueprintReadOnly)
		bool bIsPushing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzles")
		int KeyCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzles")
		class ALockedDoor* LockedDoorInRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World")
		class AInteractable* CurrentInteractable;


	//Combat Varibles
	UPROPERTY(BlueprintReadOnly)
	bool bAttacking;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bBlocking;

	bool bShouldLoopBlock;

	UPROPERTY(BlueprintReadOnly)
	bool bLMBDown;

	bool bRMBDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bHasCombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class USoundCue* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class AEnemy* CombatTarget;

	float InterpSpeed;
	bool bInterpToEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		FVector CombatTargetLocation;

	bool bLockedOn;

	//Item Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AShield* EquippedShield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;

	//Tool Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tools")
		class ATool* CurrentActiveTool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tools")
		ATool* ToolInSlotOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tools")//use instead of toolinslotone?
		TSubclassOf<ATool> ToolToUse;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Navigation
	void MoveForward(float Value);
	bool bMovingForward;

	void MoveRight(float Value);
	bool bMovingRight;

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	//Enum Setters
	void SetMovementStatus(EMovementStatus);

	void SetEquipmentStatus(EEquipmentStatus);

	//Character World Functions
	UFUNCTION(BlueprintCallable)
		void IncrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable)
		void IncrementHealth(float Amount);

	virtual void Jump() override;

	//Seconday Movement Functions
	void ShiftKeyDown();
	void ShiftKeyUp();

	void EButtonDown();
	void EButtonUp();

	//Animation Functions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		UAnimMontage* SwordMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		UAnimMontage* WorldMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
		UAnimMontage* CurrentToolMontage;

	UFUNCTION(BlueprintCallable)
		void SetCurrentToolMontage(UAnimMontage* MontageToSet);

	//Combat Functions
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void JumpAttack();

	UFUNCTION(BlueprintCallable)
	void JumpAttackMoveForward(float Value);

	void Block();

	UFUNCTION(BlueprintCallable)
	void BlockEnd();

	void LMBDown();
	void LMBUp();

	void RMBDown();
	void RMBUp();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	void SetInterpToEnemy(bool Interp);

	FRotator GetLookAtRotationYaw(FVector Target);

	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	void UpdateCombatTarget(AActor* Target);
	void UpdateCombatTarget();

	UFUNCTION(BlueprintCallable)
		void TriggerToUpdateCombatTarget(AActor* Target);

	UFUNCTION(BlueprintCallable)
		void PlaySwingSound();

	void Staggered();

	//Item Functions
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	void SetEquippedShield(AShield* ShieldToSet);
	FORCEINLINE AShield* GetEquippedShield() { return EquippedShield; }

	UFUNCTION(BlueprintCallable)
		void UpdateBowAmmo(int32 Amount);

	UFUNCTION(BlueprintCallable)
		void UpdateBombAmmo(int32 Amount);

	//TOOL Functions

	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveTool(ATool* Tool);
	UFUNCTION(BlueprintCallable)
	void EquipToolSlotOne();
	void PutAwayEquipment();
	UFUNCTION(BlueprintCallable)
	void SetToolSlotOne(UClass* Tool);
	UFUNCTION(BlueprintCallable)
	void PrimaryToolFunction();
	UFUNCTION(BlueprintCallable)
	void SecondaryToolFunction();
	UFUNCTION(BlueprintCallable)
	void SpawnTool();
	UFUNCTION()
	void ToggleZoomCamera();
	UFUNCTION()
	void RecallGrapplingHook();


	bool ArrowAmmoCheck();
	bool BombCountCheck();

	//Other inline Methods
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

	//COMBAT ENEMY FUNCTIONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<AEnemy> EnemyFilter;

	UFUNCTION(BlueprintCallable)
	void SetCombatTarget(AEnemy* Target);

	UFUNCTION(BlueprintCallable)
	void SetHasCombatTarget(bool HasTarget);

	//PUZZLE FUNCTIONS

	void BeginPushing();
	void StopPushing();
	
	UFUNCTION(BlueprintCallable)
	void ChangeKeyCount(int Amount);

	UFUNCTION()
		void UnlockDoor();

	UFUNCTION()
		void SetCurrentInteractable(AInteractable* ItemToInteract);


};
