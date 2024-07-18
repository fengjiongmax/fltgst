import UIKit
import Flutter

@UIApplicationMain
@objc class AppDelegate: FlutterAppDelegate {
  override func application(
    _ application: UIApplication,
    didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?
  ) -> Bool {
    GeneratedPluginRegistrant.register(with: self)
      
//      guard let pluginRegisterar = self.registrar(forPlugin:"NativeViewPlugin") else { return false }
//    
//      let factory = FLNativeViewFactory(messenger: pluginRegisterar.messenger())
//    
//      pluginRegisterar.register(factory,
//                                withId:"fltgst/native_view")
      
      
    return super.application(application, didFinishLaunchingWithOptions: launchOptions)
  }
}
