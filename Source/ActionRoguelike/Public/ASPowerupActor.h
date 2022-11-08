// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "SInteractionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ASPowerupActor.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AASPowerupActor : public AActor,public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="PowerUp")
	float RespawnTime;

	UFUNCTION()
	void ShowPowerup();
	void Interact_Implementation(APawn* InstigatorPawn);

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere,Category="Components")
	USphereComponent* SphereComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
