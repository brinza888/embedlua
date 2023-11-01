--[[
    This simple plugin is an example for my plugin system,
    based on C and Lua with Lua C API

    Author: Ilya Bezrukov (brinza888)
    18.10.2023
--]]

pl_name = "FirstPlugin"
pl_version = "0.0.2"
pl_author = "brinza888"
pl_alias = "fp"


pl_regcmd("hello", "Says greeting to user")
pl_regcmd("bye", "Says goodbye to user")


function pl_command(name, args)
    if name == "hello" then
        print("Hello, user!")
        return true
    elseif name == "bye" then
        print("Bye, user!")
        return true
    else
        return false
    end
    
end
