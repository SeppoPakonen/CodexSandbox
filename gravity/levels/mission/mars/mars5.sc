-- Mars Mission V
-- Script

-- Jaja, schon gut, ich weiss, dass man das Ganze
-- auch mittels Funktionen einbinden haette koennen,
-- aber ein Hauptscript ueber 10k laesst den Leuten
-- den Spass, sich darueber zu wundern.

-- Die letzen Worte eines Atari-Programmierers:
-- "Mein Text ist schon 20k lang, und gleich
-- werde ich ihn spei..."

if mod(globals.game_time, 60) == 0 then
if start_the_stuff == 0 then
  if player[1]:get_land() == 0 then
    start_the_stuff = 1
    startzeit = globals.second_time
  end
end
if start_the_stuff == 1 then

if globals.second_time == startzeit + 0 then
if naechste_welle == 1 then
local myosd = playmap[1].osd:get_osdbit(1000)
myosd:clear()
myosd:draw_typewriter_text(text1, 60, 2, 0)
end
end

if globals.second_time == startzeit + 2 then
if naechste_welle == 1 then
-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  1, E_SHIP_BLUE1,   87,   83,         0,            31,        810,                     4,             3,           0 )
myen.wp:add(   87,   83, 1.0, 0 )
myen.wp:add(  128,  329, 1.5, 0 )
myen.wp:add(  185,  212, 2.5, 0 )
myen:set_maxhit(8)
-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  2, E_SHIP_BLUE1,  222,  117,         0,            32,        820,                     4,             2,           4 )
myen.wp:add(  222,  117, 1.0, 0 )
myen.wp:add(  348,  102, 2.5, 0 )
myen.wp:add(  274,  229, 1.0, 0 )
myen:set_maxhit(8)
-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  3, E_SHIP_BLUE1,  340,  257,         0,            58,        830,                     5,             4,           5 )
myen.wp:add(  340,  257, 2.0, 0 )
myen.wp:add(  438,  310, 1.0, 0 )
myen.wp:add(  407,  197, 2.0, 0 )
myen:set_maxhit(8)
-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  4, E_SHIP_BLUE1,  439,   77,         0,            38,        840,                     2,             2,           0 )
myen.wp:add(  439,   77, 1.5, 0 )
myen.wp:add(  569,   65, 1.5, 0 )
myen.wp:add(  509,  145, 1.5, 0 )
myen:set_maxhit(8)
-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  5, E_SHIP_BLUE1,  556,  300,         0,            71,        850,                     5,             4,           0 )
myen.wp:add(  556,  300, 1.0, 0 )
myen.wp:add(  462,  349, 1.5, 0 )
myen.wp:add(  498,  294, 2.0, 0 )
myen.wp:add(  547,  356, 1.5, 0 )
myen:set_maxhit(8)
-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  6, E_SHIP_BLUE1,   66,  355,         0,            49,        860,                     3,             2,           0 )
myen.wp:add(   66,  355, 1.5, 0 )
myen.wp:add(  152,  379, 1.0, 0 )
myen:set_maxhit(8)
end
end

if globals.second_time == startzeit + 1 then
if naechste_welle == 2 then
local myosd = playmap[1].osd:get_osdbit(1000)
myosd:clear()
myosd:draw_typewriter_text(text2, 60, 2, 0)
end
end

if globals.second_time == startzeit + 3 then
if naechste_welle == 2 then

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  1, E_TOWER_FLAK,   97,  437,        0,        80,            60,        800,                     3.5,             3 )

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  2, E_TOWER_FLAK,  140,  434,        0,        80,            80,        800,                     3.5,             3 )

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  3, E_TOWER_FLAK,  186,  438,        0,        80,            100,        800,                     3.5,             3 )

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  4, E_TOWER_FLAK,  221,  437,        0,        80,            120,        800,                     3.5,             3 )

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  5, E_TOWER_FLAK,  457,  441,        0,        80,            120,        800,                     3.5,             3 )

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  6, E_TOWER_FLAK,  489,  437,        0,        80,            100,        800,                     3.5,             3 )

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  7, E_TOWER_FLAK,  524,  433,        0,        80,            80,        800,                     3.5,             3 )

-- E_TOWER_FLAK, Parameter:                       signatur, expl.zeit, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke
myen = enemies:add(  8, E_TOWER_FLAK,  566,  443,        0,        80,            60,        800,                     3.5,             3 )

-- E_SHIP_BLUE2, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  9, E_SHIP_BLUE2,  263,   79,         0,            40,        800,                     4,             5,           0 )
myen.wp:add(  263,   79, 1.0, 0 )
myen.wp:add(  339,   98, 1.0, 0 )

end
end

if globals.second_time == startzeit + 1 then
if naechste_welle == 3 then
local myosd = playmap[1].osd:get_osdbit(1000)
myosd:clear()
myosd:draw_typewriter_text(text3, 60, 2, 0)
end
end

if globals.second_time == startzeit + 3 then
if naechste_welle == 3 then

-- E_SHIP_BLUE2, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  1, E_SHIP_BLUE2,   72,   77,         0,            55,        800,                     2,             2,           0 )
myen.wp:add(   72,   77, 4.0, 0 )
myen.wp:add(  181,  180, 1.0, 0 )
myen.wp:add(  109,  305, 2.5, 0 )

