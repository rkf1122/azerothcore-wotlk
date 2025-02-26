/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it and/or modify it under version 2 of the License, or (at your option), any later version.
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

/*
 * Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warr_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellScript.h"

enum WarriorSpells
{
    // Ours
    SPELL_WARRIOR_INTERVENE_TRIGGER                 = 59667,
    SPELL_WARRIOR_SPELL_REFLECTION                  = 23920,
    SPELL_WARRIOR_IMPROVED_SPELL_REFLECTION_TRIGGER = 59725,

    // Theirs
    SPELL_WARRIOR_BLOODTHIRST                       = 23885,
    SPELL_WARRIOR_BLOODTHIRST_DAMAGE                = 23881,
    SPELL_WARRIOR_CHARGE                            = 34846,
    SPELL_WARRIOR_DAMAGE_SHIELD_DAMAGE              = 59653,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_1                = 12162,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_2                = 12850,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_3                = 12868,
    SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC         = 12721,
    SPELL_WARRIOR_EXECUTE                           = 20647,
    SPELL_WARRIOR_GLYPH_OF_EXECUTION                = 58367,
    SPELL_WARRIOR_GLYPH_OF_VIGILANCE                = 63326,
    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF        = 65156,
    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT      = 64976,
    SPELL_WARRIOR_LAST_STAND_TRIGGERED              = 12976,
    SPELL_WARRIOR_RETALIATION_DAMAGE                = 22858,
    SPELL_WARRIOR_SLAM                              = 50783,
    SPELL_WARRIOR_SUNDER_ARMOR                      = 58567,
    SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK     = 26654,
    SPELL_WARRIOR_TAUNT                             = 355,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1        = 46859,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2        = 46860,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1     = 64849,
    SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2     = 64850,
    SPELL_WARRIOR_VIGILANCE_PROC                    = 50725,
    SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT         = 59665
};

enum WarriorSpellIcons
{
    WARRIOR_ICON_ID_SUDDEN_DEATH                    = 1989
};

enum MiscSpells
{
    SPELL_PALADIN_BLESSING_OF_SANCTUARY             = 20911,
    SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY     = 25899,
    SPELL_PRIEST_RENEWED_HOPE                       = 63944,
    SPELL_GEN_DAMAGE_REDUCTION_AURA                 = 68066,
};

// Ours
class spell_warr_mocking_blow : public SpellScriptLoader
{
public:
    spell_warr_mocking_blow() : SpellScriptLoader("spell_warr_mocking_blow") { }

    class spell_warr_mocking_blow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_mocking_blow_SpellScript);

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                if (target->IsImmunedToSpellEffect(GetSpellInfo(), EFFECT_1))
                    SetHitDamage(0);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_warr_mocking_blow_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_mocking_blow_SpellScript();
    }
};

enum VictoryRushEnum
{
    SPELL_VICTORIOUS    = 32216
};

class spell_warr_victory_rush : public SpellScriptLoader
{
public:
    spell_warr_victory_rush() : SpellScriptLoader("spell_warr_victory_rush") { }

    class spell_warr_victory_rush_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_victory_rush_SpellScript);

        void VictoryRushHit()
        {
            if (Unit* player = GetCaster())
            {
                if (Unit* victim = GetHitUnit())
                {
                    if (victim->isDead())
                    {
                        player->CastSpell(player, SPELL_VICTORIOUS, true);
                    }
                }
            }
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_warr_victory_rush_SpellScript::VictoryRushHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_victory_rush_SpellScript();
    }
};

class spell_warr_intervene : public SpellScriptLoader
{
public:
    spell_warr_intervene() : SpellScriptLoader("spell_warr_intervene") { }

    class spell_warr_intervene_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_intervene_SpellScript);

        void HandleApplyAura(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                target->CastSpell((Unit*)nullptr, SPELL_WARRIOR_INTERVENE_TRIGGER, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_intervene_SpellScript::HandleApplyAura, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_intervene_SpellScript();
    }
};

class spell_warr_improved_spell_reflection : public SpellScriptLoader
{
public:
    spell_warr_improved_spell_reflection() : SpellScriptLoader("spell_warr_improved_spell_reflection") { }

    class spell_warr_improved_spell_reflection_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_improved_spell_reflection_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetDamageInfo()->GetSpellInfo() && eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_WARRIOR_SPELL_REFLECTION;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_MAX_TARGETS, aurEff->GetAmount());
            values.AddSpellMod(SPELLVALUE_RADIUS_MOD, 2000); // Base range = 100, final range = 20 value / 10000.0f = 0.2f
            eventInfo.GetActor()->CastCustomSpell(SPELL_WARRIOR_IMPROVED_SPELL_REFLECTION_TRIGGER, values, eventInfo.GetActor(), TRIGGERED_FULL_MASK, nullptr);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_improved_spell_reflection_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_improved_spell_reflection_AuraScript::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_improved_spell_reflection_AuraScript();
    }
};

class spell_warr_improved_spell_reflection_trigger : public SpellScriptLoader
{
public:
    spell_warr_improved_spell_reflection_trigger() : SpellScriptLoader("spell_warr_improved_spell_reflection_trigger") { }

    class spell_warr_improved_spell_reflection_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_improved_spell_reflection_trigger_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            GetCaster()->RemoveAurasDueToSpell(SPELL_WARRIOR_SPELL_REFLECTION);
            unitList.sort(Acore::ObjectDistanceOrderPred(GetCaster()));
            while (unitList.size() > GetSpellValue()->MaxAffectedTargets)
                unitList.pop_back();
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_improved_spell_reflection_trigger_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_improved_spell_reflection_trigger_SpellScript();
    }

    class spell_warr_improved_spell_reflection_trigger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_improved_spell_reflection_trigger_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes  /*mode*/)
        {
            if (!IsExpired())
            {
                // aura remove - remove auras from all party members
                std::list<Unit*> PartyMembers;
                GetUnitOwner()->GetPartyMembers(PartyMembers);
                for (std::list<Unit*>::iterator itr = PartyMembers.begin(); itr != PartyMembers.end(); ++itr)
                {
                    if ((*itr)->GetGUID() != GetOwner()->GetGUID())
                        if (Aura* aur = (*itr)->GetAura(59725, GetCasterGUID()))
                        {
                            aur->SetDuration(0);
                            aur->Remove();
                        }
                }
            }
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_warr_improved_spell_reflection_trigger_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_REFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_improved_spell_reflection_trigger_AuraScript();
    }
};

