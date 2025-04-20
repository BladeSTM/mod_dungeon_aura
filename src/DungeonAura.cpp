#include "DungeonAura.h"
#include "ScriptMgr.h"
#include "Creature.h"

/**
 * Constructor for the DungeonAura script
 */
DungeonAura::DungeonAura() : UnitScript("DungeonAura") 
{
    InitializeEffects();
}

/**
 * Initializes the list of dungeon aura effects
 */
void DungeonAura::InitializeEffects()
{
    // Add all damage-affecting auras to the list
    damageEffects = {
        { SPELL_DUNGEON_MASTERY, 0.10f, true, false },   // 10% bonus damage to bosses (melee)
        { SPELL_DUNGEON_MASTERY, 0.10f, true, true },    // 10% bonus damage to bosses (spell)
        { SPELL_POWERFUL_STRIKE, 0.15f, false, false },  // 15% bonus physical damage (melee)
        { SPELL_MAGICAL_ARSENAL, 0.15f, false, true }    // 15% bonus magical damage (spell)
    };
}

/**
 * Modifies melee damage based on active spell effects
 */
void DungeonAura::ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage)
{
    if (!target || !attacker)
        return;

    // Only apply if attacker is a player
    if (attacker->GetTypeId() != TYPEID_PLAYER)
        return;

    // Only apply in dungeons
    if (!attacker->GetMap() || !attacker->GetMap()->IsDungeon())
        return;

    Player* player = attacker->ToPlayer();
    if (!player)
        return;

    float totalBonus = 0.0f;

    // Check if target is a boss
    bool isBoss = false;
    if (target->GetTypeId() == TYPEID_UNIT)
    {
        Creature* creature = target->ToCreature();
        if (creature && (creature->isWorldBoss() || creature->IsDungeonBoss()))
        {
            isBoss = true;
        }
    }

    // Process each effect that affects melee damage
    for (const auto& effect : damageEffects)
    {
        // Skip spell damage effects
        if (effect.isSpellDamage)
            continue;

        // Skip boss-only effects if target is not a boss
        if (effect.bossOnly && !isBoss)
            continue;

        // Check if player has the aura
        if (player->HasAura(effect.spellId))
        {
            totalBonus += effect.bonusValue;
        }
    }

    // Apply the total bonus
    if (totalBonus > 0.0f)
    {
        damage = uint32(damage * (1.0f + totalBonus));
    }
}

/**
 * Modifies spell damage based on active spell effects
 */
void DungeonAura::ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage, SpellInfo const* spellInfo)
{
    if (!target || !attacker || !spellInfo)
        return;

    // Only apply if attacker is a player
    if (attacker->GetTypeId() != TYPEID_PLAYER)
        return;

    // Only apply in dungeons
    if (!attacker->GetMap() || !attacker->GetMap()->IsDungeon())
        return;

    Player* player = attacker->ToPlayer();
    if (!player)
        return;

    float totalBonus = 0.0f;

    // Check if target is a boss
    bool isBoss = false;
    if (target->GetTypeId() == TYPEID_UNIT)
    {
        Creature* creature = target->ToCreature();
        if (creature && (creature->isWorldBoss() || creature->IsDungeonBoss()))
        {
            isBoss = true;
        }
    }

    // Process each effect that affects spell damage
    for (const auto& effect : damageEffects)
    {
        // Skip melee damage effects
        if (!effect.isSpellDamage)
            continue;

        // Skip boss-only effects if target is not a boss
        if (effect.bossOnly && !isBoss)
            continue;

        // Check if player has the aura
        if (player->HasAura(effect.spellId))
        {
            totalBonus += effect.bonusValue;
        }
    }

    // Apply the total bonus
    if (totalBonus > 0.0f)
    {
        damage = int32(damage * (1.0f + totalBonus));
    }
}

/**
 * Registers all scripts for the DungeonAura module
 */
void AddDungeonAuraScripts()
{
    new DungeonAura();
}