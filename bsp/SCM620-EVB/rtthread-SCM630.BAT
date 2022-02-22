SET PATH=D:\software\keil5\ARM\ARMCC\Bin;C:\Program Files\VanDyke Software\Clients\;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\windows\System32\OpenSSH\;D:\software\labview\VISA\WinNT\Bin\;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;D:\software\labview\VISA\WinNT\Bin;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Users\limeng5\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=ARMCM4
SET CPU_VENDOR=ARM
SET UV2_TARGET=rtthread-SCM630
SET CPU_CLOCK=0x00B71B00
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\clock.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\components.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\cpu.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\device.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\idle.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\ipc.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmLink" --Via ".\build\rtthread.lnp"
