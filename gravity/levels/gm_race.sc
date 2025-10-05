-- LIB_RACE: Game mode "Race"
-- include with: dofile("./levels/librace.sc")
-- August 2002/jh

if (__LIBRACE__ == nil) then

__LIBRACE__ = 1

-- ***********************************************************
-- GLOBALS
-- ***********************************************************

dofile("./levels/libhelp.sc")

__race_start_together = nil

__race_wp = {}
__race_wp_count = 0

__race_pl = {{}}
__race_exit_counter = 0

if (globals.language == LANG_DE) then
  __race_text_lap = "Runde"
  __race_text_done = "fertig"
  __race_text_winner = "Gewinner"
  __race_text_fulltime = "Gesamtzeit"
else
  __race_text_lap = "Lap"
  __race_text_done = "done"
  __race_text_winner = "Winner"
  __race_text_fulltime = "Full Time"
end

globals.max_time = 0
-- In Splitscreen-Modus Time-OSDs killen...
if (globals.splitscreen == 1) then
  local n
  for n = 1, 2 do
    local tosd = playmap[n].osd:get_osdbit(1100+n)
    if (tosd) then playmap[n].osd:remove(tosd) end
  end
end

if (globals.players > 1) then 
  if (globals.extra_amount > 0) then
    globals.race_laps = globals.extra_amount
  end
end

-- ***********************************************************
-- FUNCTIONS
-- ***********************************************************

function __race_create_waypoints()
  local o = objects:get_first_object()
  local on = nil
  local xy = 0
  local waypoint = {}
  local wpw = 5

  while (o) do
    if (o:get_nr() < 5000 and o:get_type() >= OBJ_RACEPOLEL and o:get_type() <= OBJ_RACEPOLED) then

      -- clone waypoints if splitscreen
      if (globals.splitscreen == 1) then
        on = objects:add(o:get_nr()+5000, o:get_type(), o:get_x(), o:get_y())
        on:set_restrict_to_map(playmap[2])
        o:set_restrict_to_map(playmap[1])
        
        if (__race_wp_count > 0) then 
          on:set_dispframe(5)
          on:set_active(0)
        end
      end
        
      if (__race_wp_count > 0) then 
        o:set_dispframe(5)
        o:set_active(0)
      end
    
      if (xy == 0) then
        waypoint.x1 = o:get_x()
        waypoint.y1 = o:get_y()
        waypoint.o1 = o
        waypoint.on1 = on
        xy = 1
      else
        waypoint.x2 = o:get_x()
        waypoint.y2 = o:get_y()
        waypoint.o2 = o
        waypoint.on2 = on
        __race_wp_count = __race_wp_count + 1
        
        if waypoint.x1 > waypoint.x2 then
          waypoint.x1, waypoint.x2 = waypoint.x2, waypoint.x1
          waypoint.x1 = waypoint.x1 - wpw
          waypoint.x2 = waypoint.x2 + wpw
        end

        if waypoint.y1 > waypoint.y2 then
          waypoint.y1, waypoint.y2 = waypoint.y2, waypoint.y1
          waypoint.y1 = waypoint.y1 - wpw
          waypoint.y2 = waypoint.y2 + wpw
        end
        
        __race_wp[__race_wp_count] = waypoint
        waypoint = {}
        xy = 0
      end
            
    end
    
    o = o:get_next()
  end
end

function __race_activate_waypoint(wpnum, plnum)
  local n
  if (globals.splitscreen == 0) then plnum = 1 end

  if (plnum == 1) then
    -- activate wpnum  
    if (wpnum > 0) then
      __race_wp[wpnum].o1:set_active(1)
      __race_wp[wpnum].o2:set_active(1)
    end
  
    -- deactivate all others
    for n = 1, __race_wp_count do
      if (n ~= wpnum) then
        __race_wp[n].o1:set_dispframe(5)
        __race_wp[n].o1:set_active(0)
        __race_wp[n].o2:set_dispframe(5)
        __race_wp[n].o2:set_active(0)
      end
    end
    
  else
  
    if (wpnum > 0) then
      __race_wp[wpnum].on1:set_active(1)
      __race_wp[wpnum].on2:set_active(1)
    end
  
    -- deactivate all others
    for n = 1, __race_wp_count do
      if (n ~= wpnum) then
        __race_wp[n].on1:set_dispframe(5)
        __race_wp[n].on1:set_active(0)
        __race_wp[n].on2:set_dispframe(5)
        __race_wp[n].on2:set_active(0)
      end
    end
  
  end
