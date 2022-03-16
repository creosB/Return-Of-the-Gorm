// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "FireBridge.generated.h"

class UBoxComponent;

UCLASS()
class RETURNOFTHEGORM_API AFireBridge : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireBridge();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BridgeMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ExplosiveParticle = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* ExplosiveSound = nullptr;
	FTimerHandle ExplosiveTimer;
	FVector BridgeScale;

	void Explosive();
	// Explosive Settings
	UPROPERTY(EditAnywhere, Category= "Bridge Settings")
	float ExplosiveSpeed = 50.0f;
	UPROPERTY(EditAnywhere, Category= "Bridge Settings")
	float ParticleLocation = 1000.0f;
	UPROPERTY(EditAnywhere, Category= "Bridge Settings")
	float UpdateParticle = 500.0f;
	bool ExplosiveLogic = false;
	bool FirstExplosive = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
					AActor* OtherActor,
					UPrimitiveComponent* OtherComp,
					int32 OtherBodyIndex,
					bool bFromSweep,
					const FHitResult &SweepResult);

};
