-- Frapela1
-- level script
-- von Frapela
-- 16.11.2002


-- wenn Schalter getroffen, ändere Zustand der Tür
if schalter:get_dispframe() ~= letzter_zustand then
 if schalter:get_dispframe() == 0 then
   tuer0:set_maxframe(3)   
   tuer0:set_startframe(1) 
   tuer0:set_dispframe(1) 
 else
   tuer0:set_maxframe(0)   
   tuer0:set_startframe(0) 
   tuer0:set_dispframe(0)
 end
  letzter_zustand = schalter:get_dispframe()
end

-- Verfolgung oben rechts
-- wenn Gegner Nr. 46 lebt
if enemies:get_enemy(46) then
 x = player[1]:get_x()
 y = player[1]:get_y()
 if (x >= 1780 and x <= 2270) and (y >=  535 and y <= 1370) then
  x1 = player[1]:get_x()
  y1 = player[1]:get_y()
  blaugr46.wp:add( x1, y1, 0.9 )
 end
end


-- Verfolgung über Heimat-Basis
-- wenn Gegner Nr. 2 lebt
if enemies:get_enemy(2) then
 x2 = player[1]:get_x()
 y2 = player[1]:get_y()
 if (x2 >=  535 and x2 <= 1160) and (y2 >= 1613 and y2 <= 1885) then 
   x3 = player[1]:get_x()
   y3 = player[1]:get_y()
   mywp = blaugr2.wp:get(1)
   mywp:set_x( x3 )
   mywp:set_y( y3 )
   mywp:set_speed( 0.9 )
   blaugr2.wp:set_current(1)
 elseif blaugr2.wp:get(1) then
   blaugr2.wp:get(1):set_x( blaugr2.wp:get(0):get_x() )
   blaugr2.wp:get(1):set_y( blaugr2.wp:get(0):get_y() )
 end
end
