import Flutter
import UIKit

typealias setNativeView = @convention(c) (UIView) -> Void

class FLNativeViewFactory: NSObject,FlutterPlatformViewFactory {
    private var messenger: FlutterBinaryMessenger

    init(messenger: FlutterBinaryMessenger) {
        self.messenger = messenger
        super.init()
    }
    
    func create(withFrame frame: CGRect, viewIdentifier viewId: Int64, arguments args: Any?) -> any FlutterPlatformView {
        return FLNativeView(
            frame: frame, viewIdentifier: viewId, arguments: args, binaryMessenger: messenger
        )
    }
}

class FLNativeView:NSObject,FlutterPlatformView{
    private var _view:UIView
    
    init(frame: CGRect,
         viewIdentifier viewId: Int64,
         arguments args: Any?,
         binaryMessenger messenger: FlutterBinaryMessenger?) {
        self._view = UIView()
        super.init()
        createNativeView(view:_view)
    }
    
    func view() -> UIView {
        return _view
    }
    
    func createNativeView(view _view:UIView){
        let video_view = UIView()
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
