print "Test the encoding/decoding of Unicode strings in arrays"

LC = require 'clocale'
require 'array'

LC.setlocale(LC.ALL, "en_CA.UTF-8") -- affects encoding behaviour

print "The following is built using code points"

msg = int.new{9829, 32, 956, 945, 952, 951, 956, 945, 964, 953, 954, 940}
int.puts(msg) -- encodes the array as a UTF-8 string
print '\n'

print "These characters sorted are..."
int.sort(msg) -- using the quick sort algorithm
int.puts(msg)
print '\n'

print "Now enter a string and I'll tell you the code points"

io.flush()
msg = int.new(io.read())
print "The code points are..."
print(tostring(msg))

