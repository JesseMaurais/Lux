print "Test unit for the CBLAS module"

require'array'
cblas = require'cblas'

x = double.new{1, 2, 3, 4}
y = double.new{5, 6, 7, 8}
z = double.new(2)
cblas.zdotu(2, x, 1, y, 1, z)
print('(1 + 2i)(3 + 4i) + (5 + 6i)(7 + 8i) == '.. z[1]..' + '..z[2]..'i')

