// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * 
 */

namespace GameTags
{
    // Avisa o projeto que essas tags existem
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical_Blunt);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical_Slashing);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical_Piercing);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Magical_Light);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Magical_Dark);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Fire);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Ice);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Water);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Wind);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Earth);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Eletric);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Poison);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Direct);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Consumable);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Weapon);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Material);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Key);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Quest);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Miscellaneous);

}
