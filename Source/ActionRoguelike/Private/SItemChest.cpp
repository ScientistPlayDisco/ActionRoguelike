// Fill out your copyright notice in the Description page of Project Settings.


#include "SITemChest.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BashMesh");
	RootComponent = BaseMesh;
	BaseMesh->SetSimulatePhysics(true);
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh); 

	TargetPitch =110;

	//变量可被复制
	SetReplicates(true);

}
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	bLidOpened = !bLidOpened;

	OnRep_LidOpened();
}



void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch  = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch,0,0));
}

void ASItemChest::OnActorLoaded_Implementation()
{
	// ISGameplayInterface::OnActorLoaded_Implementation();
	OnRep_LidOpened();
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest,bLidOpened);	
}