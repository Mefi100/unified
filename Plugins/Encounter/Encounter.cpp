#include "Encounter.hpp"

#include "API/CAppManager.hpp"
#include "API/CServerExoApp.hpp"
#include "API/CNWSEncounter.hpp"
#include "API/CEncounterSpawnPoint.hpp"
#include "API/CEncounterListEntry.hpp"
#include "API/Constants.hpp"
#include "API/Globals.hpp"
#include "Services/Events/Events.hpp"


using namespace NWNXLib;
using namespace NWNXLib::API;

static Encounter::Encounter* g_plugin;

NWNX_PLUGIN_ENTRY Plugin* PluginLoad(Services::ProxyServiceList* services)
{
    g_plugin = new Encounter::Encounter(services);
    return g_plugin;
}


namespace Encounter {

Encounter::Encounter(Services::ProxyServiceList* services)
    : Plugin(services)
{
#define REGISTER(func) \
    GetServices()->m_events->RegisterEvent(#func, \
        [this](ArgumentStack&& args){ return func(std::move(args)); })

    REGISTER(GetNumberOfCreaturesInEncounterList);
    REGISTER(GetEncounterCreatureByIndex);
    REGISTER(SetEncounterCreatureByIndex);
    REGISTER(GetFactionId);
    REGISTER(SetFactionId);
    REGISTER(GetPlayerTriggeredOnly);
    REGISTER(SetPlayerTriggeredOnly);
    REGISTER(GetResetTime);
    REGISTER(SetResetTime);
    REGISTER(GetNumberOfSpawnPoints);
    REGISTER(GetSpawnPointByIndex);
    REGISTER(GetMinNumSpawned);
    REGISTER(GetMaxNumSpawned);
    REGISTER(GetCurrentNumSpawned);

#undef REGISTER
}

Encounter::~Encounter()
{
}

CNWSEncounter *Encounter::encounter(ArgumentStack& args)
{
    const auto encounterId = Services::Events::ExtractArgument<ObjectID>(args);

    if (encounterId == Constants::OBJECT_INVALID)
    {
        LOG_NOTICE("NWNX_Encounter function called on OBJECT_INVALID");
        return nullptr;
    }

    auto *pEncounter = Globals::AppManager()->m_pServerExoApp->GetEncounterByGameObjectID(encounterId);

    if (!pEncounter)
    {
        LOG_NOTICE("NWNX_Encounter function called on non-encounter object %x", encounterId);
    }

    return pEncounter;
}

ArgumentStack Encounter::GetNumberOfCreaturesInEncounterList(ArgumentStack&& args)
{
    int32_t retVal = 0;

    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_nNumEncounterListEntries;
    }

    return Services::Events::Arguments(retVal);
}

ArgumentStack Encounter::GetEncounterCreatureByIndex(ArgumentStack&& args)
{
    std::string resRef = "";
    float cr = 0.0;
    int32_t unique = 0;
    int32_t alreadyUsed = 0;

    if (auto *pEncounter = encounter(args))
    {
        const auto index = Services::Events::ExtractArgument<int32_t>(args);
        ASSERT_OR_THROW(index >= 0.0);

        if (index < pEncounter->m_nNumEncounterListEntries)
        {
            resRef = pEncounter->m_pEncounterList[index].m_cCreatureResRef.GetResRefStr();
            cr = pEncounter->m_pEncounterList[index].m_fCR;
            unique = pEncounter->m_pEncounterList[index].m_bUnique;
            alreadyUsed = pEncounter->m_pEncounterList[index].m_bAlreadyUsed;
        }
    }

    return Services::Events::Arguments(resRef, cr, unique, alreadyUsed);
}

ArgumentStack Encounter::SetEncounterCreatureByIndex(ArgumentStack&& args)
{
    if (auto *pEncounter = encounter(args))
    {
        const auto index = Services::Events::ExtractArgument<int32_t>(args);
        const auto resRef = Services::Events::ExtractArgument<std::string>(args);
        auto cr = Services::Events::ExtractArgument<float>(args);
        ASSERT_OR_THROW(cr >= 0.0);
        auto unique = Services::Events::ExtractArgument<int32_t>(args);
        unique = !!unique;
        auto alreadyUsed = Services::Events::ExtractArgument<int32_t>(args);
        alreadyUsed = !!alreadyUsed;

        if (index < pEncounter->m_nNumEncounterListEntries)
        {
            pEncounter->m_pEncounterList[index].m_cCreatureResRef = resRef.c_str();
            pEncounter->m_pEncounterList[index].m_fCR = cr;
            pEncounter->m_pEncounterList[index].m_fCreaturePoints = pEncounter->CalculatePointsFromCR(cr);
            pEncounter->m_pEncounterList[index].m_bAlreadyUsed = alreadyUsed;
            pEncounter->m_pEncounterList[index].m_bUnique = unique;
        }
    }

    return Services::Events::Arguments();
}