end

function __race_new_lap(plnum)
  __race_pl[plnum].cur_lap = __race_pl[plnum].cur_lap + 1
  if (__race_pl[plnum].cur_lap > 1) then
    __race_pl[plnum].lap_time[__race_pl[plnum].cur_lap-1] = __race_pl[plnum].cur_time
  end
  __race_pl[plnum].cur_time = 0
end

function __race_check_waypoints()
  local pl
  for pl = 1, globals.players do
    local check_wp = __race_pl[pl].cur_wp
 
    if (collision(player[pl]:get_x(), player[pl]:get_y(), 
                  player[pl]:get_width(), player[pl]:get_height(),
                  __race_wp[check_wp].x1, __race_wp[check_wp].y1,
                  __race_wp[check_wp].x2 - __race_wp[check_wp].x1,
                  __race_wp[check_wp].y2 - __race_wp[check_wp].y1)) then
                  
      local next_wp = check_wp + 1
      if (next_wp > __race_wp_count) then next_wp = 1 end

      __race_pl[pl].last_wp = check_wp
      
      if (check_wp == 1) then
        __race_pl[pl].started = 1
        __race_pl[pl].startcounter = 0
        __race_new_lap(pl)
        
        if (globals.players > 1 and __race_start_together) then
          __race_pl[3-pl].started = 1
        elseif (globals.players > 1) then
          if (__race_pl[3-pl].started ~= 1) then __race_pl[3-pl].startcounter = 60*3 end
        end
      end

      if (__race_pl[pl].cur_lap > globals.race_laps) then
        __race_pl[pl].done = 1
        __race_activate_waypoint(0, pl) -- clear all waypoints
        gsound:play_sound(SOUND_CARGO_RETURNED, player[pl]:get_x(), player[pl]:get_y(), 100, 1600)
      else
        __race_activate_waypoint(next_wp, pl)
        __race_pl[pl].cur_wp = next_wp
      end

      __race_update_wpstatus_osd()
      
      gsound:play_sound(SOUND_CARGO_RETURNED, player[pl]:get_x(), player[pl]:get_y())

    end -- if collision
  end -- for players
end

function __race_update_time()
  local pl
  for pl = 1, globals.players do
    if (__race_pl[pl].started and not __race_pl[pl].done) then
      __race_pl[pl].cur_time = __race_pl[pl].cur_time + 1
      __race_pl[pl].full_time = __race_pl[pl].full_time + 1
    end
    
    if (__race_pl[pl].startcounter > 0) then
      __race_pl[pl].startcounter = __race_pl[pl].startcounter - 1
      if (__race_pl[pl].startcounter == 0) then
        __race_pl[pl].started = 1
      end
    end
  end
end

function __race_create_osd()
  if (globals.splitscreen == 0) then
    __race_pl[1].osd_fulltime = playmap[1].osd:add(1, 10, 10, 80, 25, OSDBIT_SCREEN)
    __race_pl[1].osd_wpstatus[1] = gpanel[1].osd:add(1, 330, 12, 300, 22, OSDBIT_PANEL)

    local backcolor = gpanel[1].osd:get_osdbit(1000):get_last_clear_color()
    __race_pl[1].osd_wpstatus[1]:clear(backcolor)
    __race_update_wpstatus_osd(1)
  else
  
    local num
    for num = 1, 2 do
      __race_pl[num].osd_fulltime = playmap[num].osd:add(1, 10, 10, 80, 25, OSDBIT_SCREEN)
      __race_pl[num].osd_wpstatus[1] = playmap[num].osd:add(2, 80, 10, 250, 18, OSDBIT_SCREEN)
      __race_pl[num].osd_wpstatus[2] = playmap[num].osd:add(3, 80, 20, 250, 28, OSDBIT_SCREEN)

      __race_pl[num].osd_wpstatus[1]:clear()
      __race_pl[num].osd_wpstatus[2]:clear()
    end

    __race_update_wpstatus_osd()

  end
end

function __race_update_fulltime_osd()
  local pl
  for pl = 1, globals.players do
    local mytime
    local ct = __race_pl[pl].full_time
    local st = __race_pl[pl].startcounter
    local col = globals.col_yellow
    local coln = globals.col_orange

    __race_pl[pl].osd_fulltime:clear()
        
    if (st > 0) then
      mytime = __race_convert_time(st)
      
     __race_pl[pl].osd_fulltime:draw_text(FONT_IMPACT14, mytime, coln, 0, 0)
    else  
      if (__race_pl[pl].done) then col = globals.col_green end
 
      mytime = __race_convert_time(ct)

     __race_pl[pl].osd_fulltime:draw_text(FONT_IMPACT14, mytime, col, 0, 0)
   end
 end
