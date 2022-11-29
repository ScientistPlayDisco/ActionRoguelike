// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval =2.f;

	SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
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

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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

	TArray<FVector> Locations = QueryInstance ->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("",Rows);

			//
			int32 RandomIndex = FMath::RandRange(0,Rows.Num() -1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			GetWorld()->SpawnActor<AActor>(SelectedRow->MonsterClass,Locations[0],FRotator::ZeroRotator);

		}
	}
}

// int32 NrofAliveBots = 0;
// for (TActorIterator<ASAICharacter> It(GetWorld());It;++It)
// {
// 	ASAICharacter* Bot = *It;
//
// 	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
//
// 	if(ensure(AttributeComp)&&AttributeComp->IsAlive())
// 	{
// 		NrofAliveBots++;
// 	}
// }
//
// float MaxBotsCount = 10.f;
//
// if(NrofAliveBots >= MaxBotsCount)
// {
// 	return;
// }
//
// if(DifficultyCurve)
// {
// 	MaxBotsCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
// }
// if(NrofAliveBots >= MaxBotsCount)
// {
// 	UE_LOG(LogTemp,Log,TEXT("At maximum bot capacity. Skipping bot spawn."));
// }
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


void ASGameModeBase::WriteSaveGame()
{
	for (int32 i =0;i<GameState->PlayerArray.Num();i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;//single player only at this point;
		}
	}
	CurrentSaveGame->SaveActors.Empty();
	for (FActorIterator It(GetWorld());It;++It )
	{
		AActor* Actor = *It;
		if (!Actor ->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);

		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		
		CurrentSaveGame->SaveActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame = Cast<USSaveGame>( UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if (CurrentSaveGame ==nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Faild to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp,Log,TEXT("Loaded SaveGame Data."));

		for (FActorIterator It(GetWorld());It;++It )
		{
			AActor* Actor = *It;
			if (!Actor ->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData: CurrentSaveGame->SaveActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader,true);

					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp,Log,TEXT("Created New SaveGame Data."));
	}
}

