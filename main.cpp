#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <winrt/Windows.Networking.NetworkOperators.h>
#include <windows.h>

// 解决 LNK2001 无法解析符号的关键：手动链接系统库
#pragma comment(lib, "windowsapp")

using namespace winrt;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Networking::NetworkOperators;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 初始化 COM 环境
    init_apartment();

    try {
        auto profile = NetworkInformation::GetInternetConnectionProfile();
        if (profile) {
            auto manager = NetworkOperatorTetheringManager::CreateFromConnectionProfile(profile);

            // 只要不是开启状态，就执行开启
            if (manager.TetheringOperationalState() != TetheringOperationalState::On) {
                // get() 会阻塞直到任务完成
                manager.StartTetheringAsync().get();
            }
        }
    }
    catch (...) {
        // 捕获所有错误，确保开机时即使没网络也不会弹报错对话框
        return 1;
    }
    return 0;
}