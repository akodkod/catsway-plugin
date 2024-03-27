// Fill out your copyright notice in the Description page of Project Settings.


#include "SI/SimpleInteractionActor.h"


// Sets default values
ASimpleInteractionActor::ASimpleInteractionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	// Create a trigger preview box
	NearbyAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("NearbyAreaBox"));
	NearbyAreaBox->SetupAttachment(RootComponent);
	NearbyAreaBox->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	NearbyAreaBox->SetBoxExtent(FVector(250.f, 250.f, 50.f));
	NearbyAreaBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	NearbyAreaBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ASimpleInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	NearbyAreaBox->OnComponentBeginOverlap.AddDynamic(this, &ASimpleInteractionActor::OnPlayerEnterNearbyAreaBox);
	NearbyAreaBox->OnComponentEndOverlap.AddDynamic(this, &ASimpleInteractionActor::OnPlayerLeaveNearbyAreaBox);
}

// Called every frame
void ASimpleInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimpleInteractionActor::OnPlayerEnterNearbyAreaBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Execute_CanPlayerInteract(this)) return;

	bPlayerIsNear = true;
	Execute_PlayerEntersNearbyArea(this);	
}

void ASimpleInteractionActor::OnPlayerLeaveNearbyAreaBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bPlayerIsNear) return;

	bPlayerIsNear = false;
	Execute_PlayerLeavesNearbyArea(this);
}
