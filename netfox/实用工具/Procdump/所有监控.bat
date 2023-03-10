@echo off & title
 
::进程前缀名
set Proc=GameServer

::开始监测
:::Loop
cls & echo 进程监测中。。
set Num=0

for /f "tokens=2" %%i in ('tasklist ^| findstr /i "%Proc%" 2^>nul') do (
	start procdump -ma -e %%i
)