// Theirs
// 12975 - Last Stand
class spell_warr_last_stand : public SpellScriptLoader
{
public:
    spell_warr_last_stand() : SpellScriptLoader("spell_warr_last_stand") { }

    class spell_warr_last_stand_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_last_stand_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_WARRIOR_LAST_STAND_TRIGGERED });
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(GetEffectValue()));
            caster->CastCustomSpell(caster, SPELL_WARRIOR_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, nullptr, nullptr, true, nullptr);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_warr_last_stand_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_last_stand_SpellScript();
    }
};

// -12162 - Deep Wounds
class spell_warr_deep_wounds : public SpellScriptLoader
{
public:
    spell_warr_deep_wounds() : SpellScriptLoader("spell_warr_deep_wounds") { }

    class spell_warr_deep_wounds_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_deep_wounds_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_WARRIOR_DEEP_WOUNDS_RANK_1,
                    SPELL_WARRIOR_DEEP_WOUNDS_RANK_2,
                    SPELL_WARRIOR_DEEP_WOUNDS_RANK_3
                });
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 damage = std::max(GetEffectValue(), 0);
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
            {
                // include target dependant auras
                damage = target->MeleeDamageBonusTaken(caster, damage, BASE_ATTACK, GetSpellInfo());
                // apply percent damage mods
                ApplyPct(damage, 16.0f * GetSpellInfo()->GetRank() / 6.0f);
                target->CastDelayedSpellWithPeriodicAmount(caster, SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC, SPELL_AURA_PERIODIC_DAMAGE, damage, EFFECT_0);

                //caster->CastCustomSpell(target, SPELL_WARRIOR_DEEP_WOUNDS_RANK_PERIODIC, &damage, nullptr, nullptr, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_deep_wounds_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_deep_wounds_SpellScript();
    }
};

