#include "include/native_view/native_view_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "native_view_plugin.h"

void NativeViewPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  native_view::NativeViewPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
