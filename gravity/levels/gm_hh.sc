-- LIB_HH: Game mode "Hunter -- Hunted"
-- include with: dofile("./levels/libhh.sc")
-- (07.09.01, 07.02.02/jh)

if (__LIBHH__ == nil) then

__LIBHH__ = 1


-- ====================================================================
-- Needed globals
-- ====================================================================

__hh_player = {}

for a = 1, globals.players do
  __hh_player[a] = {}
  __hh_player[a].time = globals.max_time
end

if (globals.splitscreen == 0 and globals.network_game == 0) then
  __hh_split = 1
elseif (globals.network_game == 0) then
  __hh_split = 2
else
  __hh_split = 3
end

__hh_player[1].osd  = playmap[1].osd:get_osdbit(1101)   -- get time osd bit
if (globals.splitscreen == 0 and globals.network_game == 0) then
  __hh_player[2].osd  = playmap[1].osd:add(1103, 589, 10, 41, 15, OSDBIT_SCREEN)
  __hh_player[2].osd:clear()
  __hh_player[2].osd:draw_rectangle()
elseif (globals.network_game == 0) then
  __hh_player[2].osd  = playmap[2].osd:get_osdbit(1102)
end

-- create more OSDs on the right side - one for each enemy player in netgame
if (globals.network_game == 1) then
  local n
  for n = 2, globals.players do
    __hh_player[n].osd = playmap[1].osd:add(1103+(n-2), 589, 10+(n-2)*20, 41, 15, OSDBIT_SCREEN)
    __hh_player[n].osd:clear();
    __hh_player[n].osd:draw_rectangle()
    
    -- draw a label with player's name
    local label = playmap[1].osd:add_label(1110+(n-2), 400, 10+(n-2)*20, OSDBIT_SCREEN, FONT_IMPACT10, player[n]:get_name(), globals.col_yellow, 0)
    label:set_x(580 - label:get_width())
  end
end


__hh_last_time = -1
__hh_hunter = 0
__hh_hunted = 0
__hh_temp_globals_max_time = globals.max_time
globals.max_time = 0


if (globals.language == LANG_DE) then
  __hh_msg_hunter      = "JAEGER"
  __hh_msg_hunter_desc = "Flieg los und jage %s!"
  __hh_msg_hunted      = "GEJAGTER"
  if (globals.players > 2) then
    __hh_msg_hunted_desc= "Bleib am Leben - die anderen sind dir auf den Fersen..."
  else
    __hh_msg_hunted_desc = "Bleib am Leben - %s ist dir auf den Fersen..." 
  end  
else
  __hh_msg_hunter      = "HUNTER"
  __hh_msg_hunter_desc = "Your time runs out - go on and kill %s!"
  __hh_msg_hunted      = "HUNTED"
  if (globals.players > 2) then
    __hh_msg_hunted_desc = "Try to stay alive - the others are going to kill you..."
  else
    __hh_msg_hunted_desc = "Try to stay alive - %s is going to kill you..."
  end
end

-- ====================================================================
-- Main function
-- ====================================================================
function do_hunter_hunted()
  local n

  if (__hh_temp_globals_max_time > 0) then

    if (mod(globals.game_time, 60) == 0) then     
      for n = 1, globals.players do
        if (__hh_hunter > 0 and __hh_hunted ~= n and __hh_player[n].time > 0) then 
          __hh_player[n].time = __hh_player[n].time - 1
        end
      end

      __hh_update_osds()
    end

    -- exit level when a hunter is on 0-time

    for n = 1, globals.players do
      if (__hh_hunter > 0 and
          __hh_player[n].time == 0) then
        globals.exit_level = 1
      end
    end

    __hh_illuminate_hunted()
        
  end -- if max_time > 0
end

-- ====================================================================
-- Updates OSDs
-- ====================================================================
function __hh_update_osds()
  local n
  
  for n = 1, globals.players do
    __hh_player[n].osd:clear(globals.col_red)
    __hh_player[n].osd:draw_rectangle()
    
    local remtime = __hh_player[n].time
    local mytext = string.format('%02d:%02d', remtime/60, mod(remtime,60))
  
    local col = globals.col_white
    __hh_player[n].osd:draw_text(FONT_IMPACT10, mytext, col, 5, 1, 0)
                     
  end