ArgumentStack Encounter::GetFactionId(ArgumentStack&& args)
{
    int32_t retVal = 0;

    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_nFactionId;
    }

    return Services::Events::Arguments(retVal);
}

ArgumentStack Encounter::SetFactionId(ArgumentStack&& args)
{
    if (auto *pEncounter = encounter(args))
    {
        auto factionId = Services::Events::ExtractArgument<int32_t>(args);

        ASSERT_OR_THROW(factionId >= 0);

        pEncounter->m_nFactionId = factionId;
    }

    return Services::Events::Arguments();
}

ArgumentStack Encounter::GetPlayerTriggeredOnly(ArgumentStack&& args)
{
    int32_t retVal = 0;

    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_bPlayerTriggeredOnly;
    }

    return Services::Events::Arguments(retVal);
}

ArgumentStack Encounter::SetPlayerTriggeredOnly(ArgumentStack&& args)
{
    if (auto *pEncounter = encounter(args))
    {
        auto playerTriggeredOnly = Services::Events::ExtractArgument<int32_t>(args);

        playerTriggeredOnly = !!playerTriggeredOnly;

        pEncounter->m_bPlayerTriggeredOnly = playerTriggeredOnly;
    }

    return Services::Events::Arguments();
}

ArgumentStack Encounter::GetResetTime(ArgumentStack&& args)
{
    int32_t retVal = 0;

    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_nResetTime;
    }

    return Services::Events::Arguments(retVal);
}

ArgumentStack Encounter::SetResetTime(ArgumentStack&& args)
{
    if (auto *pEncounter = encounter(args))
    {
        auto resetTime = Services::Events::ExtractArgument<int32_t>(args);
        ASSERT_OR_THROW(resetTime >= 0);

        pEncounter->m_nResetTime = resetTime;
    }

    return Services::Events::Arguments();
}

ArgumentStack Encounter::GetNumberOfSpawnPoints(ArgumentStack&& args)
{
    int32_t retVal = 0;

    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_nNumSpawnPoints;
    }

    return Services::Events::Arguments(retVal);
}

ArgumentStack Encounter::GetSpawnPointByIndex(ArgumentStack&& args)
{
    float x = 0.0, y = 0.0, z = 0.0, o = 0.0;

    if (auto *pEncounter = encounter(args))
    {
        const auto index = Services::Events::ExtractArgument<int32_t>(args);
        ASSERT_OR_THROW(index >= 0);

        if (index < pEncounter->m_nNumSpawnPoints)
        {
            x = pEncounter->m_pSpawnPointList[index].m_vPosition.x;
            y = pEncounter->m_pSpawnPointList[index].m_vPosition.y;
            z = pEncounter->m_pSpawnPointList[index].m_vPosition.z;
            o = pEncounter->m_pSpawnPointList[index].m_fOrientation;
        }
    }

    return Services::Events::Arguments(x, y, z, o);
}

ArgumentStack Encounter::GetMinNumSpawned(ArgumentStack&& args)
{
    int32_t retVal = 0;
    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_nMinNumSpawnedCreatures;
    }

    return Services::Events::Arguments(retVal);
}

ArgumentStack Encounter::GetMaxNumSpawned(ArgumentStack&& args)
{
    int32_t retVal = 0;
    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_nMaxSpawnedCreatures;
    }

    return Services::Events::Arguments(retVal);
}

ArgumentStack Encounter::GetCurrentNumSpawned(ArgumentStack&& args)
{
    int32_t retVal = 0;
    if (auto *pEncounter = encounter(args))
    {
        retVal = pEncounter->m_nNumSpawnedCreatures;
    }

    return Services::Events::Arguments(retVal);
}

}
