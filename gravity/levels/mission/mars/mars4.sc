-- Mars Mission IV
-- Script

if alleweg == 1 and player[1]:get_land() == 1 and player[1]:get_land_base() == 0 then
  player[1]:set_mission_status(1)
  alleweg = 0
end