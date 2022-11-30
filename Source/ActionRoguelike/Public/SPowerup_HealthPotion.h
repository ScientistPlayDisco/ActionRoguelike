// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_HealthPotion : public AASPowerupActor
{
	GENERATED_BODY()
protected:

	// UPROPERTY(VisibleAnywhere, Category = "Components")
	// UStaticMeshComponent* MeshComp;

	// float healt amount?

	public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere,Category="Credits")
	int32 CreditCost;
	
	ASPowerup_HealthPotion();
	
};
