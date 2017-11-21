    :RESTART  
    tasklist /FI "username eq administrator" | find /C "libeventTest" > temp.txt  
    set /p num=<temp.txt  
    del /F temp.txt  
      
    if %num%==0 start /D "D:\libeventTest\Debug" libeventTest  
      
    cscript //nologo sleep.vbs  
    goto RESTART  