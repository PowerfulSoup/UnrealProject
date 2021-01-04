// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemChest.generated.h"

UCLASS()
class RPG_API AItemChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemChest();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Chest")
		class UStaticMeshComponent* ChestBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Chest")
		UStaticMeshComponent* ChestLid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Chest")
		class UBoxComponent* OpenVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TSubclassOf<class AItem> ChestContents;

	bool bIsOpened;
	FVector InitialLidLocation;
	FRotator InitialLidRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OpenItemChest(AMain* Char); //change to return an ITem instead of void?

	UFUNCTION(BlueprintCallable)
		void UpdateLidLocation(float Value);

	UFUNCTION(BlueprintCallable)
		void UpdateLidRotation(float Value);

	UFUNCTION(BlueprintImplementableEvent)
		void OpenItemChestStart();

	//UFUNCTION()
	//	void InitializeContents(int32 Value, AItem* Item);

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void SpawnChestContents();
};
