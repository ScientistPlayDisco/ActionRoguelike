// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditAnywhere,Category ="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditAnywhere,Category ="AI")
	UEnvQuery* SpawnBotQuerry;

	UPROPERTY(EditAnywhere,Category ="AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditAnywhere,Category ="AI")
	UCurveFloat* DifficultyCurve;
	
	UFUNCTION()
	void OnQuerryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
public:
	ASGameModeBase();

	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);
	
	virtual void StartPlay() override;
	UFUNCTION(Exec)
	void KillAll();
};
