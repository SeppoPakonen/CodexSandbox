-- LIB_EFFECTS: Some effects...
-- include with: dofile("./levels/libeff.sc")

if (__LIBEFFECTS__ == nil) then

__LIBEFFECTS__ = 1

-- ==============================================================
-- effect_player_damage( player_number )
-- player damage effect (trail of explosions) (24.06.01/jh)
-- ==============================================================
function effect_player_damage(pl)

  if player[pl]:get_dead() == 0 then
    local max_health = player[pl]:get_max_hull() + player[pl]:get_max_shield()
    local cur_health = player[pl]:get_hull() + player[pl]:get_shield()
    local health = ((cur_health+1) / max_health) * 100
    
    if (health < 90 and int_mod(globals.game_time, health/10) == 0) then
      local a = math.random(6)
      effects.explosions:add(EXP_SMALL_2, 5, 0, player[pl]:get_x()+1+a, player[pl]:get_y()+1+a)
    end

  end -- player not dead

end -- of function

-- ==============================================================
-- effect_enemy_damage()
-- enemy damage effect (trail of explosions) (19.04.02/jh)
-- ==============================================================
function effect_enemy_damage()
  enemy = enemies:get_first_enemy()
 
  while (enemy) do
   if (enemy:get_shrinked() == 0) then
    local health = ((enemy:get_maxhit()-enemy:get_hit()) / enemy:get_maxhit()) * 100
    local expltype = EXP_SMALL_2
    local expllen  = 4
    local anz = 7
    
    if (enemy:get_width() < 10 or enemy:get_height() < 10) then
      expltype = EXP_SMALL_4
      expllen = 2
      anz = 5
    end
    
    if (health < 90 and mod(globals.game_time, health/anz) == 0) then
      local plusx = enemy:get_width() / 5
      local plusy = enemy:get_height() / 5
      effects.explosions:add(expltype, expllen, 0,
                             enemy:get_x() + plusx + math.random(enemy:get_width()-plusx*2),
                             enemy:get_y() + plusy + math.random(enemy:get_height()-plusy*2) )
    end
   end -- not shrinked
   
   enemy = enemy:get_next()
  end -- while enemies

end -- of function

end -- include
