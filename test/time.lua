time = require'time'
t = time.time()
print('The number of seconds since the epoch is '..tostring(t))
gm = time.gmtime(t)
print('The calendar date of this is '..time.ctime(tm))

