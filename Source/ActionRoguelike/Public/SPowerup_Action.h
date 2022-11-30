// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASPowerupActor.h"
#include "SAction.h"
#include "SInteractionComponent.h"
#include "SPowerup_Action.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Action : public AASPowerupActor
{
	GENERATED_BODY()

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	UPROPERTY(EditAnywhere,Category="Powerup")
	TSubclassOf<USAction> ActionToGrant;
};
