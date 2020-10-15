// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

UCLASS()
class RPG_API ATool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATool();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tool")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tool")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool | Sounds")
		class USoundCue* PrimaryFunctionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool | Sounds")
		USoundCue* SecondaryFunctionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool | Particles")
		class UParticleSystemComponent* IdleParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool | Particles")
		class UParticleSystem* OverlapParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool")
		AController* ToolInstigator;

	bool bUsesAmmo;

	class AMain* ToolOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void PrimaryFunction();

	UFUNCTION()
		virtual void SecondaryFunction();

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		virtual void Equip(class AMain* Char);

	UFUNCTION(BlueprintCallable)
		virtual void Sheathe();

	UFUNCTION(BlueprintCallable)
		virtual void UnSheathe();

	FORCEINLINE virtual void SetInstigator(AController* Inst) { ToolInstigator = Inst; } 

};
