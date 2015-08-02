print "Test the use of C arrays for data storage"

require 'array' -- load the array module

-- Calculate an exponential sequence

a = int.new(10)
for i = 0, #a-1 do
	a[i] = 2^i
end
print('a = ' .. tostring(a))

-- Pointer arithmetic is supported on arrays

b = a + 5
assert(#b == 0)
print('b = a + 5')
for i = 0, 4 do
	print('b['..i..'] = '..b[i])
end

-- Floating point precision types too

c = double.new(10)
for i = 0, #c-1 do
	c[i] = 1/a[i]
end
print('c = ' .. tostring(c))

-- Constructor by table values

d = short.new{7, 10, 6, 14}
print('d = ' .. tostring(d))

-- Strings treated as UTF-8 encoded

e = char.new('Hello World!\n')
print(tostring(e)) -- numeric form
char.puts(e) -- text form

--[[ Arrays can be also be written as binary chunks
f = long.new{9829, 32, 956, 945, 952, 951, 956, 945, 964, 953, 954, 940, 10}
out = io.open("test.bin", "wb")
long.fwrite(f, out)
--]]

