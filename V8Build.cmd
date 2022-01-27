@echo off
:: Starts localization of environment variables in a batch file and  continues until a matching endlocal
setlocal

set vs2019=C:\Program Files (x86)\Microsoft Visual Studio\2019
set vs2022=C:\Program Files\Microsoft Visual Studio\2022

:Argument_Parser

:: if you update v8_tested_revision then also update build/Dockerfile
:: https://chromium.googlesource.com/v8/v8.git/+refs
set v8_tested_revision=9.8.170
set v8_revision=%v8_tested_revision%

:Default_Values
set download=false
set mode=Release
set isdebug=false
set isofficial=false
set outputDir=lib
set target_cpu=x64
set VC_TargetMode=x64

:Process_Arguments
if "%1"=="" goto ProcessArgument_Done
if "%1"=="-help" goto EchoUsage
if "%1"=="-h" goto EchoUsage
if /i "%1"=="init" goto SetDownloadTrue
if /i "%1"=="debug" goto SetDebugMode
if /i "%1"=="release" goto SetReleaseMode
if /i "%1"=="x64" goto SetTargetX64CPU
if /i "%1"=="arm64" goto SetTargetArm64CPU
goto Set_V8_Revision


:EchoUsage
echo Downloads, builds, and imports V8 for Bing CoreUX use.
echo.
echo V8UPDATE [-n] [mode] [revision]
echo.
echo   init      download the source V8 source code. If you already have it then don't pass this flag.
echo   mode      Build mode: "Debug" or "Release" (default).
echo   revision  V8 revision: "Latest", "Tested" (default) or branch/commit/tag.
echo             * Examples: "master", "8.9.4".
echo             * View history at https://github.com/v8/v8.git.
goto Exit

:SetDownloadTrue
set download=true
goto Process_Next_Argument

:SetDebugMode
set mode=Debug
set isdebug=true
set isofficial=false
goto Process_Next_Argument

:SetReleaseMode
set mode=Release
set isdebug=false
set isofficial=false
goto Process_Next_Argument

:Set_V8_Revision
set v8_revision=%1
goto Process_Next_Argument

:SetTargetX64CPU
set target_cpu=x64
set VC_TargetMode=x64
goto Process_Next_Argument

:SetTargetArm64CPU
set target_cpu=arm64
set VC_TargetMode=x64_arm64
goto Process_Next_Argument

:Process_Next_Argument
shift
goto Process_Arguments

:ProcessArgument_Done

:Initialize_VS_CMD
set stepname=Initializing Visual Studio VC++ CMD
if exist "%vs2019%\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" goto InitializeCMD2019
if exist "%vs2022%\Enterprise\Common7\Tools\VsDevCmd.bat" goto InitializeCMD2022
goto Error

:InitializeCMD2019
call "%vs2019%\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %VC_TargetMode%
set GYP_MSVS_VERSION=2019
goto Check_OS

:InitializeCMD2022

call "%vs2022%\Enterprise\Common7\Tools\VsDevCmd.bat" %VC_TargetMode%
set GYP_MSVS_VERSION=2022
goto Check_OS

 :Check_OS
if /i "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto CheckOSDone
if defined PROCESSOR_ARCHITEW6432 goto CheckOSDone
echo Error: This script requires a 64-bit operating system.
goto Exit
:CheckOSDone

:Check_VisualStudio_Version
if "%VisualStudioVersion%"=="16.0" goto CheckVisualStudio_Version_Done
if "%VisualStudioVersion%"=="17.0" goto CheckVisualStudio_Version_Done

echo Error: This script requires a Visual Studio 2017 Developer Command Prompt.
echo Browse to http://www.visualstudio.com for more information.
goto Exit
:CheckVisualStudio_Version_Done

:Resolving_Revision
if "%v8_revision%"=="" goto Use_Tested_Revision
if /i "%v8_revision%"=="latest" goto Use_Latest_Revision
goto Resolved_Revision

:Use_Latest_Revision
set v8_revision=master
goto Resolved_Revision

:Use_Tested_Revision
set v8_revision=%v8_tested_revision%
goto Resolved_Revision
:Resolved_Revision

echo V8 revision = %v8_revision%

:: ---------------------------------------
:: Download V8 source code and dependency
:: ---------------------------------------

:Download_Start

if /i %download%==false goto Setup_Env_Path

if not exist v8\ goto Create_V8_Dir
rd /s /q v8

