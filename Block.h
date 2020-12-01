// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UENUM(BlueprintType)
enum class EPushStatus : uint8
{
	EMS_Idle UMETA(Display = "Idle"),
	EMS_NorthOn UMETA(Display = "NorthOn"),
	EMS_SouthOn UMETA(Display = "SouthOn"),
	EMS_EastOn UMETA(Display = "EastOn"),
	EMS_WestOn UMETA(Display = "WestOn"),

	EMS_MAX UMETA(Display = "Default MAX")
};

UCLASS()
class RPG_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* NorthBoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		 UBoxComponent* SouthBoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		UBoxComponent* EastBoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		UBoxComponent* WestBoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	class UStaticMeshComponent* Mesh;

	bool bIsBeingPushed;

	FVector InitialLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EPushStatus PushStatus;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void NorthBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void NorthBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void SouthBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void SouthBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void EastOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void EastOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void WestOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void WestOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE void SetPushStatus(EPushStatus Status) { PushStatus = Status; }

};
