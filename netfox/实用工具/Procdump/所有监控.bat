@echo off & title
 
::����ǰ׺��
set Proc=GameServer

::��ʼ���
:::Loop
cls & echo ���̼���С���
set Num=0

for /f "tokens=2" %%i in ('tasklist ^| findstr /i "%Proc%" 2^>nul') do (
	start procdump -ma -e %%i
)
