// Fill out your copyright notice in the Description page of Project Settings.


#include "TagList.h"


namespace GameTags
{
    // Cria a tag de fato: Nome da variável, Tag no Editor, Comentário
    UE_DEFINE_GAMEPLAY_TAG(Damage_Physical_Blunt, "Damage.Physical.Blunt");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Physical_Slashing, "Damage.Physical.Slashing");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Physical_Piercing, "Damage.Physical.Piercing");

    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Light, "Damage.Magical.Light");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Dark, "Damage.Magical.Dark");

    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Fire, "Damage.Elemental.Fire");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Ice, "Damage.Elemental.Ice");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Water, "Damage.Elemental.Water");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Wind, "Damage.Elemental.Wind");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Earth, "Damage.Elemental.Earth");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Magical_Eletric, "Damage.Elemental.Eletric");

    UE_DEFINE_GAMEPLAY_TAG(Damage_Poison, "Damage.Poison");
    UE_DEFINE_GAMEPLAY_TAG(Damage_Direct, "Damage.Direct");

    UE_DEFINE_GAMEPLAY_TAG(Item, "Item");
    UE_DEFINE_GAMEPLAY_TAG(Item_Consumable, "Item.Consumable");
    UE_DEFINE_GAMEPLAY_TAG(Item_Weapon, "Item.Weapon");
    UE_DEFINE_GAMEPLAY_TAG(Item_Equipment, "Item.Equipment");
    UE_DEFINE_GAMEPLAY_TAG(Item_Material, "Item.Material");
    UE_DEFINE_GAMEPLAY_TAG(Item_Key, "Item.Key");
    UE_DEFINE_GAMEPLAY_TAG(Item_Quest, "Item.Quest");
    UE_DEFINE_GAMEPLAY_TAG(Item_Miscellaneous, "Item.Miscellaneous");

}