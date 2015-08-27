print "Test the use of C++11 threads in Lua"

thread = require'thread'


function square(arg)
	x = arg:get()
	print(x*x)
end

promise = thread.promise.new()
future = promise:get()
aux = thread.new(square, future)
print "Enter some number and its square is calculated in the thread"
s = io.read()
x = tonumber(s)
promise:set(x)
aux:join()


function concat(a, b, c, d)
	return 'Message: ' .. a .. b .. c .. d
end

future = thread.future.new(concat, 'Hello', ' ', 'World', '!')
print "The thread sends a message:"
message = future:get()
print(message)

