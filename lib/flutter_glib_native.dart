
import 'dart:async';

import 'package:flutter/services.dart';

import 'native_bind.dart';

class FlutterGlibNative {
  static const MethodChannel _channel = MethodChannel('flutter_glib_native');

  static Future<String?> get platformVersion async {
    registerPort();
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
