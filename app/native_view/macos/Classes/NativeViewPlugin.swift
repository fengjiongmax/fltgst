import Cocoa
import FlutterMacOS

public class NativeViewPlugin: NSObject, FlutterPlugin {
    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "native_view", binaryMessenger: registrar.messenger)
        let instance = NativeViewPlugin()
        registrar.addMethodCallDelegate(instance, channel: channel)
        let factory = NativeViewFactory(messenger: registrar.messenger)
        registrar.register(factory, withId: "fltgst/native_view")
    }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "getPlatformVersion":
      result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
    default:
      result(FlutterMethodNotImplemented)
    }
  }
}
