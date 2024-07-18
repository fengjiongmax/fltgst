import Cocoa
import FlutterMacOS

typealias setNativeView = @convention(c) (NSView) -> Void

class NativeViewFactory: NSObject, FlutterPlatformViewFactory {
    
  private var messenger: FlutterBinaryMessenger

  init(messenger: FlutterBinaryMessenger) {
    self.messenger = messenger
    super.init()
  }

  func create(
    withViewIdentifier viewId: Int64,
    arguments args: Any?
  ) -> NSView {
    return NativeView(
        viewIdentifier: viewId,
        arguments: args,
        binaryMessenger: messenger)
  }

  /// Implementing this method is only necessary when the `arguments` in `createWithFrame` is not `nil`.
  public func createArgsCodec() -> (FlutterMessageCodec & NSObjectProtocol)? {
    return FlutterStandardMessageCodec.sharedInstance()
  }
}

class NativeView: NSView {

  init(
    viewIdentifier viewId: Int64,
    arguments args: Any?,
    binaryMessenger messenger: FlutterBinaryMessenger?
  ) {
    let frame = CGRect(x: 0, y: 0, width: 200, height: 200)
    super.init(frame: frame)
    super.wantsLayer = true
//    super.layer?.backgroundColor = NSColor.systemBlue.cgColor
//    super.frame = frame
    // macOS views can be created here
    createNativeView(view: self)
  }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
  func createNativeView(view _view: NSView) {
      let video_view = NSView()
      video_view.frame = CGRect(x:0,y:0,width: 480,height: 480)
      _view.addSubview(video_view)
      let handle = dlopen("native_binding.framework/native_binding", RTLD_NOW)
      let sym = dlsym(handle, "set_native_window")
      if(sym != nil){
          let f = unsafeBitCast(sym, to: setNativeView.self)
          f(video_view)
      }
  }
}
