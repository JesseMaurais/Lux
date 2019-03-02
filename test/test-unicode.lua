print "Test the encoding/decoding of Unicode strings in arrays"

LC = require 'locale'
array = require 'array'

LC.setlocale(LC.ALL, "en_CA.UTF-8") -- affects encoding behaviour

print "The following is built using code points"

msg = array.int.new{9829, 32, 956, 945, 952, 951, 956, 945, 964, 953, 954, 940}
array.int.puts(msg) -- encodes the array as a UTF-8 string
print '\n'

print "These characters sorted are..."
array.int.sort(msg) -- using the quick sort algorithm
array.int.puts(msg)
print '\n'

print "Now enter a string and I'll tell you the code points"

io.flush()
str = io.read()
msg = array.int.decode(str)
print "The code points are..."
print(tostring(msg))

