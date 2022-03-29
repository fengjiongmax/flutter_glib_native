#import "FlutterGlibNativePlugin.h"
#if __has_include(<flutter_glib_native/flutter_glib_native-Swift.h>)
#import <flutter_glib_native/flutter_glib_native-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "flutter_glib_native-Swift.h"
#endif

@implementation FlutterGlibNativePlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftFlutterGlibNativePlugin registerWithRegistrar:registrar];
}
@end
