# **Bing CoreUX V8 plugin Development** #

In order to add more functionalities in the plugin you need to build V8 source code without any BingUX plugin code. First Open Visual Studio 2017 Developer Command Prompt then execute following command

**Command:**
```
>> V8Build.cmd init -np debug
or
>> V8Build.cmd -np debug (if you have the V8 source code)
```

After build is complete then copy the following header and library to run this C++ project perfectly.

1. copy the lib\debug\native folder to V8_Native_Plugin\native.
2. copy include folder to V8_Native_Plugin\plugin\include.
3. Open the project via Visual studio and fix any build error(could be header path) then build it.
4. copy v8.dll, v8_libbase.dll and v8_libplatform.dll to the V8_Native_Plugin\x64\Debug directory to the run the exe properly