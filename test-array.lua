print "Test the use of C arrays for data storage"

require 'array' -- load the array module

a = int.new(10)
for i = 0, #a-1 do
	a[i] = 2^i
end
print('a = ' .. tostring(a))

b = a + 5
assert(#b == 0)
print('b = a + 5')
for i = 0, 4 do
	print('b['..i..'] = '..b[i])
end

c = double.new(10)
for i = 0, #c-1 do
	c[i] = 1/a[i]
end
print('c = ' .. tostring(c))

d = short.new{7, 10, 6, 14}
print('d = ' .. tostring(d))

