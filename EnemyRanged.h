// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyRanged.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AEnemyRanged : public AEnemy
{
	GENERATED_BODY()

public:

		AEnemyRanged();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI | Ranged")
			class USphereComponent* RangedAggroSphere;

		bool bOverlappingRangedSphere;

		UPROPERTY(EditDefaultsOnly, Category = "AI | Ranged")
			TSubclassOf<class AEnemyProjectile> EnemyProjectileToSpawn;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void RangedAggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void RangedAggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void RangedAttack();

	UFUNCTION(BlueprintCallable)
	void SpawnEnemyProjectile();

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
};