// -100 - Charge
class spell_warr_charge : public SpellScriptLoader
{
public:
    spell_warr_charge() : SpellScriptLoader("spell_warr_charge") { }

    class spell_warr_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_charge_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT,
                    SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF,
                    SPELL_WARRIOR_CHARGE
                });
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 chargeBasePoints0 = GetEffectValue();
            Unit* caster = GetCaster();
            caster->CastCustomSpell(caster, SPELL_WARRIOR_CHARGE, &chargeBasePoints0, nullptr, nullptr, true);

            // Juggernaut crit bonus
            if (caster->HasAura(SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT))
                caster->CastSpell(caster, SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_charge_SpellScript();
    }
};

// -1464 - Slam
class spell_warr_slam : public SpellScriptLoader
{
public:
    spell_warr_slam() : SpellScriptLoader("spell_warr_slam") { }

    class spell_warr_slam_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_slam_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_WARRIOR_SLAM });
        }

        void SendMiss(SpellMissInfo missInfo)
        {
            if (missInfo != SPELL_MISS_NONE)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        caster->SendSpellMiss(target, SPELL_WARRIOR_SLAM, missInfo);
                    }
                }
            }
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetHitUnit())
                GetCaster()->CastCustomSpell(SPELL_WARRIOR_SLAM, SPELLVALUE_BASE_POINT0, GetEffectValue(), GetHitUnit(), TRIGGERED_FULL_MASK);
        }

        void Register() override
        {
            BeforeHit += BeforeSpellHitFn(spell_warr_slam_SpellScript::SendMiss);
            OnEffectHitTarget += SpellEffectFn(spell_warr_slam_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_slam_SpellScript();
    }
};

// -58872 - Damage Shield
class spell_warr_damage_shield : public SpellScriptLoader
{
public:
    spell_warr_damage_shield() : SpellScriptLoader("spell_warr_damage_shield") { }

    class spell_warr_damage_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_damage_shield_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_WARRIOR_DAMAGE_SHIELD_DAMAGE });
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            // % of amount blocked
            int32 damage = CalculatePct(int32(GetTarget()->GetShieldBlockValue()), aurEff->GetAmount());
            GetTarget()->CastCustomSpell(SPELL_WARRIOR_DAMAGE_SHIELD_DAMAGE, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetProcTarget(), true, nullptr, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warr_damage_shield_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_damage_shield_AuraScript();
    }
};

// -5308 - Execute
class spell_warr_execute : public SpellScriptLoader
{
public:
    spell_warr_execute() : SpellScriptLoader("spell_warr_execute") { }

    class spell_warr_execute_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_execute_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_WARRIOR_EXECUTE, SPELL_WARRIOR_GLYPH_OF_EXECUTION });
        }

        void SendMiss(SpellMissInfo missInfo)
        {
            if (missInfo != SPELL_MISS_NONE)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        caster->SendSpellMiss(target, SPELL_WARRIOR_EXECUTE, missInfo);
                    }
                }
            }
        }

        void HandleEffect(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
            {
                SpellInfo const* spellInfo = GetSpellInfo();
                int32 rageUsed = std::min<int32>(300 - spellInfo->CalcPowerCost(caster, SpellSchoolMask(spellInfo->SchoolMask)), caster->GetPower(POWER_RAGE));
                int32 newRage = std::max<int32>(0, caster->GetPower(POWER_RAGE) - rageUsed);

                // Sudden Death rage save
                if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_GENERIC, WARRIOR_ICON_ID_SUDDEN_DEATH, EFFECT_0))
                {
                    int32 ragesave = aurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue() * 10;
                    newRage = std::max(newRage, ragesave);
                }

                caster->SetPower(POWER_RAGE, uint32(newRage));
                // Glyph of Execution bonus
                if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_WARRIOR_GLYPH_OF_EXECUTION, EFFECT_0))
                    rageUsed += aurEff->GetAmount() * 10;

                int32 bp = GetEffectValue() + int32(rageUsed * spellInfo->Effects[effIndex].DamageMultiplier + caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.2f);
                caster->CastCustomSpell(target, SPELL_WARRIOR_EXECUTE, &bp, nullptr, nullptr, true, nullptr, nullptr, GetOriginalCaster()->GetGUID());
            }
        }

        void Register() override
        {
            BeforeHit += BeforeSpellHitFn(spell_warr_execute_SpellScript::SendMiss);
            OnEffectHitTarget += SpellEffectFn(spell_warr_execute_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_execute_SpellScript();
    }
};

