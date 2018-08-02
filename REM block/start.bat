@echo off

echo [INFO] Promjena direktorija
set root=C:\Users\Lovro\Anaconda3
call %root%\Scripts\activate.bat %root%

echo [INFO] Pokretanje virtualnog okruzenja za Python 3.6
call conda activate env_dlib

echo [INFO] Pokretanje glavnog programa
call python main.py

pause