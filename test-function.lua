print "Test the binding of C functions and argument order/conversion"

require 'test' -- load the "test" module

print "Result of 'WriteOut(0, 1, 2, 3, 4)'"
WriteOut(0, 1, 2, 3, 4)
print "Result of 'WriteOut(9, 8, 7, 6, 5)'"
WriteOut(9, 8, 7, 6, 5)
print "Result of 'WriteOut(nil, \"1\", 2, {3}, 4)'"
WriteOut(nil, "1", 2, {3}, 4)

