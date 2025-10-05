-- LIB_CTF: Game mode "Capture the Flag"
-- include with: dofile("./levels/libctf.sc")
-- (01.08.01/jh, 10.02.02/jh)

if (__LIBCTF__ == nil) then

__LIBCTF__ = 1

-- some globals
__ctf_player_sig = {}
__ctf_player_hasflag = {}
__ctf_player_myflag = {}
__ctf_player_idle = {}
__ctf_player_targetarea = {}
__ctf_player_flags_saved = {}


if (LANGUAGE == LANG_DE) then
  __ctf_text_player_got_flag = "Spieler %s hat deine Flagge!"
  __ctf_text_player_lost_flag = "Spieler %s hat hat deine Flagge verloren..."
  __ctf_text_player_returned_flag = "Gratulation! Du hast die Flagge von %s gesichert (das ist deine %d.)"
else
  __ctf_text_player_got_flag = "Player %s got your flag!"
  __ctf_text_player_lost_flag = "Player %s lost your flag..."
  __ctf_text_player_returned_flag = "Congratulations! You saved the flag of %s (thats your %d.)" 
end

-- do_capture_the_flag()
-- call this function from the main script
function do_capture_the_flag()
  local num
  
--  if (globals.network_game == 1 and net:get_server() == 1) then
--    __ctf_update_user_values()
--    net:game_send_user_values() 
--  elseif (globals.network_game == 1) then
--    __ctf_process_user_values()
--  end
  
  for num = 1, globals.players do
    local pl = player[num]

    if (__ctf_player_idle[num] > 0) then 
      __ctf_player_idle[num] = __ctf_player_idle[num] - 1
    end
    
    -- special key pressed?
    if (pl.controls:special() == 1) then
      if (__ctf_player_hasflag[num] > -1) then
        if (__ctf_throw_flag(num, __ctf_player_hasflag[num]) == 1) then
          __ctf_player_hasflag[num] = -1
        end
      end
    end

    if (__ctf_player_hasflag[num] >= 0 and __ctf_player_hasflag[num] ~= __ctf_player_myflag[num]) then

      if (pl:get_x() > __ctf_player_targetarea[num].x1 and
          pl:get_x() < __ctf_player_targetarea[num].x2 and
          pl:get_y() > __ctf_player_targetarea[num].y1 and
          pl:get_y() < __ctf_player_targetarea[num].y2) then
      
        -- return flag to original place
        __ctf_return_flag(__ctf_player_hasflag[num])  

        __ctf_show_returned_flag_message(num, __ctf_player_hasflag[num])
        
        -- kill flag owner
        local owner = __ctf_get_player_by_flagnum(__ctf_player_hasflag[num])
        player[owner]:set_dead()
        player[owner]:inc_suicides()
        __ctf_player_hasflag[owner] = -1

        -- increase bonus points
        player[num]:inc_bonus(500)
        __ctf_player_flags_saved[num] = __ctf_player_flags_saved[num] + 1
        
        __ctf_player_hasflag[num] = -1
      end
    end
    
    -- is player still having his flag?
    if (__ctf_player_hasflag[num] > -1) then
      local flagob = objects:get_spobject(__ctf_player_hasflag[num])
      local pl = player[num]
      if (math.abs(pl:get_x()-flagob:get_x()) > 10 or
          math.abs(pl:get_y()-flagob:get_y()) > 10) then
--        __ctf_show_lost_flag_message(num, __ctf_player_hasflag[num])
        __ctf_player_hasflag[num] = -1
      end      
    end

    -- check if flag is still reachable...
    __ctf_check_flag_reachability(__ctf_player_myflag[num])
        
  end
end

