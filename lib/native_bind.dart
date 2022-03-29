import 'dart:ffi';
import 'dart:isolate';

import 'package:ffi/ffi.dart';

import 'glib_native_gen.dart';
import 'glib_native_lib.dart';

ReceivePort? receivePort;
bool closed = false;

void registerPort() async {
  receivePort = ReceivePort()..listen(requestListenFunc);
  final int nativePort = receivePort!.sendPort.nativePort;
  nativeLib.native_initialize_dart_api(NativeApi.initializeApiDLData);
  nativeLib.native_register_dart_port(nativePort);

  native_run_func mainThreadCB = Pointer.fromFunction(nativeMainThreadRun);
  nativeLib.native_register_main_thread_callback(mainThreadCB);

  startWork();
  while (!closed) {
    await asyncSleep(500);
  }
}

void nativeMainThreadRun(Pointer<NativeFuncParam> paramPtr) {
  print("Callback from main thread");
  final param = paramPtr.ref;
  switch (param.func_type) {
    case NativeFuncTypes.HELLO:
      hello();
      break;
    case NativeFuncTypes.SAY:
      say(param.params[0].cast<Utf8>().toDartString());
      break;
    default:
      break;
  }
}

void startWork() async {
  nativeLib.native_start_work();
}

void requestListenFunc(dynamic message) {
  print("Call back from dart port(threaded native)");
  final paramPtr = Pointer<NativeFuncParam>.fromAddress(message);
  final param = paramPtr.ref;
  switch (param.func_type) {
    case NativeFuncTypes.HELLO:
      hello();
      break;
    case NativeFuncTypes.SAY:
      say(param.params[0].cast<Utf8>().toDartString());
      break;
    default:
      break;
  }
  nativeLib.native_work_finished(paramPtr);
}

void hello() {
  print("Hello!");
}

void say(String content) {
  print("Say: \"$content\"");
}

void closePort() {
  closed = true;
  receivePort!.close();
}

Future asyncSleep(int ms) {
  return Future.delayed(Duration(milliseconds: ms));
}
