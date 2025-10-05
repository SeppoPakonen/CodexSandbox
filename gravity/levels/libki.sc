-- LIB_KI: Functions for coroutines/"ki"
-- include with: dofile("./levels/libki.sc")

if (__LIBKI__ == nil) then

__LIBKI__ = 1

__ki_table = {}
__ki_count = 0


-- call this in the SC: resumes all registered coroutines
function do_ki()
  local n
  for n = 1, __ki_count do
    if (__ki_table[n].active == 1) then
      coroutine.resume(__ki_table[n].coroutine, __ki_table[n].object,
                       __ki_table[n].user1, __ki_table[n].user2,
                       __ki_table[n].user3, __ki_table[n].user4, __ki_table[n].user5)
      if (coroutine.status(__ki_table[n].coroutine) == "dead") then
        __ki_table[n].active = 0
      end
    end
  end
end

-- call this in the INI: register an enemy/object/whatever with a function
function register_enemy_ki(what, func, user1, user2, user3, user4, user5)
  register_ki("enemy", what, func, user1, user2, user3, user4, user5)
end

function register_object_ki(what, func, user1, user2, user3, user4, user5)
  register_ki("object", what, func, user1, user2, user3, user4, user5)
end

function register_ki(type, what, func, user1, user2, user3, user4, user5)
  __ki_count = __ki_count + 1
  __ki_table[ __ki_count ] = {}
  __ki_table[ __ki_count ].active = 1
  __ki_table[ __ki_count ].type = type
  __ki_table[ __ki_count ].object = what
  __ki_table[ __ki_count ].func = func
  __ki_table[ __ki_count ].user1 = user1
  __ki_table[ __ki_count ].user2 = user2
  __ki_table[ __ki_count ].user3 = user3
  __ki_table[ __ki_count ].user4 = user4
  __ki_table[ __ki_count ].user5 = user5
  __ki_table[ __ki_count ].coroutine = coroutine.create(__ki_table[ __ki_count ].func)
end


function hook_enemy_dead_6(en)
  local n
  for n = 1, __ki_count do
    if (__ki_table[n].active == 1 and __ki_table[n].type == "enemy" and
        __ki_table[n].object:get_nr() == en:get_nr()) then
        __ki_table[n].active = 0
    end
  end  
end

function hook_object_dead_6(ob)
  local n
  for n = 1, __ki_count do
    if (__ki_table[n].active == 1 and __ki_table[n].type == "object" and
        __ki_table[n].object:get_nr() == ob:get_nr()) then
        __ki_table[n].active = 0
    end
  end  
end

-- enemy ki
function __ki_enemy_follow_player(en, speed)
  en.wp:remove_all()
  en.wp:add(en:get_x(), en:get_y(), 1.0)

  if (speed == nil) then speed = 1.0 end

  local oldx = en:get_x()
  local oldy = en:get_y()

  while (1) do
    local enx = en:get_x()
    local eny = en:get_y()
    local enw = en:get_width()
    local enh = en:get_height()
    local plx = player[1]:get_x()
    local ply = player[1]:get_y()

    local mywp = en.wp:get(0)

    -- check surrounding
    if (playmap[1]:is_pixel( enx - 20, eny + enh/2 ) == 1 or
        playmap[1]:is_pixel( enx + enw + 20, eny + enh/2) == 1 or
        playmap[1]:is_pixel( enx + enw/2, eny - 20) == 1 or
        playmap[1]:is_pixel( enx + enw/2, eny + enh + 20) == 1) then
      mywp:set_x( oldx )
      mywp:set_y( oldy )
      en.wp:set_current(0)
    else
      oldx = enx
      oldy = eny
      mywp:set_x( plx )
      mywp:set_y( ply )
      mywp:set_speed(speed)
      en.wp:set_current(0)
    end

    coroutine.yield()
  end
end


function __ki_enemy_kamikaze(en, mindist)
  en.wp:remove_all()
  en.wp:add(en:get_x(), en:get_y(), 1.0)

  if (mindist == nil) then mindist = 80 end

  while (1) do
    local enx = en:get_x() + en:get_width()/2
    local eny = en:get_y() + en:get_height()/2
    local plx = player[1]:get_x()
    local ply = player[1]:get_y()
    local dist = math.sqrt( (enx-plx)^2 + (eny-ply)^2 )

    if (dist < mindist) then
      local mywp = en.wp:get(0)
      mywp:set_x(plx)
      mywp:set_y(ply)
      mywp:set_speed(10)
      en.wp:set_current(0)
    end

    coroutine.yield()
  end
end


end -- include
