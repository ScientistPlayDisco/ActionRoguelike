// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval =2.f;

}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);
}


void ASGameModeBase::OnQuerryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus !=EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	int32 NrofAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld());It;++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));

		if(ensure(AttributeComp)&&AttributeComp->IsAlive())
		{
			NrofAliveBots++;
		}
	}

	float MaxBotsCount = 10.f;

	if(NrofAliveBots >= MaxBotsCount)
	{
		return;
	}

	if(DifficultyCurve)
	{
		DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	TArray<FVector> Locations = QueryInstance ->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QuerryInstance =   UEnvQueryManager::RunEQSQuery(this,SpawnBotQuerry,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);

	QuerryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQuerryCompleted);
}
