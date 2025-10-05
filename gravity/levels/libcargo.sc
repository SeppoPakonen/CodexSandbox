-- LIB_CARGO: Cargo and Base functions
-- include with: dofile("./levels/libcargo.sc")

if (__LIBCARGO__ == nil) then

__LIBCARGO__ = 1

-- *************************************************************************
-- Cargo control functions
-- 04.09.2002/jh
-- *************************************************************************

__cargo_weight = 0

function init_cargo()
  local mybase = bases:get_first_base()
  __cargo_weight = 0
  
  while (mybase) do
    if (mybase:get_cargo_type() == CARGO_NORMAL) then
      __cargo_weight = __cargo_weight + mybase:get_cargo_weight()
    end
    mybase = mybase:get_next()
  end
  
end

function check_cargo_return(pl)
  if (pl:get_weight() > 0) then
    __cargo_weight = __cargo_weight - pl:get_weight()
    if (__cargo_weight <= 0) then
      local myosd = playmap[1].osd:get_osdbit(1000)
      myosd:clear()
      pl:set_mission_status(1)
    end
  end
end

function check_cargo_dead(pl)
  if (pl:get_weight() > 0) then
    pl:set_mission_status(-1)
  end
end


end -- include
