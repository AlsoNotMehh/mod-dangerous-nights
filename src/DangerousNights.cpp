/*
 * AzerothCore Module: mod-dangerous-nights
 * Author: AlsoNotMehh
 * License: GNU AGPL v3
 *
 * Manages dynamic nighttime rewards and loot: boosted experience,
 * extra gold, and bonus item drop rolls during night hours across Azeroth.
 */

#include "Chat.h"
#include "Config.h"
#include "Creature.h"
#include "GameTime.h"
#include "LootMgr.h"
#include "Map.h"
#include "Player.h"
#include "PlayerScript.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "World.h"
#include "WorldScript.h"
#include "WorldSessionMgr.h"

#include <cmath>
#include <ctime>

namespace
{
    bool   g_enabled             = true;
    uint32 g_nightStartHour      = 20;   // 8:00 PM
    uint32 g_nightEndHour        = 6;    // 6:00 AM
    float  g_fastClockSpeed      = 0.0f; // 0.0 = server real time, 3.0 = fast 4h/4h cycle
    float  g_xpMultiplier        = 1.50f; // +50% XP at night
    float  g_goldMultiplier      = 1.50f; // +50% Gold at night
    float  g_extraLootChance     = 20.0f; // 20% extra item drop chance
    bool   g_allowInDungeons     = false;
    bool   g_announceTransitions = true;

    bool   g_lastNightState      = false;
    uint32 g_checkTimer          = 0;

    uint32 GetCurrentHour()
    {
        time_t now = GameTime::GetGameTime().count();

        if (g_fastClockSpeed > 0.0f)
        {
            double visualSeconds = std::fmod(static_cast<double>(now) * g_fastClockSpeed, 86400.0);
            if (visualSeconds < 0.0)
                visualSeconds += 86400.0;
            return static_cast<uint32>(visualSeconds / 3600.0) % 24;
        }

        tm localTm;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&localTm, &now);
#else
        localtime_r(&now, &localTm);
#endif
        return static_cast<uint32>(localTm.tm_hour);
    }

    bool IsNightTime()
    {
        if (!g_enabled)
            return false;

        uint32 hour = GetCurrentHour();

        if (g_nightStartHour > g_nightEndHour)
            return (hour >= g_nightStartHour || hour < g_nightEndHour);
        else
            return (hour >= g_nightStartHour && hour < g_nightEndHour);
    }

    bool IsEligibleCreature(Creature const* creature)
    {
        if (!creature)
            return false;

        if (creature->IsPet() || creature->IsTotem() || creature->IsControlledByPlayer())
            return false;

        if (!g_allowInDungeons && creature->GetMap() && creature->GetMap()->IsDungeon())
            return false;

        CreatureTemplate const* cInfo = creature->GetCreatureTemplate();
        if (!cInfo)
            return false;

        // Ignore harmless critters
        if (cInfo->type == CREATURE_TYPE_CRITTER)
            return false;

        return true;
    }
}

class DangerousNights_WorldScript : public WorldScript
{
public:
    DangerousNights_WorldScript() : WorldScript("DangerousNights_WorldScript") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        g_enabled             = sConfigMgr->GetOption<bool>("DangerousNights.Enable", true);
        g_nightStartHour      = sConfigMgr->GetOption<uint32>("DangerousNights.NightStartHour", 20);
        g_nightEndHour        = sConfigMgr->GetOption<uint32>("DangerousNights.NightEndHour", 6);
        g_fastClockSpeed      = sConfigMgr->GetOption<float>("DangerousNights.FastClockSpeed", 0.0f);
        g_xpMultiplier        = sConfigMgr->GetOption<float>("DangerousNights.XPMultiplier", 1.50f);
        g_goldMultiplier      = sConfigMgr->GetOption<float>("DangerousNights.GoldMultiplier", 1.50f);
        g_extraLootChance     = sConfigMgr->GetOption<float>("DangerousNights.ExtraLootChance", 20.0f);
        g_allowInDungeons     = sConfigMgr->GetOption<bool>("DangerousNights.AllowInDungeons", false);
        g_announceTransitions = sConfigMgr->GetOption<bool>("DangerousNights.AnnounceTransitions", true);

        g_lastNightState = IsNightTime();
    }

    void OnUpdate(uint32 diff) override
    {
        if (!g_enabled || !g_announceTransitions)
            return;

        g_checkTimer += diff;
        if (g_checkTimer < 5000) // Check every 5 seconds
            return;

        g_checkTimer = 0;
        bool isNight = IsNightTime();

        if (isNight != g_lastNightState)
        {
            g_lastNightState = isNight;
            if (isNight)
            {
                sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, 
                    "|cffff2020[Dangerous Nights]|r The shadows deepen as night falls across Azeroth... Nighttime reward multipliers are now active!");
            }
            else
            {
                sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, 
                    "|cff20ff20[Dangerous Nights]|r The dawn breaks and sunlight returns... Nighttime bonuses have ended.");
            }
        }
    }
};

class DangerousNights_PlayerScript : public PlayerScript
{
public:
    DangerousNights_PlayerScript()
        : PlayerScript("DangerousNights_PlayerScript", {
            PLAYERHOOK_ON_GIVE_EXP,
            PLAYERHOOK_ON_BEFORE_LOOT_MONEY,
            PLAYERHOOK_ON_BEFORE_SEND_LOOT
        }) { }

    void OnPlayerGiveXP(Player* /*player*/, uint32& amount, Unit* victim, uint8 /*xpSource*/) override
    {
        if (!g_enabled || g_xpMultiplier <= 1.0f || !victim || !IsNightTime())
            return;

        Creature* creature = victim->ToCreature();
        if (!IsEligibleCreature(creature))
            return;

        amount = static_cast<uint32>(amount * g_xpMultiplier);
    }

    void OnPlayerBeforeLootMoney(Player* /*player*/, Loot* loot) override
    {
        if (!g_enabled || g_goldMultiplier <= 1.0f || !loot || !IsNightTime())
            return;

        if (loot->gold > 0)
        {
            loot->gold = static_cast<uint32>(loot->gold * g_goldMultiplier);
        }
    }

    void OnPlayerBeforeSendLoot(Player* /*player*/, ObjectGuid /*lootGuid*/, Loot* loot) override
    {
        if (!g_enabled || g_extraLootChance <= 0.0f || !loot || !IsNightTime())
            return;

        for (LootItem& item : loot->items)
        {
            if (roll_chance_f(g_extraLootChance))
            {
                item.count = static_cast<uint8>(item.count + 1);
            }
        }
    }
};

void AddDangerousNightsScripts()
{
    new DangerousNights_WorldScript();
    new DangerousNights_PlayerScript();
}
