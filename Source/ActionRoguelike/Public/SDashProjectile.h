// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly,Category ="Teleport")
	//传送
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly,Category ="Teleport")
	//引爆
	float DetonateDelay;
	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:
	ASDashProjectile();
};
