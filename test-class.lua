print "Test the exporting of C++ class/struct/union to Lua"

require 'test' -- load the "test" module

a = Account.new()

print "Making my first deposit of $4"
a:Deposit(4)

print "How much is in there?"
print("$" .. a:Balance())

print "I got paid, deposit $50"
a:Deposit(50)

print "I want a burger for lunch, costs $10.50"
a:Withdrawl(10.50)

print "Whats left?"
print("$" .. a:Balance())

print "I'm done at this bank!"

