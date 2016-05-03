print "Test the use of C arrays for data storage"

array = require'array' -- load the array module

print 'Store x for x from 1 to 5 in array "a"'

a = array.int.new(5)
for x = 1, #a do a[x] = x end
print('a = ' .. tostring(a))

print 'We can do arithmetic adjustments on the array bounds'

print('a + 2 = ' .. tostring(a + 2))
print('a - 2 = ' .. tostring(a - 2))

print 'We can rotate the elements of an array as well'

print('a << 3 = ' .. tostring(a << 3))
print('a >> 3 = ' .. tostring(a >> 3))

collectgarbage() -- clean away the stray pointers we made

print 'Floating point precision types can store 1/x'

b = array.double.new(#a)
for x = 1, #b do
	b[x] = 1/a[x]
end
print('b = ' .. tostring(b))

print 'Let\'s do some UTF-8 string encoding'

-- This only works for supported locales
LC = require'locale'
LC.setlocale(LC.CTYPE, "en_CA.UTF-8")
-- These are Unicode code points in hexadecimal form (must be null terminated)
s = array.int.new{0x3BC,0x3B1,0x3B8,0x3B7,0x3BC,0x3B1,0x3C4,0x3B9,0x3BA,0x3AC,0}
-- It's up to your terminal to display this properly
print(array.int.encode(s))

