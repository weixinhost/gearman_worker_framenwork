###gearman_worker_framewrok

####intro

    使用c语言更简单的创建一个gearman worker端
    
    
####Install

    cd gearman_worker_framework
    cmake .
    make 
    
    ./bin/gearman_worker_test -c ./bin/test.ini
    
    
####Files

    | - bin    (编译后的执行文件)
    | - framework (框架文件)
    | - lib       (第三方库)    
    | - src       （源码）    
    | - php_test   (php 测试脚本)
    | - CmakeLists.txt 


#### Developer Guide

    1. 配置文件使用ini格式

    2. src/main.c 中提供了两个示例worker.一个名为echo,一个名为get_config.

    3. 对于worker的job data需要打包解包请自行添加库实现。 job data是二进制安全的.
        