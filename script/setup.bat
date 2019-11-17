@echo off
setlocal enableextensions
setlocal enabledelayedexpansion
:dowhile
    set /p ref=Help? [y/n]
    if not "!ref!" == "y" (
        if not "!ref!" == "n" (
            goto dowhile
        )
    )
if "!ref!" == "y" (
    cls
    call man
)
cls
set ip=""
for /f "tokens=1-2 delims=:" %%a in ('ipconfig^|find "IPv4"') do (
    set ip=%%b
)
if %ip% == "" (
    echo You are not connected to a local net or to the Internet 
    echo You can run "run.bat" and type "@tcpserv 127.0.0.1 23"
    echo and talk with yourself. :D
) else ( 
    set ip=%ip:~1%
    set /p port="Enter port of tcpserv: "
    :dowhile2 
        set /p notif=Sound notofications? [y/n]
        if not "!notif!" == "y" (
            if not "!notif!" == "n" (
                goto dowhile2
            )
        )
    call run
    call tcpserv.exe "!ip!" "!port!" "!notif!"|| echo An error occured. You can know more in log.
)
endlocal
pause
exit /b
