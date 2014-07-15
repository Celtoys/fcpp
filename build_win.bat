
@echo off

setlocal

::
:: Search for the location of Visual Studio
::
if DEFINED VS110COMNTOOLS (
	set "VS_TOOLS_DIR=%VS110COMNTOOLS%"
) else if DEFINED VS100COMNTOOLS (
	set "VS_TOOLS_DIR=%VS100COMNTOOLS%"
) else if DEFINED VS90COMNTOOLS (
	set "VS_TOOLS_DIR=%VS90COMNTOOLS%"
) else if DEFINED VS80COMNTOOLS (
	set "VS_TOOLS_DIR=%VS80COMNTOOLS%"
) else (
	echo Microsoft Visual Studio not found
	exit
)


::
:: Apply environment necessary to use cl.exe
::
set VC_DIR=%VS_TOOLS_DIR%..\..\VC
call "%VC_DIR%\vcvarsall.bat"


::
:: Search for the windows SDK
::
set KEY_NAME="HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows"
set VALUE_NAME=CurrentInstallFolder
for /F "usebackq skip=2 tokens=1,2*" %%A in (`REG QUERY %KEY_NAME% /v %VALUE_NAME% 2^>nul`) do (
	set "ValueName=%%A"
	set "ValueType=%%B"
	set WINDOWS_SDK_DIR=%%C
)
if not defined WINDOWS_SDK_DIR (
	echo %KEY_NAME%\%VALUE_NAME% not found.
	exit
)


::
:: Quick and dirty compile of the single file
::
cl.exe src/fcpp.c /EHsc /nologo /Fobin/fcpp.obj /c
cl.exe src/example.c /EHsc /nologo /Fobin/example.obj /c
link.exe /LIBPATH:"%WINDOWS_SDK_DIR%lib" /OUT:bin/example.exe bin/fcpp bin/example
