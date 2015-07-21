print "Test the Lua interface to C++11 random number generators"

random = require 'random'
r = random.device.new()

n = 10
p = 0.5
size = 100
data = r:binomial(size, n, p)
hist = {}

for item = 0, n do
	hist[item] = 0
end

for key, value in pairs(data) do
	hist[value] = hist[value] + 1
end

for item = 0, n do
	print(item, string.rep('*', hist[item]))
end

