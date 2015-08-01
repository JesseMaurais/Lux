print "Test the encoding/decoding of Unicode strings in arrays"

LC = require 'clocale'
require 'array'

LC.setlocale(LC.ALL, "en_CA.UTF-8") -- affects encoding behaviour

print "The following is built using code points"

msg = int.new{9829, 32, 956, 945, 952, 951, 956, 945, 964, 953, 954, 940, 10}
int.fputs(msg, io.stdout) -- encodes the array as a UTF-8 string

print "Now enter a string and I'll tell you the code points"

io.flush()
msg = int.new(io.read())
print "The code points are..."
print(tostring(msg))

