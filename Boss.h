// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boss.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	EBS_Normal UMETA(Display = "Normal"),
	EBS_PissedOff UMETA(Display = "PissedOff"),
	EBS_Transitioning UMETA(Display = "Transitioning"),

	EBS_MAX UMETA(Display = "DefaultMAX")
};

UCLASS()
class RPG_API ABoss : public AEnemy
{
	GENERATED_BODY()
	
public:

	ABoss();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Boss")
		EBossState BossState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Boss")
		class UCameraComponent* IntroCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Boss")
		class UBoxComponent* UtilityHitBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy | Boss")
		class UAnimMontage* PissedOffMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Enemy | Boss")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Boss")
		class UAnimMontage* StageTransitionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Boss")
		class UParticleSystemComponent* AuraParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Boss")
		UParticleSystemComponent* WeaponParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Boss")
		TSubclassOf<class AEnemy> UnderlingToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Boss")
		int32 NumberOfUnderlingsToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Boss")
		TArray<AEnemy*> UnderlingArray;
	
protected:

	virtual void BeginPlay() override;

public:

	///INHERITED FROM BASE ENEMYCLASS///
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetEnemyMovementStatus(EEnemyMovementStatus Status)override;
	virtual EEnemyMovementStatus GetEnemyMovementStatus()override;

	virtual void AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void MoveToTarget(class AMain* Target)override;

	virtual void WeaponHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void WeaponHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void ActivateCollision()override;
	virtual void DeactivateCollision()override;

	///COMBAT FUNCTIONS
	virtual void Attack()override;
	virtual void AttackEnd()override;

	virtual void Strafe()override;

	virtual int32 DecideNextCombatAction()override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die(AActor* Causer)override;

	virtual void DeathEnd()override;

	virtual bool Alive()override;

	virtual void Disappear()override;

	virtual FRotator GetLookAtRotationYaw(FVector Target)override;

	virtual void FeedDoor()override;

	//Tool Interactions
	virtual void GetGrappled(FVector Location)override;


	//BOSS FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void IntroAnimationStart();

	FORCEINLINE void SetBossState(EBossState State) { BossState = State; }
	FORCEINLINE EBossState GetBossState() { return BossState; }

	void TransitionStage();

	void CheckTransitionRequirements();

	UFUNCTION(BlueprintCallable)
	void TurnOnParticles();

	UFUNCTION(BlueprintCallable)
	void SummonUnderlings();

	UFUNCTION()
	void UtilityHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void UtilityHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateUtilityHitBoxCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateUtilityHitBoxCollision();
};
