print "Test the use of POSIX threads in Lua"

pthread = require'pthread'

function concat(a, b, c, d)
	return 'Concatenated: ' .. a .. b .. c .. d
end

id = pthread.self()
print('Main thread ', id)
id = pthread.create(concat, 'Hello', ' ', 'World', '!')
print('Work thread ', id)
string = pthread.join(id)
print(string)


