-- LIB_HELP: Some helper functions
-- include with: dofile("./levels/libhelp.sc")

if (__LIBHELP__ == nil) then

__LIBHELP__ = 1

-- *************************************************************************
-- Some movement helpers
-- *************************************************************************

function get_speed_2_move_to( from_x, from_y, to_x, to_y, hops )
  local xspd
  local yspd
  
  local xdist = to_x - from_x
  local ydist = to_y - from_y
  
  xspd = xdist / hops
  yspd = ydist / hops
  
  return xspd, yspd
end

function collision(x1, y1, w1, h1, x2, y2, w2, h2)
  if (x1 > x2+w2 or x2 > x1+w1 or y1 > y2+h2 or y2 > y1+h1) then
    return nil
  else
    return 1
  end
end

function distance( x1, y1, x2, y2 )
  return math.sqrt( (x2-x1)^2 + (y2-y1)^2 )
end

function player_in_area(num, x1, y1, x2, y2)
  if player[num]:get_x() > x1 and
     player[num]:get_x() < x2 and
     player[num]:get_y() > y1 and
     player[num]:get_y() < y2 then
    return 1
  end
 
  return nil
end                        


-- *************************************************************************
-- Rueckwaertskompatibilitaet zu frueherer Aufteilung
-- *************************************************************************
dofile("./levels/libextra.sc")


end -- include
