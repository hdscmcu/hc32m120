﻿================================================================================
                                样例使用说明
================================================================================
版本历史
日期        版本    负责人         IAR     MDK     GCC     描述
2019-06-06  1.0     Wuze           7.70    5.16    8.3.1   first version
================================================================================
平台说明
================================================================================
GCC工程，由Eclipse IDE外挂GNU-ARM Toolchain，再结合pyOCD GDB Server实现工程的编译、
链接和调试。在用Eclipse导入工程后，请将xxxx_PyOCDDebug中pyocd-gdbserver和SVD文件
设置为正确的路径；请将xxxx_PyOCDDownload中pyocd设置为正确的路径。注意，这些路径不
能包含非英文字符。


功能描述
================================================================================
本例程实现了SPI从机模式的配置和用法。


================================================================================
测试环境
================================================================================
测试用板:
---------------------
STK_HC32M120_LQFP48_050_V11

辅助工具:
---------------------
无

辅助软件:
---------------------
无

================================================================================
使用步骤
================================================================================
1）打开工程并重新编译；
2）启动IDE的调试功能，断点查看程序执行情况；


================================================================================
注意
================================================================================
1、从机模式下，SCK最高频率为PCLK/6；
2、本例程SPI工作模式为全双工模式，使能接收中断。


================================================================================
