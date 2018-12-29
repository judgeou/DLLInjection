# DLL远程注入等其他工具

## 项目说明

* **ShellCode**: 用于将DLL转换成可执行字节的工具，是静态库
* **RemoteEipAttach**: 将本地ShellCode注入远程进程，使用修改Eip寄存器的方法，利用现有线程执行代码，无需创建新的线程，因此，请勿在dllmain中编写阻塞代码。
* **testDLL**: 仅仅是示例用的DLL，会弹出一个信息框