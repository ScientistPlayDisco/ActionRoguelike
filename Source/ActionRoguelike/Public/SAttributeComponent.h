// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"
class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged,AActor*,InstigatorActor,USAttributeComponent*,OwningComp,float,NewAttribute,float,Delata);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged,AActor*,InstigatorActor,USAttributeComponent*,OwningComp,float,NewRage,float,Delata);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable,Category="Attributes",meta= (DisplayName= "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float Health;

	UPROPERTY(BlueprintReadOnly,Replicated,Category="Attributes")
	float HealthMax;
	
	UPROPERTY(EditDefaultsOnly,Replicated,BlueprintReadOnly,Category="Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly,Replicated,BlueprintReadOnly,Category="Attributes")
	float RageMax;
	
	// UPROPERTY(ReplicatedUsing="")
	// bool bIsAlive;
	
	UFUNCTION(NetMulticast,Reliable)//mark as unreliable once we removed the 'state' our of scharacter
	void MulticastHealthChanged(AActor* InstigatorActor,float NewHealth,float Delta);

	UFUNCTION(NetMulticast,Unreliable)//mark as unreliable once we removed the 'state' our of scharacter
	void MulticastRageChanged(AActor* InstigatorActor,float NewRage,float Delta);
public:
	UFUNCTION()
	bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable)
	bool  IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	bool  IsLowHealth();
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(AActor* InstigateActor,float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsFullRage() const;
	
	UFUNCTION(BlueprintCallable)
	bool ApplyRageChange(AActor* InstigateActor,float Delta);

	
	// UFUNCTION(BlueprintCallable)
	// static  USAttributeComponent* GetAttributes(APawn* Pawn);
		
};