-- E_SHIP_BLUE2, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  2, E_SHIP_BLUE2,  200,  293,         0,            50,        800,                     2,             2,           0 )
myen.wp:add(  200,  293, 1.0, 0 )
myen.wp:add(  276,  216, 1.0, 0 )
myen.wp:add(  278,  168, 1.0, 0 )

-- E_SHIP_BLUE2, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  3, E_SHIP_BLUE2,  383,   70,         0,            40,        800,                     2,             2,           0 )
myen.wp:add(  383,   70, 1.0, 0 )
myen.wp:add(  284,   85, 1.0, 0 )
myen.wp:add(  406,  128, 1.0, 0 )

-- E_SHIP_BLUE2, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  4, E_SHIP_BLUE2,  528,  189,         0,            85,        800,                     2,             2,           0 )
myen.wp:add(  528,  189, 4.0, 60 )
myen.wp:add(  441,  257, 4.0, 60 )
myen.wp:add(  509,  258, 4.0, 60 )
myen.wp:add(  536,  134, 4.0, 60 )
myen.wp:add(  475,  290, 4.0, 60 )

-- E_SHIP_BLUE2, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  5, E_SHIP_BLUE2,  523,  369,         0,            60,        800,                     2,             2,           0 )
myen.wp:add(  523,  339, 1.0, 0 )
myen.wp:add(  466,  316, 1.0, 0 )
myen.wp:add(  510,  321, 1.0, 0 )

-- E_SHIP_BLUE2, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  6, E_SHIP_BLUE2,   66,  380,         0,            75,        800,                     6,             6,           0 )
myen.wp:add(   66,  380, 1.0, 0 )
myen.wp:add(  168,  376, 1.0, 0 )
myen.wp:add(  128,  359, 1.0, 0 )

end
end






if globals.second_time == startzeit + 1 then
if naechste_welle == 4 then
local myosd = playmap[1].osd:get_osdbit(1000)
myosd:clear()
myosd:draw_typewriter_text(text4, 60, 2, 0)
end
end

if globals.second_time == startzeit + 3 then
if naechste_welle == 4 then

-- E_USER, Parameter:                     signatur,        image, [unbenutzt], [unbenutzt], [unbenutzt], [unbenutzt]
myen = enemies:add( 400, E_USER,   62,   69,      0, USER_ENEMY00,           0,           0,           0,           0 )
myen.wp:add(   65,   66, 7.0, 90 )
myen.wp:add(  603,   60, 7.0, 90 )
myen.wp:add(  573,  418, 7.0, 90 )
myen.wp:add(  457,  409, 7.0, 0 )
myen.wp:add(  328,  225, 7.0, 0 )
myen.wp:add(  212,  404, 7.0, 0 )
myen.wp:add(   55,  413, 7.0, 90 )
myen:set_maxhit(15)

-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  2, E_SHIP_BLUE1,  116,  124,         0,            33,        800,                     5,             4,           0 )
myen.wp:add(  116,  124, 5.0, 0 )
myen.wp:add(  517,  135, 3.0, 0 )

-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  3, E_SHIP_BLUE1,  519,  192,         0,            33,        800,                     5,             4,           0 )
myen.wp:add(  519,  192, 3.0, 0 )
myen.wp:add(  109,  177, 5.0, 0 )

-- E_SHIP_BLUE1, Parameter:                        signatur, feuerfrequenz, akt.radius, schussgeschwindigkeit, schussstaerke, [unbenutzt]
myen = enemies:add(  4, E_SHIP_BLUE1,  116,  230,         0,            33,        800,                     5,             4,           0 )
myen.wp:add(  116,  230, 5.0, 0 )
myen.wp:add(  519,  251, 3.0, 0 )

end
end

if naechste_welle == 5 then

    local myosd = playmap[1].osd:get_osdbit(1000)
    myosd:draw_typewriter_text(text5, 10, 2, 1)

player[1]:set_lifes(1)
naechste_welle = 6

end

if naechste_welle == 6 then
if player[1]:get_land() == 1 and player[1]:get_land_base() == 0 then

    local myosd = playmap[1].osd:get_osdbit(1000)
    myosd:set_active(0)

-- E_USER, Parameter:                     signatur,        image, [unbenutzt], [unbenutzt], [unbenutzt], [unbenutzt]
myen = enemies:add( 10, E_USER,   39,  417,      0, USER_ENEMY01,           0,           0,           0,           0 )
myen.wp:add(   39,  417, 1.0, 0 )
myen.wp:add(  332,  177, 8.0, 6000 )
myen:set_maxhit(30)
naechste_welle = 7

end
end

if globals.second_time > startzeit + 11 then
if naechste_welle == 8 then
if player[1]:get_land() == 1 then
player[1]:set_mission_status(1)
end
end
end

-- Ende der 60er-Modulation und der Variablenabfrage,
-- die Soundsache wird also wieder 60x/sec geprueft.
end
end




if sound_time > 0 then
if sound_time == globals.game_time - 120 then gsound:play_sound(SOUND_SHOOT_ROCKET, 240, 200, 80)
elseif sound_time == globals.game_time - 260 then gsound:play_sound(SOUND_SHOOT_ROCKET, 240, 200, 80)
elseif sound_time == globals.game_time - 380 then gsound:play_sound(SOUND_SHOOT_ROCKET, 240, 200, 80)
elseif sound_time == globals.game_time - 480 then gsound:play_sound(SOUND_SHOOT_ROCKET, 240, 200, 80)
elseif sound_time == globals.game_time - 550 then gsound:play_sound(SOUND_SHOOT_ROCKET, 240, 200, 80)
end
end
