--[[--
	An early attempt to create a uniform API for linear algebra
--]]--

require'array'
require'complex'
require'numeric.blas'

function matrix(array, rows, cols, stride)
	-- Create a BLAS/LAPACK matrix from an array
	if type(array) == 'table' then
		-- Assume it's an array's metatable
		if rows and cols then
			array = array.new(rows*cols)
		else
			error('must provide both rows and columns')
		end
	else
		-- Assume it's an actual array
		size = #array
		if cols == nil then
			if rows then
				cols = size/rows
			end
		end
	end
end

