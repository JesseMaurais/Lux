print "Test the use of C arrays for data storage"

require 'array' -- load the array module

a = int.new(10)

print('object a is ' .. tostring(a))

for i = 0, #a-1 do
	a[i] = 2^i
end

for i = 0, #a-1 do
	print('a['..i..'] = '..a[i])
end


b = a + 5

assert(#b == 0)
print('b = a + 5 is '..tostring(b))

for i = 0, 4 do
	print('b['..i..'] = '..b[i])
end


c = double.new(10)

for i = 0, #c-1 do
	c[i] = 1/a[i]
end

print('object c is '..tostring(c))

for i = 0, #c-1 do
	print('c['..i..'] = '..c[i])
end


d = char.new('Hello World!')

print('object d is '..tostring(d))

for i = 0, #d-1 do
	print('d['..i..'] = '..string.char(d[i]))
end

e = short.new{7, 10, 6, 14}

print('object e is '..tostring(e))

for i = 0, #e-1 do
	print('e['..i..'] = '..e[i])
end

