@echo off
@echo= 
@echo set file name
@echo= 
@echo= 
set /p a_name=input your file name :

:start
python char2mozzi.py %a_name%.raw %a_name%.h %a_name% 4096


