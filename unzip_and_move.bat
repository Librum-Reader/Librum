@echo off
setlocal enabledelayedexpansion

:: Store the provided file path in a variable
set "sourceZip=%*"

:: Get the current directory of the batch script
for %%A in ("%~dp0.") do set "currentDir=%%~fA"

:: Create a temporary directory
set "tempDir=%temp%\tempZipExtract"
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

:: Copy all immediate files from subfolders in the temporary directory to the current directory
for /d %%D in ("%tempDir%\*") do (
    pushd "%%D"
    for %%F in (*) do (
        copy "%%F" "%currentDir%\" /Y
    )
    popd
)


:: Copy files from the current directory
xcopy "%tempDir%\*" "%currentDir%\" /I /Y

:: Copy files from immediate subdirectories
for /d %%D in ("%tempDir%\*") do (
    xcopy "%%D\*" "%currentDir%\" /I /Y
)


:: Start librum once we are done
start "" "%currentDir%\librum.exe"

:: Clean up the temporary directory
rmdir /s /q "%tempDir%"

:: Delete the zip file
del /f /q "%sourceZip%"

endlocal