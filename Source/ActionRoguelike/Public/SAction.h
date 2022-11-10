// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
protected:

	UFUNCTION(BlueprintCallable,Category="Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(BlueprintReadOnly,Category="Action")
	bool bIsRunning;
public:
	UPROPERTY(EditDefaultsOnly,Category="Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	bool IsRunning() const;
	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	void StopAction(AActor* Instigator);
	
	UPROPERTY(EditDefaultsOnly,Category="Action")
	FName ActionName;//Fname通常用于正面使用，它是Hash的

	UWorld* GetWorld() const override;
};
