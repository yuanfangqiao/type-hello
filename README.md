# type-hello
--- 

### 简介
使用C语言编写的一个HTTP服务，Hello World！打字特效HTML网页。
已在Mac / Linux 系统验证。

### 操作

根据需要修改源代码
修改服务端口为默认的HTTP 80：
```shell
#define SERVER_PORT 80
```

编译
```shell
gcc -o type_hello_httpd type_hello_httpd.c
```

控制台运行
```shell
./type_hello_httpd
```

 后台运行
 ```shell
nohup ./type_hello_httpd > out.log 2>&1  &
 ```

查看日志
```shell
tail -f out.log
```

查看服务进程
```shell
ps -ef | grep type_hello
```

杀掉服务进程
```shell
kill [pid]
```
