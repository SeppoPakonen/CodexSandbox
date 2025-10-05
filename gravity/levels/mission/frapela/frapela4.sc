-- Frapela4
-- level script
-- von Frapela und Frapela sin Fru als gute Zuhörerin
-- 24.11.2002

-- eine saudähmliche Art Variabelen zu deklarieren, ich weiß, aber ging nicht anders.
-- Wenn doch, bin für jede Hilfe dankbar.
-- Wieso kann ich in der INI-Datei "aok" nicht Null ("0") zuweisen??? aok = 0
-- aok ist und bleibt nil ("nil").
if aok == nil then
-- aok = 0
-- okdist = 0
end

check_respawning_extras()

-- Führung von Agent
-- wenn Gegner Nr. 1 lebt
if enemies:get_enemy(1) then
x = player[1]:get_x()
y = player[1]:get_y()
agentx = agent:get_x()
agenty = agent:get_y()
 agentdist = sqrt( (x-agentx)^2 + (y-agenty)^2 )
 -- wenn Distanz (Spieler <-> Gegner) kleiner 300 Pixel, dann Gegner-Verfolgung
 if agentdist <= 300 then
  aok = 1
  -- wo ist die Wand?
  local s, t
  for s = agenty-2, agenty+25, 27 do
   for t = agentx-2, agentx+44 do
    if playmap[1]:is_pixel(t, s) == 1 then
     hilfsvar = 1
     s = agenty+25
     t = agentx+44
    end
   end
  end
  if hilfsvar == 0 then
   for s = agentx-2, agentx+44, 46 do
    for t = agenty-2, agenty+25 do
     if playmap[1]:is_pixel(s, t) == 1 then
      hilfsvar = 1
      s = agentx+44
      t = agenty+25
     end
    end
   end
  end
  -- wenn keine Wand, dann bewegen 
  if hilfsvar == 0 then
   xalt = agentx
   yalt = agenty
   mywp1 = agent.wp:get(0)
   mywp1:set_x( x )
   mywp1:set_y( y )
   mywp1:set_speed(1.0)
   agent.wp:set_current(0)
   -- ansonsten, wenn Wand, dann anhalten
  else
   agentx = xalt
   agenty = yalt
   hilfsvar = 0
   mywp1 = agent.wp:get(0)
   mywp1:set_x( agentx )
   mywp1:set_y( agenty )
   mywp1:set_speed(1.0)
   agent.wp:set_current(0)
  end
 -- ansonsten, wenn außerhalb 300 Pixel anhalten
 else
  mywp1 = agent.wp:get(0)
  mywp1:set_x( agentx )
  mywp1:set_y( agenty )
  mywp1:set_speed(1.0)
  agent.wp:set_current(0)
  if aok == 1 then
    aok = 0
    okdist = okdist + 1
    if koll == 1 then
      koll = 0
      local agentwegosd = playmap[1].osd:get_osdbit(1000)   
      if (LANGUAGE == LANG_DE) then
        agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] " .. player[1]:get_name() .. ", du Blindnase! Hast du keine Augen im Kopf??? Am Besten du begibst dich nochmal zur Mars Mission V und trainierst ein wenig!", 

20, 4, 1)
      else
        agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Hey, " .. player[1]:get_name() .. "! Do you want to kill us??? Pay attention!!!", 30, 4, 1)
      end
    else
      local agentwegosd = playmap[1].osd:get_osdbit(1000)   
      if okdist == 1 then
        if (LANGUAGE == LANG_DE) then
          agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Hey, " .. player[1]:get_name() .. "! Wo bist du hin? Ich habe den Sichtkontakt zu dir verloren. Komm sofort zurueck!", 30, 4, 1)
        else
          agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Hey, " .. player[1]:get_name() .. "! Where are you! I can not see you. Come back and help me!", 30, 4, 1)
        end
      elseif okdist == 2 then
        if (LANGUAGE == LANG_DE) then
          agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Wo bist du jetzt schon wieder? Wie oft muss ich dir noch sagen, dass ich auf Sichtkontakt angewiesen bin?", 30, 4, 1)
        else
          agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Where are you now? Do you not understand, that I must see you?", 30, 4, 1)
        end
      elseif okdist == 3 then
       if (LANGUAGE == LANG_DE) then
          agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Ok " .. player[1]:get_name() .. ", du kannst oder willst es nicht begreifen. Ich sage da jetzt nichts mehr zu!", 30, 4, 1)
        else
          agentwegosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Ok " .. player[1]:get_name() .. ", you can not or you do not want to understand. By the way, I do not talk annymore!", 30, 4, 1)
        end    
      end
    end
  end
 end
if (agentx < 1186) and (agenty < 883) then
player[1]:set_mission_status(1)
end
else
player[1]:set_mission_status(-1)
end



-- Hätte ich auch gerne im Script gehabt, aber es gab wieder Schwierigkeiten mit der Variabelen-
-- Deklaration. Diesmal hat okdist Werte angenommen, die sie nicht haben dürfte. Dieses Problem
-- habe ich allerdings nicht ausgibig geprüft.
-- Die untenstehenden Zeilen sollten stehen unter (wenn der Spieler wieder in Reichweite kommt):
--  -- wenn Distanz (Spieler <-> Gegner) kleiner 300 Pixel, dann Gegner-Verfolgung
--  if agentdist <= 300 then
--    aok = 1
--    ...     
--    if okdist == 1 then
--      local agentdaosd = playmap[1].osd:get_osdbit(1000)
--      if (LANGUAGE == LANG_DE) then
--        agentdaosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Hi " .. player[1]:get_name() .. ", auch mal wieder in der Gegend, schoen dich zu sehen?", 30, 4, 1)
--      else
--        agentdaosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Hi " .. player[1]:get_name() .. ", you here?", 30, 4, 1)
--      end
--    elseif okdist == 2 then
--      local agentdaosd = playmap[1].osd:get_osdbit(1000)
--      if (LANGUAGE == LANG_DE) then
--        agentdaosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Ach nee " .. player[1]:get_name() .. ", du schon wieder? Lass am Besten ein Passfoto hier, damit ich dich wenigstens darauf sehen kann!", 30, 4, 1)
--      else
--        agentdaosd:draw_typewriter_text("[center][f:impact10][c:white]Spezial-Agent:[c:yellow] Oh " .. player[1]:get_name() .. ", you again? Please give me your foto by that I can see you at least?", 30, 4, 1)
--      end  
--    elseif oksist > nil then
--      player[1]:set_mission_status(1)
--    end
--    ...