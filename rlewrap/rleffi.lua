--[[ Copyright 2014 Google Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
]]

require 'paths'

-- Reads the whole content of the specified file.
local function readContent(path)
    local file = io.open(path)
    local content = file:read("*a")
    file:close()
    return content
end

-- Appends all srcList values to the destList.
local function appendAll(destList, srcList)
    for _, val in ipairs(srcList) do
        table.insert(destList, val)
    end
end


local ffi = require 'ffi'
-- We let the rle::RLEInterface to look like a C struct.
ffi.cdef("typedef struct RLEInterface RLEInterface;")
ffi.cdef(readContent(paths.thisfile("rlewrap.inl")))
local lib = ffi.load(package.searchpath('librlewrap',package.cpath))

-- Defining the metatable for RLEInterface userdata.
local mt = {}
mt.__index = mt
mt.loadRom = lib.rle_loadRom
mt.act = lib.rle_act
mt.getScreenWidth = lib.rle_getScreenWidth
mt.getScreenHeight = lib.rle_getScreenHeight
mt.fillObs = lib.rle_fillObs
mt.fillObsGray = lib.rle_fillObsGray
mt.fillRamObs = lib.rle_fillRamObs
mt.isGameOver = lib.rle_isGameOver
mt.resetGame = lib.rle_resetGame
mt.loadState = lib.rle_loadState
mt.saveState = lib.rle_saveState
mt.numActions = lib.rle_numLegalActions
mt.actions = lib.rle_legalActions
mt.lives = lib.rle_livesRemained

mt.getString = lib.rle_getString
mt.getInt = lib.rle_getInt
mt.getBool = lib.rle_getBool
mt.getFloat = lib.rle_getFloat
mt.setString = lib.rle_setString
mt.setInt = lib.rle_setInt
mt.setBool = lib.rle_setBool
mt.setFloat = lib.rle_setFloat

--mt.restoreSnapshot = function(self, snapshot)
--    lib.rle_restoreSnapshot(self, snapshot, #snapshot)
--end

--mt.saveSnapshot = function(self)
--    local size = lib.rle_getSnapshotLength(self)
--    local buf = ffi.new("char[?]", size)
--    lib.rle_saveSnapshot(self, buf, size)
--    return ffi.string(buf, size)
--end


ffi.metatype("RLEInterface", mt)

-- Creates a new RLEInterface instance.
function rlewrap.newAle()
    a= ffi.gc(lib.rle_new(), lib.rle_gc)
    return a
end

-- Creates a new RLEInterface instance.
--function rlewrap.newAle(romPath, corePath)
--    if not paths.filep(romPath) then
--        error(string.format('no such ROM file: %q', romPath))
--    end
--    if not paths.filep(corePath) then
--        error(string.format('no such core file: %q', corePath))
--    end
--    a= ffi.gc(lib.rle_new(romPath, corePath), lib.rle_gc)
--    return a
--end

-- Converts the prlette values to RGB values.
-- A new ByteTensor is returned.
--function rlewrap.getRgbFromPrlette(obs)
--    obs = obs:contiguous()
--    assert(obs:nElement() == obs:storage():size(),
--        "the obs should not share a bigger storage")
--    local rgbShape = {3}
--    appendAll(rgbShape, obs:size():totable())
--
--    local rgb = torch.ByteTensor(unpack(rgbShape))
--    lib.rle_fillRgbFromPrlette(torch.data(rgb), torch.data(obs),
--            rgb:nElement(), obs:nElement())
--    return rgb
--end