function __ctf_throw_flag(plnum, flagnum)
  local hx = player[plnum]:get_x() + player[plnum]:get_width()/2
  local hy = player[plnum]:get_y() + player[plnum]:get_height()/2
  local obj = objects:get_spobject(flagnum)
  local dir = 0        -- 0: up, 1: down, 2: left, 3: right
  local pxspd = player[plnum]:get_xspd()
  local pyspd = player[plnum]:get_yspd()

  if (pxspd > 1) then dir = 2 end
  if (pxspd < -1) then dir = 3 end
  if (pyspd > 1) then dir = 0 end
  if (pyspd < -1) then dir = 1 end

  local map = playmap[1]
      
  local impossible = 0
  local hxp = 0
  local hyp = 0
  
  -- test direction
  if (dir == 0) then
    if (map:is_tile(hx, hy-30) == 0) then
      hxp = 0; hyp = -30
    else 
      dir = 1 
    end
  end
  
  if (dir == 1) then
    if (map:is_tile(hx, hy+30) == 0) then
      hxp = 0; hyp = 25
    else
      dir = 2
    end
  end

  if (dir == 2) then
    if (map:is_tile(hx-30, hy) == 0) then
      hxp = -25; hyp = 0
    else
      dir = 3
    end
  end
  
  if (dir == 3) then
    if (map:is_tile(hx+30, hy) == 0) then
      hxp = 25; hyp = 0
    else
      impossible = 1
    end
  end

  if (impossible == 0) then  
    obj:set_xspd(hxp/15)
    obj:set_yspd(hyp/15)
    __ctf_player_idle[plnum] = 60

--    __ctf_show_lost_flag_message(plnum, flagnum)
    
    return 1
  end

  return 0        
end

function __ctf_return_flag(flagnum)
  local plnum = __ctf_get_player_by_flagnum(flagnum)
  local obj = objects:get_spobject(flagnum)
  local hx = player[plnum]:get_home_x() + 30
  local hy = player[plnum]:get_home_y() - 6

  obj:set_x(hx)
  obj:set_y(hy)          
end

function hook_object_update_5(ob)
  if (ob:get_type() == SPOBJ_FLAG) then
    local xspd = ob:get_xspd()
    local yspd = ob:get_yspd()
    local dec = 0.06
    
    if (xspd >= dec) then xspd = xspd - dec
    elseif (xspd <= -dec) then xspd = xspd + dec
    else xspd = 0 end

    if (yspd >= dec) then yspd = yspd - dec
    elseif (yspd <= -dec) then yspd = yspd + dec
    else yspd = 0 end
    
    ob:set_xspd(xspd)
    ob:set_yspd(yspd)
  end
end

function hook_player_with_spobject_5(pl, ob)
  if (ob:get_type() == SPOBJ_FLAG) then
  
    local plnum = __ctf_get_player_num(pl:get_sig())
    local objnum = ob:get_nr()

if (nil) then
    -- show message when player gets enemy's flag
    local enemy_player = __ctf_get_player_by_flagnum(objnum)
    if (__ctf_player_idle[plnum] == 0 and
        __ctf_player_hasflag[plnum] == -1 and 
        enemy_player ~= plnum) then
        
      local mytext = string.format(__ctf_text_player_got_flag, player[plnum]:get_name())
      local screen = enemy_player
      if (enemy_player > 2) then screen = 1 end
      show_player_message(player[enemy_player]:get_name(), globals.col_yellow,
                          mytext, globals.col_green,
                          60*2, screen, 0)
                         
    end
end
              
    if (__ctf_player_idle[plnum] == 0 and
       (__ctf_player_hasflag[plnum] == -1 or __ctf_player_hasflag[plnum] == objnum)) then
      __ctf_player_hasflag[plnum] = objnum
      ob:set_x( pl:get_x() )
      ob:set_y( pl:get_y() )
    end
    
  end
end

function hook_player_dead_5(pl)
  local plnum = __ctf_get_player_num(pl:get_sig())
--  if (__ctf_player_hasflag[plnum] > 0) then __ctf_show_lost_flag_message(plnum, __ctf_player_hasflag[plnum]) end
  __ctf_player_hasflag[plnum] = -1  
