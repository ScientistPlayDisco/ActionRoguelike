// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectFraction = 0.2f;
	Duration = 0.f;
	Period = 0.f;
}


void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this,&USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this,&USActionEffect_Thorns::OnHealthChanged);
	}
}



void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delata)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if(Delata <0.f && OwningActor != InstigatorActor)
	{
		int32 ReflectAmount = FMath::RoundToInt(Delata*ReflectFraction);
		if(ReflectFraction ==0)
		{
			return;
		}
		ReflectAmount =FMath::Abs(ReflectAmount);
		USGameplayFunctionLibrary::ApplyDamage(OwningActor,InstigatorActor,ReflectAmount);
	}
}