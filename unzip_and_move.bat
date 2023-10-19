@echo off
setlocal enabledelayedexpansion

:: Store the provided file path in a variable
set "sourceZip=%*"

:: Get the current directory of the batch script
for %%A in ("%~dp0.") do set "currentDir=%%~fA"

:: Create a temporary directory
set "tempDir=%temp%\tempZipExtract"

:: Check if tempDir exists, if so delete it
if exist "%tempDir%\" rmdir /s /q "%tempDir%"

:: Create tempDir
mkdir "%tempDir%"

echo %tempDir%

:: Attempt to elevate privileges
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system" || (
    goto UACPrompt
)
goto gotAdmin

:UACPrompt
powershell -Command "Start-Process -FilePath '%~f0' -ArgumentList '%sourceZip%' -Verb RunAs"
exit /B

:gotAdmin

:: Unzip the contents to the temporary directory and overwrite if necessary
powershell -command "Expand-Archive -Path '%sourceZip%' -DestinationPath '%tempDir%' -Force"

if errorlevel 1 (
    echo Error: Failed to extract the contents of the zip file.
    pause
    exit /b 1
)

:: Copy files directly from the root of tempDir
for %%F in ("%tempDir%\*.*") do (
    copy "%%F" "%currentDir%\" /Y
)

:: Copy directories directly from the root of tempDir
for /d %%E in ("%tempDir%\*") do (
    xcopy "%%E" "%currentDir%\%%~nxE\" /E /I /Y
)

:: Copy files and directories from subfolders in tempDir
for /d %%D in ("%tempDir%\*") do (
    pushd "%%D"
    
    :: Copy files from subdirectories
    for %%F in (*) do (
        copy "%%F" "%currentDir%\" /Y
    )

    :: Copy directories from subdirectories
    for /d %%E in ("*") do (
        xcopy "%%E" "%currentDir%\%%E\" /E /I /Y
    )

    popd
)





:: Copy files from the current directory
@REM xcopy "%tempDir%\*" "%currentDir%\" /I /Y

@REM :: Copy files from immediate subdirectories
@REM for /d %%D in ("%tempDir%\*") do (
@REM     xcopy "%%D\*" "%currentDir%\" /I /Y
@REM )


:: Start librum once we are done
start "" "%currentDir%\librum.exe"

:: Clean up the temporary directory (optional)
rmdir /s /q "%tempDir%"

endlocal
