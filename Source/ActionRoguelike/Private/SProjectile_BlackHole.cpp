// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile_BlackHole.h"

void ASProjectile_BlackHole::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit)
{
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpluse, Hit);

}
