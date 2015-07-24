print "Test the use of C++11 threads in Lua"

thread = require'thread'
mutex = require'mutex'

term = mutex.new()

function say(message)
	term:lock()
	print(message)
	term:unlock()
end

hello = thread.new(say, 'Hello')
world = thread.new(say, 'World')

hello:join()
world:join()

