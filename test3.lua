print('test3: access C structs & unions')

a = Record()
print('a has type ' .. tostring(a))
a.word = 1
if a.byte == 1 then
	print('little endian machine')
else
	print('big endian machine')
end
a.real = 3.14
print('pi is approximately ' .. a.real)

b = Record{ word = 2, real = 3.2 }
print('pi is certainly not '.. b.real)

c = Record(4)
print('c = Record(4) has size #c = ' .. #c)

print('This should fail')
c.not_a_key = "Holy Cow!"
