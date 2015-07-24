print "Test the modules for POSIX and C standard libraries"

array = require'array'
uni = require'unistd'
io = require'cstdio'
std = require'cstdlib'

path = char.new(io.FILENAME_MAX)

uni.confstr(uni._CS_PATH, path, #path)
print "The system search path is "
io.puts(path)

uni.getlogin_r(path, #path)
print "The current user is "
io.puts(path)

uni.getcwd(path, #path)
print "The current working directory is "
io.puts(path)

home = std.getenv('HOME')
print "The user's home directory is "
io.puts(home)

print "Changing to HOME directory"
if uni.chdir(home) < 0 then
	io.perror('chdir')
else
	print "Changing back to working directory"
	if uni.chdir(path) < 0 then
		io.perror('chdir')
	end
end

if uni.isatty(uni.STDIN_FILENO) > 0 then
	uni.ttyname_r(uni.STDIN_FILENO, path, #path)
	print "Execution occuring within a terminal with name "
	io.puts(path)
else
	-- How else would you see this message?
	print "Execution not withing a terminal"
end

print "Sounding the alarm in 2 seconds"
uni.alarm(2)

print "Going to sleep for 3 seconds (unless interrupted by alarm)"
uni.sleep(3)


