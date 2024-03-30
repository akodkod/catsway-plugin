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

void USimpleInventoryComponent::AddItem(USimpleInventoryItem* Item) {
  AddItemQuantity(Item, 1);
}

void USimpleInventoryComponent::AddItemQuantity(USimpleInventoryItem* Item, int32 Quantity) {
  if (Item == nullptr || Quantity <= 0) return;

  for (FInventoryItem& InventoryItem: Items) {
    if (InventoryItem.Item == Item) {
      InventoryItem.Quantity += Quantity;
      return;
    }
  }

  FInventoryItem NewInventoryItem;
  NewInventoryItem.Item = Item;
  NewInventoryItem.Quantity = Quantity;
  Items.Add(NewInventoryItem);
}

void USimpleInventoryComponent::RemoveItem(USimpleInventoryItem* Item) {
  RemoveItemQuantity(Item, 1);
}

void USimpleInventoryComponent::RemoveItemQuantity(USimpleInventoryItem* Item, int32 Quantity) {
  if (Item == nullptr || Quantity <= 0) return;

  for (int32 i = 0; i < Items.Num(); ++i) {
    FInventoryItem& InventoryItem = Items[i];
    
    if (InventoryItem.Item == Item) {
      InventoryItem.Quantity -= Quantity;
      
      if (InventoryItem.Quantity <= 0) {
        Items.RemoveAt(i);
      }
      
      return;
    }
  }
}

bool USimpleInventoryComponent::HasItem(USimpleInventoryItem* Item) const {
  for (const FInventoryItem& InventoryItem: Items) {
    if (InventoryItem.Item == Item) {
      return true;
    }
  }

  return false;
}

int32 USimpleInventoryComponent::GetItemQuantity(USimpleInventoryItem* Item) const {
  for (const FInventoryItem& InventoryItem: Items) {
    if (InventoryItem.Item == Item) {
      return InventoryItem.Quantity;
    }
  }

  return 0;
}

void USimpleInventoryComponent::SetItemQuantity(USimpleInventoryItem* Item, int32 Quantity) {
  if (Item == nullptr || Quantity < 0) return;

  for (FInventoryItem& InventoryItem: Items) {
    if (InventoryItem.Item == Item) {
      InventoryItem.Quantity = Quantity;
      return;
    }
  }

  FInventoryItem NewInventoryItem;
  NewInventoryItem.Item = Item;
  NewInventoryItem.Quantity = Quantity;
  Items.Add(NewInventoryItem);
}
