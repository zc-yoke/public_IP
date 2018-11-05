Introduction
=======
## Describe
* This is a program to get the public ip address,it depends on libcurl.so you should compile the libcurl before compile this program

* this  program used the multithreaded technique #include<thread> which is support c++11

* this program get the ip address every 5 seconds,if the  children threaded cannot get the ip address,will get a empty buffers

* this program will store the ip address and compare the result between the old ip and the new ip.if the ip has changed ,the event whill be written to the log file


## Test on platform
This program test on bellow platform

* ** window 7 64bit **

* **centos 7.5**
* **ubuntu 18.04**

Attention:on linux you should modfiy the head file who prive the sleep() function.

On linux you can use #include <unistd.h>,just comment the "#include <windows.h>"

On window you can use #include <windows.h>,just commet the "#include <unistd.h>"

## Compile sample
g++ maim.cpp -std=c++11 -o getip -I /"path to the licurl header files" -L /"path to the libcurl.a" -lcurl