:Create_V8_Dir
set stepname=Creating V8 directory
echo %stepname% ....
md v8
if errorlevel 1 goto Error

:Download_DepotTools
cd v8
set stepname=Downloading Depot Tools
echo %stepname% ....
powershell -Command "Invoke-WebRequest https://storage.googleapis.com/chrome-infra/depot_tools.zip -OutFile DepotTools.zip"
if errorlevel 1 goto Error
:Download_DepotTools_Done

:ExpandDepotTools
set stepname=Expanding Depot Tools
echo %stepname% ....
powershell -Command "Expand-Archive -Path DepotTools.zip -DestinationPath DepotTools"
if errorlevel 1 goto Error
cd ..
:ExpandDepotToolsDone

:Setup_Env_Path
echo setting the environment path for depot tool ....
set DEPOT_TOOLS_WIN_TOOLCHAIN=0
path %cd%\v8\DepotTools;%path%
if /i %download%==false goto Download_Completed
:Setup_Env_Path_Completed

:Client_Sync
cd v8
set stepname=Downloading V8 and dependencies
echo %stepname% ....
call gclient config https://chromium.googlesource.com/v8/v8
if errorlevel 1 goto Error
call gclient sync -r %v8_revision% >sync.log
if errorlevel 1 goto Error
:Client_Sync_Done
echo "download complete" %cd%
cd ..
:Download_Completed

:: --------------------------------
:: Building V8 with BingUX plugin
:: --------------------------------
:Build_V8

:: remove if old build exist
IF EXIST v8\v8\out (
echo "Deleting old Build"
rd /s /q v8\v8\out
) ELSE (
md v8\v8\out
)


echo Build mode: %mode%

:Build64Bit
set stepname=Building 64-bit V8
echo %stepname% %cd%...
cd v8\v8
::call "%VSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall" %VC_TargetMode%
if errorlevel 1 goto Error
call gn gen out\%target_cpu%\%mode% --args="fatal_linker_warnings=false is_clang=false is_component_build=false use_custom_libcxx=false v8_monolithic=true is_debug=%isdebug% is_official_build=%isofficial% target_cpu=\"%target_cpu%\" v8_enable_i18n_support=false v8_target_cpu=\"%target_cpu%\" v8_use_external_startup_data=false v8_win64_unwinding_info=true enable_precompiled_headers=false v8_enable_31bit_smis_on_64bit_arch=false chrome_pgo_phase=0 use_thin_lto=false enable_precompiled_headers=false v8_enable_31bit_smis_on_64bit_arch=false chrome_pgo_phase=0 use_thin_lto=false"
if errorlevel 1 goto Error

echo "Invoking v8_monolith system..."
ninja -C out\%target_cpu%\%mode% v8_monolith
if errorlevel 1 goto Error
cd ..\..
:Build64Bit_Done

::-------------------------------------
:: import v8_monolith.lib file
::-------------------------------------

:Import_Files

:EnsureLibDir
set stepname=Creating Lib Directory
if not exist %outputDir% goto Create_Lib_Dir
echo Removing old lib directory ...
rd /s /q %outputDir%\%target_cpu%
rd /s /q %outputDir%\include
rd /s /q V8.CoreUXPlugin.Private\runtimes\
:Create_Lib_Dir
echo Creating lib directory ...
md %outputDir%\%target_cpu%
if errorlevel 1 goto Error
:EnsureLibDirDone

:ImportLibs
set stepname=Importing V8 libraries
echo %stepname% ...
copy v8\v8\out\%target_cpu%\%mode%\obj\v8_monolith.lib %outputDir%\%target_cpu%\
copy v8\v8\out\%target_cpu%\%mode%\obj\*.pdb %outputDir%\%target_cpu%\
copy v8\v8\out\%target_cpu%\%mode%\obj\third_party\zlib\*.pdb %outputDir%\%target_cpu%\
copy v8\v8\out\%target_cpu%\%mode%\obj\third_party\zlib\google\*.pdb %outputDir%\%target_cpu%\
if errorlevel 1 goto Error

:ImportHeaders
set stepname=Importing V8 header files
echo %stepname% ...
xcopy /S /I /E v8\v8\include %outputDir%\include
if errorlevel 1 goto Error
:ImportHeadersDone

:ImportLibsDone

::----------------------
:: Building V8 done
::----------------------

echo ********* Succeeded ! ******************
goto Exit

:Error
echo *** THE PREVIOUS (%stepname%) STEP FAILED ***

:Exit
endlocal