end

function __race_update_wpstatus_osd()
  local wpcount = __race_wp_count * globals.race_laps
  local curwp   = 0
  local max_w   = 298  -- width of rectangle
  local y1      = 0
  local y2      = 8
  local mytext  = ""
  local pl      = 0
  local plnum   = 1
  if (globals.splitscreen == 1) then 
    plnum = 2 
    max_w = 220
  end

  for pl = 1, plnum do
       
    if (globals.splitscreen == 0) then
      curwp = ((__race_pl[pl].cur_lap - 1) * __race_wp_count) + __race_pl[pl].last_wp - 1
      __race_pl[pl].osd_wpstatus[1]:clear()
      __race_pl[pl].osd_wpstatus[1]:draw_rectangle(globals.col_grey, 0, y1, max_w+1, y2)
  
      if (curwp < wpcount) then
        mytext  = string.format("%s: %s %d/%d", player[pl]:get_name(), __race_text_lap, __race_pl[pl].cur_lap, globals.race_laps)
        if (curwp > 0) then __race_pl[pl].osd_wpstatus[1]:draw_filled_rectangle(globals.col_blue, 1, y1+1, max_w * (curwp / wpcount), y2-1) end
      else
        mytext  = string.format("%s (%s)", player[pl]:get_name(), __race_text_done)
        __race_pl[pl].osd_wpstatus[1]:draw_filled_rectangle(globals.col_lblue, 1, y1+1, max_w, y2-1)
      end
      __race_pl[pl].osd_wpstatus[1]:draw_text(FONT_MINIFONT, mytext, globals.col_yellow, 2, y1+2)
      
    else
   
      local n
      for n = 1, 2 do 

        local npl
        if (n == 2) then npl = 3-pl else npl = pl end
  
        curwp = ((__race_pl[npl].cur_lap - 1) * __race_wp_count) + __race_pl[npl].last_wp - 1

        __race_pl[pl].osd_wpstatus[n]:clear()
        __race_pl[pl].osd_wpstatus[n]:draw_rectangle(globals.col_grey, 0, y1, max_w+1, y2)

        if (curwp < wpcount) then
          mytext  = string.format("%s: %s %d/%d", player[npl]:get_name(), __race_text_lap, __race_pl[npl].cur_lap, globals.race_laps)
          if (curwp > 0) then __race_pl[pl].osd_wpstatus[n]:draw_filled_rectangle(globals.col_blue, 1, y1+1, max_w * (curwp / wpcount), y2-1) end
        else
          mytext  = string.format("%s (%s)", player[npl]:get_name(), __race_text_done)
          __race_pl[pl].osd_wpstatus[n]:draw_filled_rectangle(globals.col_lblue, 1, y1+1, max_w, y2-1)
        end
        __race_pl[pl].osd_wpstatus[n]:draw_text(FONT_MINIFONT, mytext, globals.col_yellow, 2, y1+2)
       
      end -- for osd
    end -- if splitscreen
  end -- for players
end

function __race_init_players()
  local pl
  for pl = 1, globals.players do
    
--    player[pl]:set_fuel_dec(0)
    player[pl]:set_lifes(99)

    __race_pl[pl] = {}
    __race_pl[pl].pl = player[pl]    -- player structure
    __race_pl[pl].cur_lap = 0        -- current lap
    __race_pl[pl].cur_wp = 1         -- current waypoint to fly through
    __race_pl[pl].last_wp = 0        -- last waypoint...
    __race_pl[pl].started = nil      -- player passed first wp?
    __race_pl[pl].done = nil         -- player finished?
    __race_pl[pl].cur_time = 0       -- current time in this lap
    __race_pl[pl].full_time = 0      -- current overall time
    __race_pl[pl].osd_fulltime = nil -- osd object for player time
    __race_pl[pl].osd_wpstatus = {}  -- osd object for waypoint status
    __race_pl[pl].lap_time = {}      -- array with lap times
    __race_pl[pl].startcounter = 60*15  -- time to start time :)
    
    local l
    for l = 1, globals.race_laps do
      __race_pl[pl].lap_time[l] = 0
    end
  end

