-- Frapela5b
-- level script
-- von Frapela und Frapela sin Fru als ... na ihr wisst schon.
-- 29.11.2002

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

local i
for i = 1, 20 do
  if enemies:get_enemy(i) then
    x = player[1]:get_x()
    y = player[1]:get_y()
    verfolgerx[i] = verfolger[i]:get_x()
    verfolgery[i] = verfolger[i]:get_y()
    verfolgerdist = sqrt( (x-verfolgerx[i])^2 + (y-verfolgery[i])^2 )
    if verfolgerdist <= 280 then
      local s, t
      for s = verfolgery[i]-2, verfolgery[i]+25, 27 do
        for t = verfolgerx[i]-2, verfolgerx[i]+44 do
          if playmap[1]:is_pixel(t, s) == 1 then
            hilfsvar[i] = 1
            s = verfolgery[i]+25
            t = verfolgerx[i]+44
          end
        end
      end
      if hilfsvar[i] == 0 then
        local s, t
        for s = verfolgerx[i]-2, verfolgerx[i]+44, 46 do
          for t = verfolgery[i]-2, verfolgery[i]+25 do
            if playmap[1]:is_pixel(s, t) == 1 then
              hilfsvar[i] = 1
              s = verfolgerx[i]+44
              t = verfolgery[i]+25
            end
          end
        end
      end
      if hilfsvar[i] == 0 then
        xalt[i] = verfolgerx[i]
        yalt[i] = verfolgery[i]
        verfolgerwp[i] = verfolger[i].wp:get(0)
        verfolgerwp[i]:set_x( x )
        verfolgerwp[i]:set_y( y )
        verfolgerwp[i]:set_speed(1.0)
        verfolger[i].wp:set_current(0)
      else
        hilfsvar[i] = 0
        verfolgerx[i] = xalt[i]
        verfolgery[i] = yalt[i]
        verfolgerwp[i] = verfolger[i].wp:get(0)
        verfolgerwp[i]:set_x( verfolgerx[i] )
        verfolgerwp[i]:set_y( verfolgery[i] )
        verfolgerwp[i]:set_speed(1.0)
        verfolger[i].wp:set_current(0)
      end
    else
      verfolgerwp[i] = verfolger[i].wp:get(0)
      verfolgerwp[i]:set_x( verfolgerx[i] )
      verfolgerwp[i]:set_y( verfolgery[i] )
      verfolgerwp[i]:set_speed(1.0)
      verfolger[i].wp:set_current(0)
    end

  end
end
