-- LIB_SW: Functions for Switches and Doors
-- include with: dofile("./levels/libsw.sc")

if (__LIBSWITCH__ == nil) then

__LIBSWITCH__ = 1

-- *************************************************************************
-- Door/switch control functions
-- 04.08.2002/jh
-- *************************************************************************

__doorswitch_data = {}
__doorswitch_num = 0

function close_door( door )
  if (door:get_type() >= OBJ_DOORH and door:get_type() <= OBJ_DOORV2) then
    door:set_maxframe(3)
    door:set_startframe(1)
    door:set_dispframe(1)
  end
end

function open_door( door )
  if (door:get_type() >= OBJ_DOORH and door:get_type() <= OBJ_DOORV2) then
    door:set_maxframe(0)
    door:set_startframe(0)
    door:set_dispframe(0)
  end
end

function switch_on( sw )
  if (sw:get_type() >= OBJ_SWITCHR and sw:get_type() <= OBJ_SWITCHD) then
    sw:set_dispframe(1)
  end
end

function switch_off( sw )
  if (sw:get_type() >= OBJ_SWITCHR and sw:get_type() <= OBJ_SWITCHD) then
    sw:set_dispframe(0)
  end
end

function door_status( door )
  if (door:get_type() >= OBJ_DOORH and door:get_type() <= OBJ_DOORV2) then
    if (door:get_maxframe() == 3) then
      return 1
    else
      return 0
    end
  else
    return -1
  end
end

function switch_status( sw )
  if (sw:get_type() >= OBJ_SWITCHR and sw:get_type() <= OBJ_SWITCHD) then
    if (sw:get_dispframe() == 0) then
      return 0
    else
      return 1
    end
  else
    return -1
  end
end

function register_door_with_switch( door, sw )
  if (sw:get_type() >= OBJ_SWITCHR and sw:get_type() <= OBJ_SWITCHD and
      door:get_type() >= OBJ_DOORH and door:get_type() <= OBJ_DOORV2) then
    __doorswitch_num = __doorswitch_num + 1  
    __doorswitch_data[ __doorswitch_num ] = {}
    __doorswitch_data[ __doorswitch_num ].door = door
    __doorswitch_data[ __doorswitch_num ].switch = sw
  end
end

function check_door_switch(sw)
  local n
  for n = 1, __doorswitch_num do
    if (sw:get_nr() == __doorswitch_data[n].switch:get_nr()) then
      if (door_status(__doorswitch_data[n].door) == 0) then
        close_door(__doorswitch_data[n].door)
      else
        open_door(__doorswitch_data[n].door)
      end
    end
  end
end


end -- include
