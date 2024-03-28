// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleInteractionCharacterComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CATSWAY_API USimpleInteractionCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleInteractionCharacterComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	float InteractionDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	float NearbyDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	FVector TraceCenterOffset = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	bool bDebug = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	AActor* FocusedActor = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> NearbyActors;

	void FocusActor(AActor* Actor);
	void UnfocusActor(AActor* Actor);

	void MarkActorAsNearby(AActor* Actor);
	void UnmarkActorAsNearby(AActor* Actor);
	void UnmarkActorsAsNearbyThatAreNotFromList(TArray<AActor*> Actors);

	bool IsActorVisibleToCharacter(AActor* Actor);
	bool IsActorVisibleToCamera(AActor* Actor);

	bool IsActorTraceable(AActor* Actor, FVector Start, AActor* IgnoreActor);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Simple Interaction")
	void TraceAndFocusOnInteractableActor();
	
	UFUNCTION(BlueprintCallable, Category = "Simple Interaction")
	void InteractWithFocusedActor();

	UFUNCTION(BlueprintCallable, Category = "Simple Interaction")
	void UnfocusCurrentActor();

	UFUNCTION(BlueprintCallable, Category = "Simple Interaction")
	void UnmarkAllNearbyActors();
};
