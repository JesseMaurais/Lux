time = require'time'
-- time returns an unsigned integer type
t = time.time()
print('The number of seconds since the epoch is ' .. tostring(t))
-- localtime returns a tm structure for your timezone
tm = time.localtime(t)
print('The calendar date in your timezone is ' .. time.asctime(tm))
-- gmtime also returns a tm structure as a Universal Time Coordinate
gm = time.gmtime(t)
print('The calendar epressed in GMT is ' .. time.asctime(gm))
-- ctime(t) is equivalent to asctime(localtime(t))

t = time.time() -- new timestamp
-- we can recover the old one using mktime and tm
t0 = time.mktime(tm)
-- the elapsed time can be found (like t - t0)
dt = time.difftime(t0, t)
print('Duration in seconds up to now is ' .. tostring(dt))

