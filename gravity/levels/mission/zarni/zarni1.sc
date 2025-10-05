if player_in_area(1, 1800, 600, 2000, 900) then
 player[1]:set_mission_status(1)
end

if schalter:get_dispframe() ~= letzter_zustand then
  tuer:set_maxframe(0)
  tuer:set_startframe(0)
  tuer:set_dispframe(0)

  letzter_zustand = schalter:get_dispframe()
end
