#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "LootChest.generated.h"

/**
 *
 */
UCLASS()
class RPG_API ALootChest : public AInteractable
{
	GENERATED_BODY()
public:

	ALootChest();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable | Chest")
		class UStaticMeshComponent* ChestLid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Interactable | Chest")
	TSubclassOf<class AItem> ChestContents;

	bool bIsOpened;
	FVector InitialLidLocation;
	FRotator InitialLidRotation;

protected:

	virtual void BeginPlay() override;

public:

		virtual void InteractFunction(AMain* Char) override;

		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

		UFUNCTION(BlueprintCallable)
			void UpdateLidLocation(float Value);

		UFUNCTION(BlueprintCallable)
			void UpdateLidRotation(float Value);

		UFUNCTION(BlueprintImplementableEvent)
			void OpenItemChestStart();

		UFUNCTION(BlueprintCallable)
			void SpawnChestContents();
};
