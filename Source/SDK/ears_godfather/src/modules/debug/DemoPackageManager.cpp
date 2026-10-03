#include "DemoPackageManager.h"

// SDK
#include "SDK/ears_godfather/src/modules/debug/DemoPackage.h"

// c++
#include <assert.h>

namespace EARS::Modules
{
    void DemoPackageManager::SetPendingPackageByIndex(const uint32_t InIndex)
    {
        const DemoPackage* Package = GetPackage(InIndex);
        assert(Package);

        SetPendingPackage(Package);
    }

    void DemoPackageManager::ClearPendingPackage()
    {
        SetPendingPackage(nullptr);
    }

    void DemoPackageManager::SetPendingPackage(const DemoPackage* InNewPackage)
    {
        m_ActivePackageGuid.Clear();

        if (InNewPackage)
        {
            m_PendingPackageGuid = *InNewPackage->GetInstanceID();
        }
        else
        {
            m_PendingPackageGuid.Clear();
        }
    }

    DemoPackageManager* EARS::Modules::DemoPackageManager::GetInstance()
    {
        return *(DemoPackageManager**)0x1129880;
    }
}
