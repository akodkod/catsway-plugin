// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleInteraction.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SimpleInteractionActor.generated.h"

UCLASS()
class CATSWAY_API ASimpleInteractionActor : public AActor, public ISimpleInteraction
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleInteractionActor();

	// Trigger preview box to detect player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simple Interaction")
	UBoxComponent* NearbyAreaBox;

	// Debug
	UPROPERTY(EditAnywhere, Category = "Simple Interaction")
	bool bDebug = false;

protected:
	bool bPlayerIsNear = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerEnterNearbyAreaBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerLeaveNearbyAreaBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Add default function for ISimplyInteractable::CanPlayerInteract
	virtual bool CanPlayerInteract_Implementation() override
	{
		return true;
	}
};
