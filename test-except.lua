print "Test of C++ exception forwarding to Lua's error handler"
require 'test'
print "The following should end with a stack trace"
Except("Catch me if you can")
