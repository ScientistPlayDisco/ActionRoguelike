// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SProjectile_BlackHole.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASProjectile_BlackHole : public ASProjectileBase
{
	GENERATED_BODY()

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit) override;
};
