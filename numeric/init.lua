--[[--
	An early attempt to create a uniform API for linear algebra
--]]--

require'array'
require'complex'
require'numeric.blas'

local Module = {}
local Vector = {}
local Matrix = {}

function Vector.nrm2(this)
	return getmetatable(this.array).nrm2(this.dim, this.array, this.inc)
end

function Module.vector(array, dim, inc) 
	local table = {}
	-- Create a BLAS/LAPCK vector from an array
	if type(array) == 'table' then
		-- Assume it's an array's metatable
		table.array = array.new(dim)
		table.dim = dim
		table.inc = 1
	else
		-- Assume it's an actual array
	end 
	-- 
	return setmetatable(table, Vector)
end

function Module.matrix(array, rows, cols, stride)
	-- Create a BLAS/LAPACK matrix from an array
	if type(array) == 'table' then
		-- Assume it's an array's metatable
	else
		-- Assume it's an actual array
	end
end

return Module