// 12809 - Concussion Blow
class spell_warr_concussion_blow : public SpellScriptLoader
{
public:
    spell_warr_concussion_blow() : SpellScriptLoader("spell_warr_concussion_blow") { }

    class spell_warr_concussion_blow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_concussion_blow_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            SetHitDamage(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetEffectValue()));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_concussion_blow_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_concussion_blow_SpellScript();
    }
};

// 23881 - Bloodthirst
class spell_warr_bloodthirst : public SpellScriptLoader
{
public:
    spell_warr_bloodthirst() : SpellScriptLoader("spell_warr_bloodthirst") { }

    class spell_warr_bloodthirst_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_bloodthirst_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetEffectValue();
            ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

            if (Unit* target = GetHitUnit())
            {
                damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
                damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
            }
            SetHitDamage(damage);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetEffectValue();
            GetCaster()->CastCustomSpell(GetCaster(), SPELL_WARRIOR_BLOODTHIRST, &damage, nullptr, nullptr, true, nullptr);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_bloodthirst_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHit += SpellEffectFn(spell_warr_bloodthirst_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_bloodthirst_SpellScript();
    }
};

// 23880 - Bloodthirst (Heal)
class spell_warr_bloodthirst_heal : public SpellScriptLoader
{
public:
    spell_warr_bloodthirst_heal() : SpellScriptLoader("spell_warr_bloodthirst_heal") { }

    class spell_warr_bloodthirst_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_bloodthirst_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BLOODTHIRST_DAMAGE))
                SetHitHeal(GetCaster()->CountPctFromMaxHealth(spellInfo->Effects[EFFECT_1].CalcValue(GetCaster())));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_bloodthirst_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_bloodthirst_heal_SpellScript();
    }
};

// 7384, 7887, 11584, 11585 - Overpower
class spell_warr_overpower : public SpellScriptLoader
{
public:
    spell_warr_overpower() : SpellScriptLoader("spell_warr_overpower") { }

    class spell_warr_overpower_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_overpower_SpellScript);

        void HandleEffect(SpellEffIndex /*effIndex*/)
        {
            uint32 spellId = 0;
            if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1))
                spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1;
            else if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2))
                spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2;

            if (!spellId)
                return;

            if (Player* target = GetHitPlayer())
                if (target->HasUnitState(UNIT_STATE_CASTING))
                    target->CastSpell(target, spellId, true, 0, 0, GetCaster()->GetGUID());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_overpower_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_ANY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_overpower_SpellScript();
    }
};

// 5246 - Intimidating Shout
class spell_warr_intimidating_shout : public SpellScriptLoader
{
public:
    spell_warr_intimidating_shout() : SpellScriptLoader("spell_warr_intimidating_shout") { }

    class spell_warr_intimidating_shout_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_intimidating_shout_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove(GetExplTargetWorldObject());
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_intimidating_shout_SpellScript();
    }
};

// -772 - Rend
class spell_warr_rend : public SpellScriptLoader
{
public:
    spell_warr_rend() : SpellScriptLoader("spell_warr_rend") { }

