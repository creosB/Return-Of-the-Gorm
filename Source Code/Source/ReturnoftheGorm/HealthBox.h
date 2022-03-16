// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthBox.generated.h"

UCLASS()
class RETURNOFTHEGORM_API AHealthBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	           FVector NormalImpulse, const FHitResult& Hit);

private:
	void FindPointer();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components",meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* HealthBoxMesh;
	// Effect System
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Effects", meta = (AllowPrivateAccess = true))
	UParticleSystem* GiveHealthParticle = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Effects", meta = (AllowPrivateAccess = true))
	USoundBase* GiveHealthSound = nullptr;
	// Healthbox giving this amounth health percent.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Components",meta = (AllowPrivateAccess = true))
	float GiveHealthValue = 10.0f;

};
