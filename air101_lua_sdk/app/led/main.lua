-- LuaTools需要PROJECT和VERSION这两个信息
PROJECT = "helloworld"
VERSION = "1.0.0"
log.info("main", PROJECT, VERSION)
-- 引入必要的库文件(lua编写), 内部库不需要require
-- sys库是标配
_G.sys = require("sys")
local LED1 =gpio.setup(24, 0, gpio.PULLUP) -- PB8输出模式
sys.taskInit(function()
    while 1 do
        log.info("LED开启!!!")
        LED1(0)
        sys.wait(1000)
        log.info("LED关闭!!!")
        LED1(1)
        sys.wait(1000)
    end
end)
 
-- 用户代码已结束---------------------------------------------
-- 结尾总是这一句
sys.run()
-- sys.run()之后后面不要加任何语句!!!!!
