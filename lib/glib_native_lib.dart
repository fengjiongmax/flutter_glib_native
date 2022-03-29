import 'dart:ffi';
import 'dart:io';

import 'glib_native_gen.dart';

class GLibNative {
  static final GLibNative _singleton = GLibNative._internal();
  GLibNativeGenerated? nativeGenerated;

  factory GLibNative() {
    if (_singleton.nativeGenerated == null) {
      String libName = 'libglib_native.so';
      if (Platform.isAndroid) {
      } else if (Platform.isIOS) {
        libName = 'libglib_native.dylib';
      } else if (Platform.isMacOS) {
      } else if (Platform.isLinux) {
      } else if (Platform.isWindows) {
        libName = 'libglib_native.dll';
      }
      DynamicLibrary nativeNRMobile = DynamicLibrary.open(libName);

      _singleton.nativeGenerated = GLibNativeGenerated(nativeNRMobile);
    }
    return _singleton;
  }

  GLibNative._internal();
}

final GLibNativeGenerated nativeLib = GLibNative().nativeGenerated!;
