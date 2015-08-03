print "Test the use of C arrays for data storage"

require 'array' -- load the array module

print 'Store x for x from 1 to 5 in array "a"'

a = int.new(5)
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

b = double.new(#a)
for x = 1, #b do
	b[x] = 1/a[x]
end
print('b = ' .. tostring(b))

print 'Strings are decoded according to the locale'

c = char.new('Hello World!\n')
char.puts(c) -- text form
print('code points = ' .. tostring(c))

