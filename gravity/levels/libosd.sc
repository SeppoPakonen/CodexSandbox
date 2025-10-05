-- LIB_OSD: Some OSD things...
-- include with: dofile("./levels/libosd.sc")

if (__LIBOSD__ == nil) then

__LIBOSD__ = 1

-- globals
screens = 1
if (globals.splitscreen == 1) then screens = 2 end

default_osd = {}
default_osd[1] = playmap[1].osd:get_osdbit(1000)
if (screens == 2) then default_osd[2] = playmap[2].osd:get_osdbit(1000) end

-- ==============================================================
-- Remaining-Time-OSD 1: A simple down-counting clock
-- you can give x/y coordinates if you want (default pos: 0/0)
-- (08.08.01/jh)
-- ==============================================================
function init_remaining_time_osd1(x, y)
  if (globals.max_time > 0) then
    if (x==nil) then x = 0 end
    if (y==nil) then y = 0 end
    remtime1osd = {}
    local a
    for a = 1, screens do
      remtime1osd[a] = playmap[a].osd:add( 1100+a, x, y, 41, 15, OSDBIT_SCREEN )
      remtime1osd[a]:clear(globals.col_blue)
      remtime1osd[a]:draw_rectangle()
      remtime1osd[a].lasttime = 0
    end
  end
end

function update_remaining_time_osd1()
  if (globals.max_time > 0 and
      globals.second_time ~= remtime1osd[1].lasttime) then
    local remtime = globals.max_time - globals.second_time
    local mytext = string.format('%02d:%02d', remtime/60, int_mod(remtime,60))
    local a
    for a = 1, screens do
      remtime1osd[a]:clear(globals.col_blue);
      remtime1osd[a]:draw_rectangle()
      local col = globals.col_white
      if (remtime <= 15) then col = globals.col_orange end
      remtime1osd[a]:draw_text(FONT_IMPACT10, mytext, col, 5, 1, 0)
    end
    remtime1osd[1].lasttime = globals.second_time
  end
end

-- ==============================================================
-- Remaining-Time-OSD 2: A down-counting bar :-) (BETA)
-- you can give x/y coordinates if you want (default pos: 0/0)
-- (08.08.01/jh)
-- ==============================================================
function init_remaining_time_osd2(x, y)
  if (x==nil) then x = 0 end
  if (y==nil) then y = 0 end
  remtime2osd = {}
  local a
  for a = 1, screens do
    remtime2osd[a] = playmap[a].osd:add( 1110+a+2, x, y, 61, 15, OSDBIT_SCREEN )
    remtime2osd[a]:clear(globals.col_yellow)
    remtime2osd[a]:draw_rectangle()
    remtime2osd[a].lasttime = 0
  end
end

function update_remaining_time_osd2()
  local remtime = globals.max_time*60 - globals.game_time
  local t = (remtime2osd[1]:get_w()-3) / (globals.max_time*60)
  local a
  for a = 1, screens do
    remtime2osd[a]:draw_filled_rectangle(0,
                                         t * remtime+1, 1,
                                         t * remtime-t+1,
                                         remtime2osd[a]:get_h()-2)
  end
end


-- ==============================================================
-- show_player_message(title, col1, text, col2, time, screen, rect)
-- Shows a message on the default OSD (screen is 1 [left] or 2 [right])
-- (12.08.01/jh)
-- ==============================================================
function show_player_message(title, col1, text, col2, time, screen, rect)
  if (screens == 1) then screen = 1 end
  if (not default_osd[screen]) then return end
  
  default_osd[screen]:clear()
  if (rect == 1) then default_osd[screen]:draw_rectangle() end
  default_osd[screen]:draw_text(FONT_IMPACT14, title, col1, 0, 10, 1)
  default_osd[screen]:draw_text(FONT_IMPACT10, text, col2, 0, 30, 1)
  default_osd[screen]:set_active(1)
  default_osd[screen]:set_timeout(time)
end


-- ==============================================================
-- init_base_labels
-- Shows labels with player's name under base
-- (10.03.02/jh)
-- ==============================================================
function init_base_labels()
  local curbase = bases:get_first_base()

  while (curbase) do
    local nextbase = curbase:get_next()
    local x = curbase:get_x() + 2
    local y = curbase:get_y() + curbase:get_height() + 2
    local text = ""

    -- get player for base
    local n
    local pl = -1
    for n = 1, globals.players do 
      if (player[n]:get_homebase() == curbase:get_nr()) then pl = n end
    end

    if (pl >= 0) then 
      local text = player[pl]:get_name()
      playmap[1].osd:add_label(2000+curbase:get_nr(), x, y, OSDBIT_MAP, FONT_MINIFONT, text, globals.col_yellow, 0)
    end
   
    curbase = nextbase
  end
end


end -- include
