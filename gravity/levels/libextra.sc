-- LIB_EXTRA: Functions with Extras
-- include with: dofile("./levels/libextra.sc")

if (__LIBEXTRA__ == nil) then

__LIBEXTRA__ = 1

-- *************************************************************************
-- RESPAWNING EXTRAS
-- 11.01.2002/jh
--
-- In INI script call: add_respawning_extra( xpos, ypos, strength, seconds )
-- In SC script call: check_respawning_extras()
-- *************************************************************************

__rextra_data = {}
__rextra_num = 0

function add_respawning_extra( xpos, ypos, strength, seconds, id )
  local myid
  local mynum

  if (id == nil) then 
    __rextra_num = __rextra_num + 1
    myid = 10000 + __rextra_num
    mynum = __rextra_num
  else
    myid = id
    local a
    for a = 1, __rextra_num do
      if (__rextra_data[a].id == myid) then
        mynum = a
      end
    end
  end
  
  objects:add_special( myid, SPOBJ_EXTRA, xpos, ypos, strength )
  __rextra_data[ mynum ] = {}
  __rextra_data[ mynum ].counter = seconds * 60
  __rextra_data[ mynum ].xpos = xpos
  __rextra_data[ mynum ].ypos = ypos
  __rextra_data[ mynum ].strength = strength
  __rextra_data[ mynum ].seconds = seconds
  __rextra_data[ mynum ].id = myid
end

function check_respawning_extras()
  local num
  for num = 1, __rextra_num do
    local extraobj = objects:get_spobject( __rextra_data[num].id )

    if (extraobj == nil) then
      __rextra_data[ num ].counter = __rextra_data[ num ].counter - 1
      if (__rextra_data[ num ].counter == 0) then
        add_respawning_extra( __rextra_data[num].xpos, __rextra_data[num].ypos, __rextra_data[num].strength, __rextra_data[num].seconds, __rextra_data[num].id )
      end
    end

  end
end


end -- include
