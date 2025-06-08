@echo off
javac -cp ".;gs-core-1.3.jar;gs-ui-1.3.jar" TourPlanner.java
if errorlevel 1 (
    echo Compilation failed.
    pause
    exit /b 1
)
java -cp ".;gs-core-1.3.jar;gs-ui-1.3.jar" TourPlanner
pause
