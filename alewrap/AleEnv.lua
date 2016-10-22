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


function alewrap.createEnv(romName, coreName, extraConfig)
    return alewrap.AleEnv(romName,  coreName, extraConfig)
end

local RAM_LENGTH = 128

-- Copies values from src to dst.
local function update(dst, src)
    for k, v in pairs(src) do
        dst[k] = v
    end
end

-- Copies the config. An error is raised on unknown params.
local function updateDefaults(dst, src)
    for k, v in pairs(src) do
        if dst[k] == nil then
            _print_usage(dst)
            error("unsupported param: " .. k)
        end
    end
    update(dst, src)
end

local Env = torch.class('alewrap.AleEnv')
function Env:__init(romPath, corePath, extraConfig)
    self.config = {
        -- An additional reward signal can be provided
        -- after the end of one game.
        -- Note that many games don't change the score
        -- when loosing or gaining a life.
        gameOverReward=0,
        -- Screen display can be enabled.
        display=false,
--        display=true,
        -- The RAM can be returned as an additional .observation.
        enableRamObs=false,
        -- Certain games can be played using two agents
        twoPlayers=false
    }
    updateDefaults(self.config, extraConfig)
    
    self.win = nil
    if self.config.enableRamObs then
        obsShapes={{height, width}, {RAM_LENGTH}}
    end
    self.ale = alewrap.newAle()
    -- below configuration must be set before rom load
    self.ale:setBool("two_players", self.config.twoPlayers)
    self.ale:loadRom(romPath, corePath)
    
    self.envSpec = {
        --nActions=18,
        nActions=self.ale:numActions(),
        obsShapes=obsShapes,
    }
    local width = self.ale:getScreenWidth()
    local height = self.ale:getScreenHeight()
    local obsShapes = {{height, width}}
end

-- Returns a description of the observation shapes
-- and of the possible actions.
function Env:getEnvSpec()
    return self.envSpec
end

-- Returns a list of observations.
-- The integer palette values are returned as the observation.
function Env:envStart()
--  print ("in Ale Env start")
    self.ale:resetGame()
    return self:_generateObservations()
end

-- Does the specified actions and returns the (reward, observations) pair.
-- Valid actions:
--     {torch.Tensor(zeroBasedAction)}
-- The action number should be an integer from 0 to numActions
function Env:envStep(actionsA, actionsB)
    if actionsB then
      assert(#actionsB == 1, "one action is expected")
      assert(actionsB[1]:nElement() == 1, "one discrete action is expected")
    else
--      actionsB = torch.Tensor(1,self.ale:numActions()):zero()
      actionsB = torch.Tensor(1,self.ale:numActions()):zero()
    end
    
    assert(#actionsA == 1, "one action is expected")
    assert(actionsA[1]:nElement() == 1, "one discrete action is expected")

    if self.ale:isGameOver() then
        self.ale:resetGame()
        -- The first screen of the game will be also
        -- provided as the observation.
        return self.config.gameOverReward, self:_generateObservations()
    end

    local reward = self.ale:act(actionsA[1][1], actionsB[1][1])
    return reward, self:_generateObservations()
end

--function Env:getRgbFromPalette(obs)
--    return alewrap.getRgbFromPalette(obs)
--end

function Env:_createObs()
--  print("in create obs")
    -- The torch.data() function is provided by torchffi.
    local width = self.ale:getScreenWidth() --in bytes
    local height = self.ale:getScreenHeight() 
--    local obs = torch.ByteTensor(height, width)
      local obs = torch.ByteTensor(3, height, width)
    
--      print ("1 : In AleEnv createObs, Params: " .. " Width: " ..width .. " Height: " .. height .. "nElements : " ..   obs:nElement())  
--      self.ale:fillObsGray(torch.data(obs), obs:nElement())
      self.ale:fillObs(torch.data(obs), obs:nElement())
--      print ("2: In AleEnv createObs, Params: " .. " Width: " ..width .. " Height: " .. height .. "nElements : " ..   obs:nElement())  
    
--    print ("Finished CreateObs")
 
    return obs
end

function Env:_createRamObs()
--    print("in Ram Obs:" )
    local ram = torch.ByteTensor(RAM_LENGTH)
    self.ale:fillRamObs(torch.data(ram), ram:nElement())
    return ram
end

-- TODO : make sure that the input array is in the correct format
function Env:_display(obs)
    require 'image'
--    print ("in display")
--    local frame = self:getRgbFromPalette(obs)
    self.win = image.display({image=obs, win=self.win})
end

-- Generates the observations for the current step.
function Env:_generateObservations()
--  print("in AleEnv generateObservation")
  
    local obs = self:_createObs()
--    print ("after create obs config_display= " .. tostring(self.config.display))
  
    if self.config.display then
--      print ("in config display :" .. tostring(self.config.display)  )      
        self:_display(obs)
    end
--      print("in AleEnv generateObservation - after create")
    if self.config.enableRamObs then
        local ram = self:_createRamObs()
        return {obs, ram}
    else
        return {obs}
    end
end

function Env:saveState()
    self.ale:saveState()
end

function Env:loadState()
    return self.ale:loadState()
end

function Env:actions()
    local nactions = self.ale:numActions()
    local actions = torch.IntTensor(nactions)
    self.ale:actions(torch.data(actions), actions:nElement())
    return actions
end

function Env:lives()
    return self.ale:lives()
end

--function Env:saveSnapshot()
--    return self.ale:saveSnapshot()
--end
--
--function Env:restoreSnapshot(snapshot)
--    self.ale:restoreSnapshot(snapshot)
--end

function Env:getScreenWidth()
  return self.ale:getScreenWidth()
end

function Env:getScreenHeight()
  return self.ale:getScreenHeight()
end


-- Get the value of a setting.
function Env:getString(key)
  return self.ale:getString(key)
end

function Env:getInt(key)
  return self.ale:getInt(key)
end

function Env:getInt(key)
  return self.ale:getInt(key)
end

function Env:getBool(key)
  return self.ale:getBool(key)
end

function Env:getFloat(key)
  return self.ale:getFloat(key)
end

-- Set the value of a setting. loadRom() must be called before the
-- setting will take effect.
function Env:setString(key, value)
  self.ale:setString(key, value);
end

function Env:setInt(key, value)
  self.ale:setInt(key, value);
end

function Env:setBool(key, value)
  self.ale:setBool(key, value);
end

function Env:setFloat(key, value)
  self.ale:setFloat(key, value);
end
