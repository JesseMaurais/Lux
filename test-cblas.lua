print "Test unit for the CBLAS module"

require'array'
require'complex'
cblas = require'cblas'

i = complex.i
x = complex.new{i * 1 + 2, i * 3 + 4}
y = complex.new{i * 5 + 6, i * 7 + 8}
z = cblas.cdotu(2, x, 1, y, 1)

print('('..tostring(x[1])..')('..tostring(y[1])..') + ('..tostring(x[2])..')('..tostring(y[2])..') == '..tostring(z))

