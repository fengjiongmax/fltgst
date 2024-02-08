import 'package:flutter_test/flutter_test.dart';
import 'package:native_view/native_view.dart';
import 'package:native_view/native_view_platform_interface.dart';
import 'package:native_view/native_view_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockNativeViewPlatform
    with MockPlatformInterfaceMixin
    implements NativeViewPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final NativeViewPlatform initialPlatform = NativeViewPlatform.instance;

  test('$MethodChannelNativeView is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelNativeView>());
  });

  test('getPlatformVersion', () async {
    NativeView nativeViewPlugin = NativeView();
    MockNativeViewPlatform fakePlatform = MockNativeViewPlatform();
    NativeViewPlatform.instance = fakePlatform;

    expect(await nativeViewPlugin.getPlatformVersion(), '42');
  });
}