    class spell_warr_rend_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_rend_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;

                // $0.2 * (($MWB + $mwb) / 2 + $AP / 14 * $MWS) bonus per tick
                float ap = caster->GetTotalAttackPowerValue(BASE_ATTACK);
                int32 mws = caster->GetAttackTime(BASE_ATTACK);
                float mwbMin = caster->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE);
                float mwbMax = caster->GetWeaponDamageRange(BASE_ATTACK, MAXDAMAGE);
                float mwb = ((mwbMin + mwbMax) / 2 + ap * mws / 14000) * 0.2f;
                amount += int32(caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), mwb));

                // "If used while your target is above 75% health, Rend does 35% more damage."
                // as for 3.1.3 only ranks above 9 (wrong tooltip?)
                if (GetSpellInfo()->GetRank() >= 9)
                {
                    if (GetUnitOwner()->HasAuraState(AURA_STATE_HEALTH_ABOVE_75_PERCENT, GetSpellInfo(), caster))
                        AddPct(amount, GetSpellInfo()->Effects[EFFECT_2].CalcValue(caster));
                }
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_rend_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_rend_AuraScript();
    }
};

// 64380, 65941 - Shattering Throw
class spell_warr_shattering_throw : public SpellScriptLoader
{
public:
    spell_warr_shattering_throw() : SpellScriptLoader("spell_warr_shattering_throw") { }

    class spell_warr_shattering_throw_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_shattering_throw_SpellScript);

        void HandleScript(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            // remove shields, will still display immune to damage part
            if (Unit* target = GetHitUnit())
                target->RemoveAurasWithMechanic(1 << MECHANIC_IMMUNE_SHIELD, AURA_REMOVE_BY_ENEMY_SPELL);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_shattering_throw_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_shattering_throw_SpellScript();
    }
};

// 12328, 18765, 35429 - Sweeping Strikes
class spell_warr_sweeping_strikes : public SpellScriptLoader
{
public:
    spell_warr_sweeping_strikes() : SpellScriptLoader("spell_warr_sweeping_strikes") { }

    class spell_warr_sweeping_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_sweeping_strikes_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK });
        }

        bool Load() override
        {
            _procTarget = nullptr;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            _procTarget = eventInfo.GetActor()->SelectNearbyNoTotemTarget(eventInfo.GetProcTarget());
            return _procTarget && !eventInfo.GetDamageInfo()->GetSpellInfo();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 damage = eventInfo.GetDamageInfo()->GetDamage();
            GetTarget()->CastCustomSpell(_procTarget, SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK, &damage, 0, 0, true, nullptr, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_sweeping_strikes_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_sweeping_strikes_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }

    private:
        Unit* _procTarget;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_sweeping_strikes_AuraScript();
    }
};

// 50720 - Vigilance
class spell_warr_vigilance : public SpellScriptLoader
{
public:
    spell_warr_vigilance() : SpellScriptLoader("spell_warr_vigilance") { }

