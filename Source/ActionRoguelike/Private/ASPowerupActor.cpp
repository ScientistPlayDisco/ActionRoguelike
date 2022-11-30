// Fill out your copyright notice in the Description page of Project Settings.


#include "ASPowerupActor.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AASPowerupActor::AASPowerupActor()
{
 	SphereComp =CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Meshcomp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
	RespawnTime =10.f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(IsActive);

	RootComponent->SetVisibility(IsActive,true);
}

void AASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}
void AASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}
void AASPowerupActor::HideAndCooldownPowerup()
{
	 // if (GetOwner()->HasAuthority())
	 // {
	 // 	ServerHideAndCooldownPowerup_Implementation();
	 // }
	SetPowerupState(false);

	FTimerHandle TimerHandle_RespawnTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AASPowerupActor::ShowPowerup, RespawnTime);
}

void AASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	IsActive =bNewIsActive;
	OnRep_IsActive();
}

// Called every frame
void AASPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AASPowerupActor::GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AASPowerupActor,IsActive);
}