--if (nil) then  
  if (globals.players == 2) then
    local mybase = bases:get_base(0)
    player[1]:set_home_x( mybase:get_land_x()+10 )
    player[1]:set_home_y( mybase:get_land_y() - player[1]:get_height() )
    player[2]:set_home_x( mybase:get_land_x()+mybase:get_land_w()-player[2]:get_width()-10 )
    player[2]:set_home_y( mybase:get_land_y() - player[2]:get_height() )
    player[1]:return_home()
    player[2]:return_home()
  end
--end
end

function __race_create_highscores()
  local n
  local laps = globals.race_laps
  if (laps > 8) then laps = 8 end
  
  globals.use_user_stats = 1
  
  if (globals.players == 1) then
  
    for n = 1, laps do
      local mytext = string.format("%s %d", __race_text_lap, n)
      globals.user_stats[n]:set_text(mytext)
      globals.user_stats[n]:set_plusx(155)
      
      local mytime = __race_convert_time(__race_pl[1].lap_time[n])
      globals.user_stats[n]:set_value(0, mytime)
    end  
    
    globals.user_stats[laps+2]:set_text(__race_text_fulltime)
    globals.user_stats[laps+2]:set_plusx(155)
    
    local mytime = __race_convert_time(__race_pl[1].full_time)
    globals.user_stats[laps+2]:set_value(0, mytime)
    globals.user_stats[laps+2]:set_valuecolor(0, globals.col_white)

    if (__race_pl[1].done) then
      globals:set_highscore(mytime)
    end
      
  elseif (globals.players == 2) then
    for n = 1, laps do
      local mytext = string.format("%s %d", __race_text_lap, n)
      globals.user_stats[n]:set_text(mytext)
      globals.user_stats[n]:set_plusx(55)
      
      local mytime = __race_convert_time(__race_pl[1].lap_time[n])
      globals.user_stats[n]:set_value(0, mytime)
      local mytime = __race_convert_time(__race_pl[2].lap_time[n])
      globals.user_stats[n]:set_value(1, mytime)
    end  
    
    globals.user_stats[laps+2]:set_text(__race_text_fulltime)
    globals.user_stats[laps+2]:set_plusx(55)
    
    local mytime = __race_convert_time(__race_pl[1].full_time)
    globals.user_stats[laps+2]:set_value(0, mytime)
    globals.user_stats[laps+2]:set_valuecolor(0, globals.col_orange)

    local mytime = __race_convert_time(__race_pl[2].full_time)
    globals.user_stats[laps+2]:set_value(1, mytime)
    globals.user_stats[laps+2]:set_valuecolor(1, globals.col_orange)
    
    local winner = 0
    if (__race_pl[2].full_time < __race_pl[1].full_time) then
      winner = 1
    end
    if (__race_pl[2].full_time ~= __race_pl[1].full_time) then
      globals.user_stats[laps+2]:set_valuecolor(winner, globals.col_green)
    end
  end
end

function __race_check_finish()
  local pl
  local okay = 1
  if (__race_exit_counter == 0) then
    for pl = 1, globals.players do
      if (not __race_pl[pl].done) then
        okay = nil
      end
    end
    if (okay) then 
      __race_exit_counter = 60*4 
      
      if (globals.splitscreen == 1) then
        local n
        for n = 1, 2 do
          local myosd = playmap[n].osd:get_osdbit(1000)
          local winner = 1
          if (__race_pl[2].full_time < __race_pl[1].full_time) then
            winner = 2
          end
          local mytext = string.format("[center][c:white][f:impact14]%s: [w:30][c:yellow]%s", __race_text_winner, player[winner]:get_name())
          myosd:draw_typewriter_text(mytext, 30, 8, 0)
        end
      end
      
    end
  else
    if (__race_exit_counter > 1) then
      __race_exit_counter = __race_exit_counter - 1
    elseif (__race_exit_counter == 1) then
      globals.exit_level = 1
    end
  end
end

function __race_convert_time(t)
  return string.format("%02d:%02d:%02d", 
                t/3600,
                mod(t, 3600) / 60,
                mod(t, 60)*1.667)
end

function hook_exit_level_0()
  __race_create_highscores()
end

function do_race()
  __race_check_waypoints()
  __race_check_finish()
  __race_update_time()
  __race_update_fulltime_osd()
end

-- ***********************************************************
-- INITIALIZATION
-- ***********************************************************

__race_create_waypoints()
__race_init_players()
__race_create_osd()


end -- include
