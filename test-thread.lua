print "Test the use of C++11 threads in Lua"

require'thread'

--[[
function sqr(arg)
	x = arg:get()
	print(x*x)
end

prom = promise.new()
arg = prom:get()
th = thread.new(sqr, arg)
print "Enter some number and its square is calculated in the thread"
str = io.read()
x = tonumber(str)
prom:set(x)
]]--

function cat(a, b, c, d)
	return 'Message: ' .. a .. b .. c .. d
end

fut = future.new(cat, 'Hello', ' ', 'World', '!')
print "Press any key to see the threads message"
io.read()
message = fut:get()
print(message)
later = nil

