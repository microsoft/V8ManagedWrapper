# **V8 Managed Library for Embedding Application** #

[V8](https://github.com/v8/v8) is Google’s open source high-performance JavaScript engine, written in C++. It is used in Google Chrome, the open source browser from Google, and in Node.js, among others. It implements ECMAScript as specified in ECMA-262, and runs on Windows 7 or later, macOS 10.12+, and Linux systems that use x64, IA-32, ARM, or MIPS processors. V8 can run standalone, or can be embedded into any C++ application.

This wrapper project will expose some of V8 functionalities to managed (e.g: C#) world to execute JavaScript. Bing UX platform is using this v8.dll in their Aspnet application to execute JavaScript on server.

## **How to Build V8 with Wrapper Code** ##
V8Build.cmd is a small batch file which will download V8 source code and build. V8_Native.sln is a  C project which expose V8 api for P/Invoke. If you interest to get more information about V8 build please visit [Here](https://github.com/v8/v8/wiki/Building-with-GN).

Please make sure you install all [prerequisite](https://chromium.googlesource.com/chromium/src/+/master/docs/windows_build_instructions.md#Setting-up-Windows) in your machine(install Visual Studio, Debugging tools for Windows). Here is the stuff to install in your  machine :

1. Visual Studio with Microsoft.VisualStudio.Workload.NativeDesktop and Microsoft.VisualStudio.Component.VC.ATLMFC
2. Windows SDK
3. Debugging Tools For Windows

Please follow **Setting up Windows [Visual Studio]** section in the [wiki](https://chromium.googlesource.com/chromium/src/+/master/docs/windows_build_instructions.md#Setting-up-Windows). You don't need to install depot_tools, V8Build.cmd will take care of all the V8 dependency.

# **Build Instruction ** #

V8Build.cmd will initialize  **Visual Studio 2019 Developer Command Prompt** and setup proper environment variable. Then It'll download depot_tools and V8 source code and build it.

```
**Command** for V8Build.cmd

**Arguments :**
   init    =  It will download V8 source code and dependency(depot_tools). If you already have the V8 source code then don't pass this flag.

   [mode]   = debug/release. This will build V8 in debug mode. Default mode is "release"

   [revision] = Optional. You can specify any specific version of V8. e.g: Examples: "master", "8.9.4".
```

1. Build V8 without having v8 source code.
```
   >> V8Build.cmd init debug/release(default) x64(default) / arm64
```

2. Build V8  (if you have downloaded the V8 code via init command)
```
   >> V8Build.cmd  debug/release(default)
```
**Output Folder Structure**
```
lib\
  ---v8_monolith.lib
  ---include
      ---- .h files
```
3. Open V8_Native\V8_Native.sln file and build it.
```
   cd V8_Native
   msbuild V8_Native.vcxproj /p:configuration=release /p:platform=x64
```

4. Open and build V8_Managed_Library. V8_Managed_Library\JsRT.V8 is the managed wrapper library which do P/Invoke V8.dll.
```
   cd V8_Managed_Library
   dotnet build
   ; running test project
   cd Tests
   dotnet run
```

