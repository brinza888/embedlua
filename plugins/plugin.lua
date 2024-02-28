--[[
    This simple plugin is an example for my plugin system,
    based on C and Lua with Lua C API

    Author: Ilya Bezrukov (brinza888)
    18.10.2023, 04.11.2023
--]]

api.setname("FirstPlugin")
api.setversion("0.0.2")
api.setauthor("brinza888")
api.setalias("fp")

function hello(command)
    print("Hello, user!")
end

function bye(command)
    print("Bye, user!")
end

api.regcmd("hello", "Says greeting to user", hello)
api.regcmd("bye", "Says goodbye to user", bye)
