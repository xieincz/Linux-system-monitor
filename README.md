# Linux-system-monitor
Monitor the cpu usage, cpu temperature, and memory usage of the Linux system in real time, and save the information to a file by date and time. 实时监控Linux系统的cpu使用率、cpu温度、内存使用率，并且将这些信息按日期和时间保存到文件中
## Usage 使用方法
Open `monitor.cpp`, and modify `"/home/ubuntu/monitor/logs"` on line 11 to the path of the folder you created yourself.

打开`monitor.cpp`，将第11行的`"/home/ubuntu/monitor/logs"`修改为你自己创建的文件夹的路径。

Then compile with the following command.

然后用以下命令进行编译。
```bash
g++ monitor.cpp -o monitor -O3
```
Then run the program in the background with the following command.

然后用以下命令将程序放到后台运行。
```bash
nohup monitor > monitor.log 2>&1 &
```
Note: To reduce I/O time, this program saves logs every half hour. If you need to modify it, you can modify the value of `chunckSize` in line 10 of `monitor.cpp`, `1800` means save every 1800 seconds. This program has been tested on Ubuntu Server 22.04 LTS arm64.

注：为了减少I/O时间，本程序每半小时保存一次日志。如果需要调整，可以修改`monitor.cpp`第10行的`chunckSize`的值，`1800`表示每隔1800秒保存一次。本程序在 Ubuntu Server 22.04 LTS arm64 上通过了测试。
