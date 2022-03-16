// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class RETURNOFTHEGORM_API AGrenade : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	           FVector NormalImpulse, const FHitResult& Hit);
	virtual void Tick(float DeltaSeconds) override;

private:
	void FindPointer();
	UPROPERTY(EditAnywhere, Category= "Components")
	UStaticMeshComponent* GrenadeMesh;
	// Effect System
	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	UParticleSystem* GrenadeParticle = nullptr;
	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	USoundBase* GrenadeSound = nullptr;
	// Grenade decreasing the health
	UPROPERTY(EditAnywhere, Category= "Components")
	float DamageValue = 10.0f;
	float SpawnedTime;
	UPROPERTY(EditAnywhere, Category= "Components")
	float ExplosiveTime = 0.75f;
};
