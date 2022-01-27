# **Bing CoreUX V8 plugin Test Project** #

In order to run the Tests project you need to build V8 source code with plugin code. First Open Visual Studio 2017 Developer Command Prompt then execute following command

**Command:**
```
>> V8Build.cmd init debug/release  
or
>> V8Build.cmd debug/release (if you have the V8 source code)
```

After build is complete  V8_monolith.lib and header files will be copied to lib folder. Then you need to build V8_Native project to generate v8.dll files.