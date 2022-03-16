// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"


class UProjectileMovementComponent;

UCLASS()
class RETURNOFTHEGORM_API AProjectileBase : public AActor
{
	GENERATED_BODY()

	private:
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 1300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleTrail = nullptr;


	// VARIABLES
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 50.0f;
	// Arka kısımda ki dumanı için
	UPROPERTY(EditAnywhere, Category= "Effect")
	UParticleSystem* HitParticle = nullptr;
	// Ses için
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundBase* LaunchSound;

	// FUNCTION
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	public:
	// Sets default values for this actor's properties
	AProjectileBase();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};