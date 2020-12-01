// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_DefaultMAX	UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

AWeapon();

UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
EWeaponState WeaponState;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
class USoundCue* OnEquipSound;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
USoundCue* SwingSound;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Particles")
bool bWeaponParticles;

UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Skeletal Mesh")
class USkeletalMeshComponent* SkeletalMesh;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
class UBoxComponent* CombatCollision;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
float Damage;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
TSubclassOf<UDamageType> DamageTypeClass;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
AController* WeaponInstigator;

UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
class AMain* WeaponOwner;


protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Equip(class AMain* Char);

	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

	UFUNCTION()
		virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

	UFUNCTION(BlueprintCallable)
		void Sheathe();

	UFUNCTION(BlueprintCallable)
		void UnSheathe();
};
