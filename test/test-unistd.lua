print "Test the modules for POSIX and C standard libraries"

array = require'array'
uni = require'unistd'
io = require'stdio'
std = require'stdlib'

str = array.char.new(io.FILENAME_MAX)

uni.confstr(uni._CS_PATH, str, #str)
print "The system search path is "
io.puts(str)

uni.getlogin_r(str, #str)
print "The current user is "
io.puts(str)

uni.getcwd(str, #str)
print "The current working directory is "
io.puts(str)

home = std.getenv('HOME')
print "The user's home directory is "
io.puts(home)

print "Changing to HOME directory"
if uni.chdir(home) < 0 then
	io.perror('chdir')
else
	print "Changing back to working directory"
	if uni.chdir(str) < 0 then
		io.perror('chdir')
	end
end

if uni.isatty(uni.STDIN_FILENO) > 0 then
	uni.ttyname_r(uni.STDIN_FILENO, str, #str)
	print "Execution occuring within a terminal with name "
	io.puts(str)
else
	-- How else would you see this message?
	print "Execution not within a terminal"
end

print "Sounding the alarm in 2 seconds"
uni.alarm(2)

print "Going to sleep for 3 seconds (unless interrupted by alarm)"
uni.sleep(3)


