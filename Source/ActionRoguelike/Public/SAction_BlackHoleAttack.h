// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_BlackHoleAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_BlackHoleAttack : public USAction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Effects")
	FName HandSocketName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attack")
	float AttackAnimDelay;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION(BlueprintCallable)
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
public:

	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	USAction_BlackHoleAttack();
};
