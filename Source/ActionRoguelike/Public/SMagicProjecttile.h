// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SMagicProjecttile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	
	ASMagicProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DamageAmount;
	
	UFUNCTION() 
	void OnActorOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
						UPrimitiveComponent* PrimitiveComponent1,
						int I, bool bArg, const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TSubclassOf<USActionEffect> BurningActionClass;
	

};