    class spell_warr_vigilance_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_vigilance_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_WARRIOR_GLYPH_OF_VIGILANCE,
                    SPELL_WARRIOR_VIGILANCE_PROC,
                    SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT,
                    SPELL_GEN_DAMAGE_REDUCTION_AURA
                });
        }

        bool Load() override
        {
            _procTarget = nullptr;
            return true;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->CastSpell(target, SPELL_GEN_DAMAGE_REDUCTION_AURA, true);

            if (Unit* caster = GetCaster())
                target->CastSpell(caster, SPELL_WARRIOR_VIGILANCE_REDIRECT_THREAT, true);
        }

        void HandleAfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            //! WORKAROUND
            //! this glyph is a proc
            if (Unit* caster = GetCaster())
            {
                if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_WARRIOR_GLYPH_OF_VIGILANCE, EFFECT_0))
                    GetTarget()->ModifyRedirectThreat(glyph->GetAmount());
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (target->HasAura(SPELL_GEN_DAMAGE_REDUCTION_AURA) &&
                    !(target->HasAura(SPELL_PALADIN_BLESSING_OF_SANCTUARY) ||
                      target->HasAura(SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY) ||
                      target->HasAura(SPELL_PRIEST_RENEWED_HOPE)))
            {
                target->RemoveAurasDueToSpell(SPELL_GEN_DAMAGE_REDUCTION_AURA);
            }

            target->ResetRedirectThreat();
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            _procTarget = GetCaster();
            return _procTarget;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(_procTarget, SPELL_WARRIOR_VIGILANCE_PROC, true, nullptr, aurEff);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_warr_vigilance_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectApply += AuraEffectApplyFn(spell_warr_vigilance_AuraScript::HandleAfterApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_warr_vigilance_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            DoCheckProc += AuraCheckProcFn(spell_warr_vigilance_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_vigilance_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }

    private:
        Unit* _procTarget;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_vigilance_AuraScript();
    }
};

// 50725 - Vigilance
class spell_warr_vigilance_trigger : public SpellScriptLoader
{
public:
    spell_warr_vigilance_trigger() : SpellScriptLoader("spell_warr_vigilance_trigger") { }

    class spell_warr_vigilance_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_vigilance_trigger_SpellScript);

        void HandleScript(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            // Remove Taunt cooldown
            if (Player* target = GetHitPlayer())
                target->RemoveSpellCooldown(SPELL_WARRIOR_TAUNT, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_vigilance_trigger_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_vigilance_trigger_SpellScript();
    }
};

// 58387 - Glyph of Sunder Armor
class spell_warr_glyph_of_sunder_armor : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_sunder_armor() : SpellScriptLoader("spell_warr_glyph_of_sunder_armor") { }

    class spell_warr_glyph_of_sunder_armor_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_glyph_of_sunder_armor_AuraScript);

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(aurEff->GetBase());
                spellMod->op = SpellModOp(aurEff->GetMiscValue());
                spellMod->type = SPELLMOD_FLAT;
                spellMod->spellId = GetId();
                spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
            }

            spellMod->value = aurEff->GetAmount();
        }

        void Register() override
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_warr_glyph_of_sunder_armor_AuraScript::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_glyph_of_sunder_armor_AuraScript();
    }
};

// 20230 - Retaliation
class spell_warr_retaliation : public SpellScriptLoader
{
public:
    spell_warr_retaliation() : SpellScriptLoader("spell_warr_retaliation") { }

    class spell_warr_retaliation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_retaliation_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_WARRIOR_RETALIATION_DAMAGE });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // check attack comes not from behind and warrior is not stunned
            return GetTarget()->isInFront(eventInfo.GetActor(), M_PI) && !GetTarget()->HasUnitState(UNIT_STATE_STUNNED);
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_WARRIOR_RETALIATION_DAMAGE, true, nullptr, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_retaliation_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_retaliation_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warr_retaliation_AuraScript();
    }
};

void AddSC_warrior_spell_scripts()
{
    // Ours
    new spell_warr_mocking_blow();
    new spell_warr_intervene();
    new spell_warr_improved_spell_reflection();
    new spell_warr_improved_spell_reflection_trigger();
    new spell_warr_victory_rush();

    // Theirs
    new spell_warr_bloodthirst();
    new spell_warr_bloodthirst_heal();
    new spell_warr_charge();
    new spell_warr_concussion_blow();
    new spell_warr_damage_shield();
    new spell_warr_deep_wounds();
    new spell_warr_execute();
    new spell_warr_glyph_of_sunder_armor();
    new spell_warr_intimidating_shout();
    new spell_warr_last_stand();
    new spell_warr_overpower();
    new spell_warr_rend();
    new spell_warr_retaliation();
    new spell_warr_shattering_throw();
    new spell_warr_slam();
    new spell_warr_sweeping_strikes();
    new spell_warr_vigilance();
    new spell_warr_vigilance_trigger();
}
