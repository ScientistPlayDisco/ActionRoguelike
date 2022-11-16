// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.h"
#include "SAction.generated.h"

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
public:

	UPROPERTY()
	bool IsRunning;

	UPROPERTY()
	AActor* Instigator;
};



/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated)
	USActionComponent* ActionComp;
	
	UFUNCTION(BlueprintCallable,Category="Actions")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	UFUNCTION()
	void OnRep_RepData();
public:

	void Initialize(USActionComponent* NewActionComp);
	
	UPROPERTY(EditDefaultsOnly,Category="Actions")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent,Category = "Actions")
	bool IsRunning() const;
	UFUNCTION(BlueprintNativeEvent,Category = "Actions")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "Actions")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "Actions")
	void StopAction(AActor* Instigator);
	
	UPROPERTY(EditDefaultsOnly,Category="Actions")
	FName ActionName;//Fname通常用于正面使用，它是Hash的

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return  true;
	}
};
