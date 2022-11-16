// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval =2.f;

}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);

	//Make sure we have assigned at least one power-up class
	if(ensure(PowerupClasses.Num()>0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,PowerupSpawnQuery,this,EEnvQueryRunMode::AllMatching,nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld());It;++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));

		if(ensure(AttributeComp)&&AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);//pass in PLayer for kill credit.
		}
	}
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
		MaxBotsCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if(NrofAliveBots >= MaxBotsCount)
	{
		UE_LOG(LogTemp,Log,TEXT("At maximum bot capacity. Skipping bot spawn."));
	}
	TArray<FVector> Locations = QueryInstance ->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}	
	UEnvQueryInstanceBlueprintWrapper* QuerryInstance =   UEnvQueryManager::RunEQSQuery(this,SpawnBotQuerry,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);

	QuerryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQuerryCompleted);
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn Powerup is Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;

	while (SpawnCounter < DesiredPowerupCount && Locations.Num()>0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0,Locations.Num()-1);

		FVector PickedLocation =Locations[RandomLocationIndex];

		Locations.RemoveAt(RandomLocationIndex);
		bool bValidLocation =true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo <RequirePowerupDistance)
			{

				bValidLocation = true;
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;
		}
		int32 RandomClassIndex = FMath::RandRange(0,PowerupClasses.Num() -1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		PickedLocation.Z += 30.f;
		GetWorld()->SpawnActor<AActor>(RandomPowerupClass,PickedLocation,FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}

}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}


void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	 ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if(Player) 
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());
		float RespawnDelay =2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);

		UE_LOG(LogTemp,Log,TEXT("OnActorKilled: Victim: %s,Killer:%s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
	}


	APawn* KillerPawn = Cast<APawn>(Killer);
	 if (KillerPawn)
	 {
		 ASPlayerState* PlayState = KillerPawn->GetPlayerState<ASPlayerState>();
		 if (PlayState)
		 {
			 int32 CreditsPerKill =50;
		 	PlayState->AddCredits(CreditsPerKill);
		 }
	 }
}
