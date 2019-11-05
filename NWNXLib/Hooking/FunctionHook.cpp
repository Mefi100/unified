#include "Hooking/FunctionHook.hpp"
#include "Platform/ASLR.hpp"
#include "Assert.hpp"

namespace NWNXLib {

namespace Hooking {

using namespace NWNXLib::Platform;

FunctionHook::FunctionHook(uintptr_t originalFunction, uintptr_t newFunction)
{
    int rv;

    m_funchook = funchook_create();

    rv = funchook_prepare(m_funchook, (void **) &originalFunction, (void *) newFunction);
    ASSERT(rv == FUNCHOOK_ERROR_SUCCESS);

    rv = funchook_install(m_funchook, 0);
    ASSERT(rv == FUNCHOOK_ERROR_SUCCESS);

    m_trampoline = (void *) originalFunction;
    ASSERT(m_trampoline);
}

FunctionHook::~FunctionHook()
{
    funchook_uninstall(m_funchook, 0);
    funchook_destroy(m_funchook);
}

}

}
