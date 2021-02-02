#include "Events/AssociateEvents.hpp"
#include "API/CNWSCreature.hpp"
#include "API/Functions.hpp"
#include "Events.hpp"

namespace Events {

using namespace NWNXLib;

static Hooks::Hook s_AddAssociateHook;
static Hooks::Hook s_RemoveAssociateHook;

AssociateEvents::AssociateEvents()
{
     Events::InitOnFirstSubscribe("NWNX_ON_ADD_ASSOCIATE_.*", []() {
         s_AddAssociateHook = Hooks::HookFunction(API::Functions::_ZN12CNWSCreature12AddAssociateEjt,
                                           (void*)&AddAssociateHook, Hooks::Order::Earliest);
    });

    Events::InitOnFirstSubscribe("NWNX_ON_REMOVE_ASSOCIATE_.*", []() {
        s_RemoveAssociateHook = Hooks::HookFunction(API::Functions::_ZN12CNWSCreature15RemoveAssociateEj,
                                             (void*)&RemoveAssociateHook, Hooks::Order::Earliest);
    });
}

void AssociateEvents::AddAssociateHook(CNWSCreature* thisPtr, ObjectID oidAssociate, uint16_t nAssociateType)
{
    Events::PushEventData("ASSOCIATE_OBJECT_ID", NWNXLib::Utils::ObjectIDToString(oidAssociate));
    Events::SignalEvent("NWNX_ON_ADD_ASSOCIATE_BEFORE", thisPtr->m_idSelf);
    s_AddAssociateHook->CallOriginal<void>(thisPtr, oidAssociate, nAssociateType);
    Events::PushEventData("ASSOCIATE_OBJECT_ID", NWNXLib::Utils::ObjectIDToString(oidAssociate));
    Events::SignalEvent("NWNX_ON_ADD_ASSOCIATE_AFTER", thisPtr->m_idSelf);
}

void AssociateEvents::RemoveAssociateHook(CNWSCreature* thisPtr, ObjectID oidAssociate)
{
    Events::PushEventData("ASSOCIATE_OBJECT_ID", NWNXLib::Utils::ObjectIDToString(oidAssociate));
    Events::SignalEvent("NWNX_ON_REMOVE_ASSOCIATE_BEFORE", thisPtr->m_idSelf);
    s_RemoveAssociateHook->CallOriginal<void>(thisPtr, oidAssociate);
    Events::PushEventData("ASSOCIATE_OBJECT_ID", NWNXLib::Utils::ObjectIDToString(oidAssociate));
    Events::SignalEvent("NWNX_ON_REMOVE_ASSOCIATE_AFTER", thisPtr->m_idSelf);
}

}
