-- Weapon Storage
-- Script

if mod(globals.game_time, 5) then
  if player[1]:get_weapon_bullets(0) == 0 then
    slotzaehler1 = 0
  elseif player[1]:get_weapon_bullets(1) == 0 then
    slotzaehler1 = 1
  elseif player[1]:get_weapon_bullets(2) == 0 then
    slotzaehler1 = 2
  elseif player[1]:get_weapon_bullets(3) == 0 then
    slotzaehler1 = 3
  elseif player[1]:get_weapon_bullets(4) == 0 then
    slotzaehler1 = 4
  elseif player[1]:get_weapon_bullets(5) == 0 then
    slotzaehler1 = 5
  else
    slotzaehler1 = player[1]:get_active_weapon()
  end
  if player[2]:get_weapon_bullets(0) == 0 then
    slotzaehler2 = 0
  elseif player[2]:get_weapon_bullets(1) == 0 then
    slotzaehler2 = 1
  elseif player[2]:get_weapon_bullets(2) == 0 then
    slotzaehler2 = 2
  elseif player[2]:get_weapon_bullets(3) == 0 then
    slotzaehler2 = 3
  elseif player[2]:get_weapon_bullets(4) == 0 then
    slotzaehler2 = 4
  elseif player[2]:get_weapon_bullets(5) == 0 then
    slotzaehler2 = 5
  else
    slotzaehler2 = player[2]:get_active_weapon()
  end
end -- modulation