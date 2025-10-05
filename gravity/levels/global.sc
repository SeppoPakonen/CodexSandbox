-- global.sc
-- Global script file
-- -> used when global_script in .lvl is set

update_remaining_time_osd1()

for n = 1, globals.players do
  effect_player_damage(n)
end

if (globals.capture_the_flag == 1) then
  do_capture_the_flag()
end

if (globals.hunter_hunted == 1) then
  do_hunter_hunted()
end

if (globals.race == 1) then
  do_race()
end
