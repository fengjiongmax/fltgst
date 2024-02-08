
import 'native_view_platform_interface.dart';

class NativeView {
  Future<String?> getPlatformVersion() {
    return NativeViewPlatform.instance.getPlatformVersion();
  }
}
