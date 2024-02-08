import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'native_view_method_channel.dart';

abstract class NativeViewPlatform extends PlatformInterface {
  /// Constructs a NativeViewPlatform.
  NativeViewPlatform() : super(token: _token);

  static final Object _token = Object();

  static NativeViewPlatform _instance = MethodChannelNativeView();

  /// The default instance of [NativeViewPlatform] to use.
  ///
  /// Defaults to [MethodChannelNativeView].
  static NativeViewPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [NativeViewPlatform] when
  /// they register themselves.
  static set instance(NativeViewPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
