print "Test the use of C++11 threads in Lua"

require'thread'

door = mutex.new()

function say(message)
	door:lock()
	print(message)
	door:unlock()
end

hello = thread.new(say, 'Hello')
world = thread.new(say, 'World')

hello:join()
world:join()