end

-- ====================================================================
-- Illuminates hunted player
-- ====================================================================
function __hh_illuminate_hunted()
  if (__hh_hunted > 0) then
    effects.pixels:add(2, player[__hh_hunted]:get_x()+8, player[__hh_hunted]:get_y()+8,
                       14, 14, 0, 0, 28,
                       PIX_YELLOW, 10, 4000, 15)
  end
end


-- ====================================================================
-- Restores changed variables on level exit
-- ====================================================================
function hook_exit_level_6()
  -- give bonus points
  local n
  for n = 1, globals.players do
    player[n]:inc_bonus(__hh_player[n].time * 20)
  end
 
  -- restore max time
  globals.max_time = __hh_temp_globals_max_time
end

-- ====================================================================
-- Chooses a new hunted player if one commits suicide
-- ====================================================================
function __hh_get_new_hunted(mysig)
  local newh = -1

  if (globals.players == 2) then
    if (__hh_hunted == 1) then newh = 2
    else newh = 1 end
    return newh
  end
  
  -- if more than 2 players: choose next by next higher signature
  local psigs = {}
  local n
  for n = 1, globals.players do
    psigs[n] = player[n]:get_sig()
  end
  
  sort(psigs)
  
  for n = 1, globals.players do
    if (psigs[n] > mysig) then
      newh = get_player_nr_by_sig(psigs[n]) + 1
      break
    end
  end

  -- if no newh change, then mysig is highest sig in list -> choose lowest
  if (newh == -1) then 
    newh = get_player_nr_by_sig(psigs[1]) + 1 
  end

  return newh
end

-- ====================================================================
-- One player is dead - change roles
-- ====================================================================
function hook_player_dead_6(pl)
  local pshot = get_player_nr_by_sig(pl:get_sig())
  local pkiller = get_player_nr_by_sig(pl:get_killedby())

  -- choose another player if suicide of hunted
  if (pkiller < 0 and __hh_hunted == pshot+1) then
    pkiller = __hh_get_new_hunted(player[__hh_hunted]:get_sig()) - 1
  end
   
  if (pkiller >= 0 and (__hh_hunted == pshot+1 or __hh_hunter == 0)) then
    __hh_hunter = pshot+1
    __hh_hunted = pkiller+1
                 
    local msg1 = string.format(__hh_msg_hunter_desc, player[__hh_hunted]:get_name())
    local msg2 = string.format(__hh_msg_hunted_desc, player[__hh_hunter]:get_name())
    
    -- show messages
    local screen1 = __hh_hunter
    local screen2 = __hh_hunted

    local msg_hunter = 0
    local msg_hunted = 0
    
    -- if network game: show only one message
    if (__hh_split == 3) then
      if (__hh_hunted == 1) then
        msg_hunted = 1
      else
        msg_hunter = 1
      end
      screen1 = 0
      screen2 = 0
    else
      msg_hunter = 1
      msg_hunted = 1
    end
        
    if (msg_hunter == 1) then
      show_player_message(__hh_msg_hunter, globals.col_yellow, 
                          msg1, globals.col_green,
                          60*4, screen1, 0)
    end
    
    if (msg_hunted == 1) then
      show_player_message(__hh_msg_hunted, globals.col_yellow, 
                          msg2, globals.col_green,
                          60*4, screen2, 0)
    end
    
    -- deactivate all weapons of hunted (except single blaster)
    local n
    local single = -1
    
    for n = 0,5 do
      local tslot = player[__hh_hunted]:get_weapon_slot(n)
      if (tslot.type == W_SINGLE) then single = n end
    end
        
    for n = 0,5 do
      if (n ~= single) then player[__hh_hunted]:deactivate_weapon_slot(n) end
      player[__hh_hunter]:activate_weapon_slot(n)
    end
    
    player[__hh_hunted]:set_active_weapon(single)
    
  end
end

-- ====================================================================
-- Init stuff
-- ====================================================================



end -- include
