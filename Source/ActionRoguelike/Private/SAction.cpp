// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
	
}

void USAction::SetCoolDown(float Time)
{
	bIsCoolDown = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CoolDownTimer,this,&USAction::CoolDown,Time);
}

void USAction::ImplementActAfterStartAction_Implementation()
{
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{

	if(IsRunning())
	{
		return false;
	}
	if (bIsCoolDown)
	{
		return false;
	}
	USActionComponent* Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this,FString::Printf(TEXT("Started: %s"),*ActionName.ToString()),FColor::Green);
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	RepData.IsRunning = true;
	RepData.Instigator = Instigator;
	//
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted =GetWorld()->TimeSeconds;
	}
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(),this);

	ImplementActAfterStartAction();
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this,FString::Printf(TEXT("Stoped: %s"),*ActionName.ToString()),FColor::White);

	//ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	RepData.IsRunning = false;
	RepData.Instigator = Instigator;
	//
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(),this);

}

UWorld* USAction::GetWorld() const
{
	//the outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	// AActor* Actor = Cast<AActor>(GetOuter());
	// return Actor->GetComponentByClass(USActionComponent::StaticClass());
	// return Cast<USActionComponent>(GetOuter());
	return  ActionComp;
}

void USAction::OnRep_RepData()
{
	if (RepData.IsRunning) 
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void USAction::CoolDown()
{
	bIsCoolDown = false;
}


bool USAction::IsRunning_Implementation() const
{
	return RepData.IsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction,RepData);
	DOREPLIFETIME(USAction,TimeStarted);
	DOREPLIFETIME(USAction,ActionComp);

}