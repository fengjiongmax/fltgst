import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'native_view_platform_interface.dart';

/// An implementation of [NativeViewPlatform] that uses method channels.
class MethodChannelNativeView extends NativeViewPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('native_view');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