end

function __ctf_get_player_by_flagnum(flagnum)
  local n
  for n = 1, table.getn(__ctf_player_myflag) do
    if (__ctf_player_myflag[n] == flagnum) then
      return n
    end
  end
  return 0
end

function __ctf_get_player_num(sig)
  local n
  for n = 1, table.getn(__ctf_player_sig) do
    if (__ctf_player_sig[n] == sig) then
      return n
    end
  end
  return 0
end

function __ctf_show_lost_flag_message(plnum, flagnum)
  local enemy_player = __ctf_get_player_by_flagnum(flagnum)
  if (enemy_player ~= plnum) then
        
    local mytext = string.format(__ctf_text_player_lost_flag, player[plnum]:get_name())
    local screen = enemy_player
    if (enemy_player > 2 or globals.network_game == 1) then screen = 1 end
    show_player_message(player[enemy_player]:get_name(), globals.col_yellow,
                        mytext, globals.col_green,
                        60*2, screen, 0)
  end
end

function __ctf_show_returned_flag_message(plnum, flagnum)
  local enemy_player = __ctf_get_player_by_flagnum(flagnum)
        
  local mytext = string.format(__ctf_text_player_returned_flag, 
                        player[enemy_player]:get_name(),
                        __ctf_player_flags_saved[plnum]+1)
                          
  local screen = plnum
  if (plnum > 2 or globals.network_game == 1) then screen = 1 end
  local okay = 1
  if (globals.network_game == 1 and plnum > 1) then okay = 0 end
  
  if (okay == 1) then
    show_player_message(player[plnum]:get_name(), globals.col_yellow,
                        mytext, globals.col_green,
                        60*4, screen, 0)
  end
end

function __ctf_sort_players_and_get_position(sig)
  local psigs = {}
  local n
  for n = 1, globals.players do
    psigs[n] = player[n]:get_sig()
  end
  
  table.sort(psigs)
  
  for n = 1, globals.players do
    if (psigs[n] == sig) then
      return n-1
    end
  end

  return 0
end

function __ctf_check_flag_reachability(flagnum)
  local myflag = objects:get_spobject(flagnum)
  local displaced = 0
  
  -- check for screen bounds
  if (myflag:get_x() > globals.map_w*globals.tile_w - myflag:get_width()) or
     (myflag:get_x() <= 0) or
     (myflag:get_y() > globals.map_h*globals.tile_h - myflag:get_height()) or
     (myflag:get_y() <= 0) then
     displaced = 1
  end
  
  if (displaced == 1) then
    __ctf_return_flag(flagnum)
    local owner = __ctf_get_player_by_flagnum(__ctf_player_hasflag[flagnum])
    __ctf_player_hasflag[owner] = -1
  end
end


function __ctf_update_user_values()
end

function __ctf_process_user_values()
end

-- place flags
for plnum = 1, globals.players do
  local hx = player[plnum]:get_x()
  local hy = player[plnum]:get_y()
  local num = player[plnum]:get_sig()
  
  if (hx == 0 and hy == 0) then
    player[plnum]:set_homebase(plnum-1)
    player[plnum]:return_home()
    hx = player[plnum]:get_x()
    hy = player[plnum]:get_y()
  end

  local flagnum = __ctf_sort_players_and_get_position( player[plnum]:get_sig() )
  objects:add_special( num, SPOBJ_FLAG, hx+30, hy-6, flagnum )

  __ctf_player_sig[plnum] = num
  __ctf_player_myflag[plnum] = num
  __ctf_player_hasflag[plnum] = -1
  __ctf_player_idle[plnum] = 0
  __ctf_player_flags_saved[plnum] = 0
  
  __ctf_player_targetarea[plnum] = { x1 = hx-20, y1 = hy-10, x2 = hx+20+20, y2 = hy+25 }
                           
end



end -- include
