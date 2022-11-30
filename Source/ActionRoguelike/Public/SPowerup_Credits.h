// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASPowerupActor.h"
#include "SInteractionComponent.h"
#include "SPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Credits : public AASPowerupActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly,Category="Credits")
	int32 CreditsAmount =80;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	ASPowerup_Credits();
};
