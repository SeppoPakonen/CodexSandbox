-- Mars Mission VI
-- Script

if player[1]:get_land_base() == 5 then
  do_rabbit_stuff()
elseif player[1]:get_land_base() == 6 then
  player[1]:set_homebase(102)
end

if player[1]:get_land() ~= 1 then
  if timeout_yes == 1 then
    local anotherosd = playmap[1].osd:get_osdbit(1000)
    anotherosd:set_timeout(1)
    timeout_yes = 0
  end
end

-- if globals.game_time == zaehl_uhr + 720 then
--   bombpic = objects:add(98, USER_OBJECT01, 1, 10)
--   bombpic:set_invisible(1)
--   the_home_base:set_cargo_type(CARGO_OBJECT)
--   the_home_base:set_cargo_object(bombpic)
--   the_home_base:set_cargo_weight(20)
--   the_home_base:set_cargo_name("Wenn Sie das lesen, funzt das OSD nicht!")
-- end

if globals.game_time == zaehl_uhr + 720 then
--  haenge_bombe = objects:add_special(1234, USER_OBJECT01, player[1]:get_x(), player[1]:get_y() + 10, 0)
  haenge_bombe = objects:add_special(1234, USER_OBJECT01, player[1]:get_x() + breite, player[1]:get_y() + hoehe - 8, 0)
  player[1]:set_weapon(0, W_NONE, 0, 1)
  player[1]:set_weapon(1, W_NONE, 0, 1)
  player[1]:set_weapon(2, W_NONE, 0, 1)
  player[1]:set_weapon(3, W_NONE, 0, 1)
  player[1]:set_weapon(4, W_NONE, 0, 1)
  player[1]:set_weapon(5, W_NONE, 0, 1)
  player[1]:set_active_weapon(0)
  schongeworfen = 0
  player[1]:set_homebase(102)
  player[1]:set_weight(20)
end

if player[1].controls:shoot() == 1 then
  if schongeworfen == 0 then
    schongeworfen = 1
    player[1]:set_weight(0)
  end
end

if sieges_uhr + 180 == globals.game_time then
  player[1]:set_mission_status(1)
end