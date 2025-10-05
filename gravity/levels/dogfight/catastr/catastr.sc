-- Catastrophe
-- level script

check_respawning_extras()


if (mod(globals.game_time, 2) == 0) then
  effects.pixels:add(2, 2237, 1487, 3, 3,  0.8, -1.7, 250, PIX_BLUE, 10, 325, 80, 468, 300)
  effects.pixels:add(2, 2494, 1488, 3, 3, -0.8, -1.7, 250, PIX_BLUE, 10, 325, 80, 468, 300)
end
