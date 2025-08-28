使用arm-none-eabi-gcc编译工程，并且编译出lst和hex文件
main函数带简单的多task test
支持的memory：
  Flash范围0x000000000~x00080000，SRAM范围0x200000000~x20010000。
支持的外设：
  无，且不需要uart支持。
调度：
  timer为1ms一个tick。