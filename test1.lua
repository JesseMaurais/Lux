print('test1: exporting arbitrary C functions')

HelloWorld()

Repeat(1, 2, 3)

three = Add(1, 2)

print('1 + 2 = ' .. three)

print('Next we induce C++ to throw an exception')

Die("because it's time to finish")

-- we never get here
