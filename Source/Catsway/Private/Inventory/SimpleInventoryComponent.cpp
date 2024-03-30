// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/SimpleInventoryComponent.h"

// Sets default values for this component's properties
USimpleInventoryComponent::USimpleInventoryComponent() {
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = false;

  // ...
}

// Called when the game starts
void USimpleInventoryComponent::BeginPlay() {
  Super::BeginPlay();

  // ...
  
}

// Called every frame
void USimpleInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void USimpleInventoryComponent::AddItem(FName RowName) {
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Adding item with system name %s"), *RowName.ToString()));
  
  if (!ItemExistsInDataTable(RowName)) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item with system name %s does not exist in data table"), *RowName.ToString()));
    return;
  }

  FInventoryItem Item;
  Item.RowName = RowName;
  Items.Add(Item);
}

void USimpleInventoryComponent::RemoveItem(FName RowName) {
  for (int i = 0; i < Items.Num(); i++) {
    if (Items[i].RowName == RowName) {
      Items.RemoveAt(i);
      return;
    }
  }
}

bool USimpleInventoryComponent::HasItem(FName RowName) {
  for (int i = 0; i < Items.Num(); i++) {
    if (Items[i].RowName == RowName) {
      return true;
    }
  }
  
  return false;
}

bool USimpleInventoryComponent::ItemExistsInDataTable(FName RowName) {
  return ItemsDataTable->FindRow<FInventoryItemDataTable>(RowName, TEXT("")) != nullptr;
}


