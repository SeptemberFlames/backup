@echo off 
:loop
	::完整进程名称
	procdump -ma -e GameServer.exe
	::睡眠制定三秒
	ping -n 1 127.0.0.1>nul
	::继续循环处理
goto loop