-- Frapela5a
-- level script
-- von Frapela (Frapela sin Fru hat mal 'ne Pause eingelegt)
-- 30.11.2002

check_respawning_extras()

if schalter:get_dispframe() ~= letzter_zustand then
 if schalter:get_dispframe() == 0 then
   tuer1:set_maxframe(3)
   tuer1:set_startframe(1)
   tuer1:set_dispframe(1)
   tuer2:set_maxframe(0)
   tuer2:set_startframe(0)
   tuer2:set_dispframe(0)
 else
   tuer1:set_maxframe(0)
   tuer1:set_startframe(0)
   tuer1:set_dispframe(0)
   tuer2:set_maxframe(3)
   tuer2:set_startframe(1)
   tuer2:set_dispframe(1)
 end
  letzter_zustand = schalter:get_dispframe()
end