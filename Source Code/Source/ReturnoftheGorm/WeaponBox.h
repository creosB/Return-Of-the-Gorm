// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBox.generated.h"

UCLASS()
class RETURNOFTHEGORM_API AWeaponBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBox();

private:
	// Function
	void FindPointer();

	// WeaponBox Settings
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* WeaponBoxMesh;
	// Effect System
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Effects", meta = (AllowPrivateAccess = true))
	UParticleSystem* GiveWeaponParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Effects", meta = (AllowPrivateAccess = true))
	USoundBase* GiveWeaponSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	           FVector NormalImpulse, const FHitResult& Hit);
};
