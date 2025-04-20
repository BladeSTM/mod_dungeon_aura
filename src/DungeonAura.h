#ifndef _DUNGEON_AURA_H_
#define _DUNGEON_AURA_H_

#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "SpellAuras.h"
#include <vector>

// Dungeon rule spells
enum DungeonRuleSpells
{
    SPELL_DUNGEON_MASTERY = 800001,
    SPELL_MAGICAL_ARSENAL = 800002,
    SPELL_POWERFUL_STRIKE = 800003,
    SPELL_HEROIC_ENDURANCE = 800004,
    SPELL_IMPROVED_VITALITY = 800005,
    SPELL_RAPID_MOVEMENT = 800006
};

/**
 * Structure to define dungeon aura effects
 */
struct DungeonAuraEffect 
{
    uint32 spellId;      // ID of the spell
    float bonusValue;    // Value of the bonus (0.10 = 10%)
    bool bossOnly;       // Whether the effect applies only to bosses
    bool isSpellDamage;  // Whether this affects spell damage (true) or melee damage (false)
};

class DungeonAura : public UnitScript
{
public:
    DungeonAura();

    /**
     * Modifies the melee damage based on active spell effects
     */
    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override;

    /**
     * Modifies the spell damage based on active spell effects
     */
    void ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage, SpellInfo const* spellInfo) override;

private:
    /**
     * List of dungeon aura effects that affect damage
     */
    std::vector<DungeonAuraEffect> damageEffects;

    /**
     * Initializes the effect lists
     */
    void InitializeEffects();
};

// Function to register all scripts
void AddDungeonAuraScripts();

#endif // _DUNGEON_AURA_H_