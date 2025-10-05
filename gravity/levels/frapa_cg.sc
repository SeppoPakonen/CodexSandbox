-- SC-Datei
-- Frapa CG
-- von Frapela
-- 09.02.2003

-----------------------------------------------------------------------------------------------------------
-- Platzhalter -- Platzhalter -- Platzhalter -- Platzhalter -- Platzhalter -- Platzhalter -- Platzhalter --
-----------------------------------------------------------------------------------------------------------

  local frapela_spieler2_x       = player[2]:get_x() + (player[2]:get_width()/2)
  local frapela_spieler2_y       = player[2]:get_y() + (player[2]:get_height()/2)
  local frapela_spieler2_xgeschw = player[2]:get_xspd()
  local frapela_spieler2_ygeschw = player[2]:get_yspd()
  local frapela_spieler1_x       = player[1]:get_x() + (player[1]:get_width()/2)
  local frapela_spieler1_y       = player[1]:get_y() + (player[1]:get_height()/2)
  local frapela_spieler1_xgeschw = player[1]:get_xspd()
  local frapela_spieler1_ygeschw = player[1]:get_yspd()
  local frapela_spieler_dist     = sqrt( (frapela_spieler2_x-frapela_spieler1_x)^2 + (frapela_spieler2_y-frapela_spieler1_y)^2 )

  local frapela_spobj_anz        = 0
  local frapela_alt_spobj_dist   = 600
  local frapela_alt_gegner_dist  = 600
  local frapela_spobj_nr         = {}
  local frapela_spobj_typ        = {}
  local frapela_spobj_da         = 0
  local frapela_gegner_da        = 0

  local frapela_waffenzeit, frapela_schusswinkel, frapela_multiplikator, frapela_schussanpassung
  local frapela_spobjekt, frapela_next_spobj, frapela_objekt, frapela_next_obj, frapela_enemy, frapela_next_enemy
  local frapela_neu_spobj_x, frapela_neu_spobj_y, frapela_neu_spobj_dist
  local frapela_alt_spobj_x, frapela_alt_spobj_y, frapela_alt_spobj_winkel, frapela_multi_spobj
  local frapela_obj_x1, frapela_obj_y1, frapela_obj_x2, frapela_obj_y2
  local frapela_neu_gegner_x, frapela_neu_gegner_y, frapela_neu_gegner_dist
  local frapela_alt_gegner_x, frapela_alt_gegner_y, frapela_alt_gegner_winkel
  local frapela_gegner_xgeschw, frapela_gegner_ygeschw
  local frapela_gegner_multiplikator, frapela_gegner_schussanpassung
  local frapela_geschw_multi = 45

  if schwierigkeit <= 7 then
    frapela_probe_zeit    = frapela_probe_zeit    - player[2]:get_weapon_slot(frapela_waffe_probe).reload
    frapela_rocket_zeit   = frapela_rocket_zeit   - player[2]:get_weapon_slot(frapela_waffe_rocket).reload
    frapela_double_zeit   = frapela_double_zeit   - player[2]:get_weapon_slot(frapela_waffe_double).reload
    frapela_freezer_zeit  = frapela_freezer_zeit  - player[2]:get_weapon_slot(frapela_waffe_freezer).reload
    frapela_shrinker_zeit = frapela_shrinker_zeit - player[2]:get_weapon_slot(frapela_waffe_shrinker).reload
    frapela_mine_zeit     = frapela_mine_zeit     - player[2]:get_weapon_slot(frapela_waffe_mine).reload
    frapela_gas_zeit      = frapela_gas_zeit      - player[2]:get_weapon_slot(frapela_waffe_gas).reload
  end

---------------------------------------------------------------------------------------------------
-- PROGRAMM -- PROGRAMM -- PROGRAMM -- PROGRAMM -- PROGRAMM -- PROGRAMM -- PROGRAMM -- PROGRAMM ---
---------------------------------------------------------------------------------------------------

if player[2]:get_freezed() == 0 then

if frapela_wandzeit + frapela_zufi_wand <= globals.game_time then

  if player[2]:get_fuel() < 1 then
    player[2]:set_fuel( player[2]:get_fuel() + 99 )
  end

  frapela_waffenzeit = frapela_spieler_dist / (schwierigkeit * 2 - 2)
  if (frapela_waffenzeit < 1) then 
    frapela_waffenzeit = 1 
  end

  if mod(globals.game_time, frapela_waffenzeit) == 0 then
    if     (frapela_waffe_probe >= 0) and (player[2]:get_weapon_slot(frapela_waffe_probe).bullets ~= 0) and (frapela_probe_zeit + player[2]:get_weapon_slot(frapela_waffe_probe).reload < globals.game_time) then
      frapela_waffenschacht = frapela_waffe_probe
      frapela_probe_zeit    = globals.game_time
    elseif (frapela_waffe_rocket >= 0) and (player[2]:get_weapon_slot(frapela_waffe_rocket).bullets ~= 0) and (frapela_rocket_zeit + player[2]:get_weapon_slot(frapela_waffe_rocket).reload < globals.game_time) then
      frapela_waffenschacht = frapela_waffe_rocket
      frapela_rocket_zeit   = globals.game_time
    elseif (frapela_waffe_double >= 0) and (player[2]:get_weapon_slot(frapela_waffe_double).bullets ~= 0) and (frapela_double_zeit + player[2]:get_weapon_slot(frapela_waffe_double).reload < globals.game_time) then
      frapela_waffenschacht = frapela_waffe_double
      frapela_double_zeit   = globals.game_time
    elseif (frapela_waffe_freezer >= 0) and (player[2]:get_weapon_slot(frapela_waffe_freezer).bullets ~= 0) and (frapela_freezer_zeit + player[2]:get_weapon_slot(frapela_waffe_freezer).reload < globals.game_time) then
      frapela_waffenschacht = frapela_waffe_freezer
      frapela_freezer_zeit  = globals.game_time
    elseif (frapela_waffe_shrinker >= 0) and (player[2]:get_weapon_slot(frapela_waffe_shrinker).bullets ~= 0) and (frapela_shrinker_zeit + player[2]:get_weapon_slot(frapela_waffe_shrinker).reload < globals.game_time) then
      frapela_waffenschacht = frapela_waffe_shrinker
      frapela_shrinker_zeit = globals.game_time
    elseif (frapela_waffe_mine >= 0) and (player[2]:get_weapon_slot(frapela_waffe_mine).bullets ~= 0) and (frapela_mine_zeit + player[2]:get_weapon_slot(frapela_waffe_mine).reload < globals.game_time) then
      frapela_waffenschacht = frapela_waffe_mine
      frapela_mine_zeit     = globals.game_time
    --elseif (frapela_waffe_bomb >= 0) and (player[2]:get_weapon_slot(frapela_waffe_bomb).bullets ~= 0) then
    --frapela_waffenschacht = frapela_waffe_bomb
    --elseif (frapela_waffe_bigbomb >= 0) and (player[2]:get_weapon_slot(frapela_waffe_bigbomb).bullets ~= 0) then
    --frapela_waffenschacht = frapela_waffe_bigbomb
    elseif (frapela_waffe_gas >= 0) and (player[2]:get_weapon_slot(frapela_waffe_gas).bullets ~= 0) and (frapela_gas_zeit + player[2]:get_weapon_slot(frapela_waffe_gas).reload < globals.game_time) then
      frapela_waffenschacht = frapela_waffe_gas
      frapela_gas_zeit      = globals.game_time
    else
      if frapela_waffe_single  >= 0 then
        frapela_waffenschacht = frapela_waffe_single
      else
        frapela_waffenschacht = random(frapela_waffenanzahl_sp2) - 1
      end
    end
    if frapela_waffe_bigbomb >= 0 then
      if player[2]:get_weapon_slot(frapela_waffe_bigbomb).bullets ~= 0 then
        if frapela_spieler2_y - frapela_spieler1_y < 0 then
          if   frapela_spieler2_x - ( (frapela_spieler1_y - frapela_spieler2_y) * ((frapela_spieler1_xgeschw - frapela_spieler2_xgeschw)/2) ) <= frapela_spieler1_x + 10 then
            if frapela_spieler2_x - ( (frapela_spieler1_y - frapela_spieler2_y) * ((frapela_spieler1_xgeschw - frapela_spieler2_xgeschw)/2) ) >= frapela_spieler1_x - 10 then
              frapela_waffenschacht = frapela_waffe_bigbomb
            end
          end
        end
      end
    end
    if frapela_spieler_dist < 100 then
      if frapela_waffe_bomb >= 0 then
        if player[2]:get_weapon_slot(frapela_waffe_bomb).bullets ~= 0 then
          if frapela_bomb_zeit + player[2]:get_weapon_slot(frapela_waffe_bomb).reload < globals.game_time then
            frapela_bomb_zeit = globals.game_time
            frapela_waffenschacht = frapela_waffe_bomb
          end
        end
      end
    end
    if (player[1]:get_freezed() > 0) or (player[1]:get_shrinked() > 0) then
      if frapela_waffe_single  >= 0 then
        frapela_waffenschacht = frapela_waffe_single
      end
    end
    player[2]:set_active_weapon( frapela_waffenschacht )
  end

  frapela_schusswinkel = (asin( (frapela_spieler2_x-frapela_spieler1_x)/(frapela_spieler_dist) ))
  if abs(frapela_schusswinkel) < 45 then
    frapela_multiplikator = abs(frapela_schusswinkel) / 6
  else
    frapela_multiplikator = abs((abs(frapela_schusswinkel)-90)) / 6
  end
  if ( frapela_spieler_dist > 50 ) and ( player[2]:get_shrinked() == 0 ) then
    if frapela_spieler2_y - frapela_spieler1_y < 0 then
      if frapela_spieler2_x - frapela_spieler1_x < 0 then
        frapela_schussanpassung = (frapela_multiplikator * ((-1)*frapela_spieler1_xgeschw + frapela_spieler2_xgeschw)) + (frapela_multiplikator * (frapela_spieler1_ygeschw - frapela_spieler2_ygeschw))
      else
        frapela_schussanpassung = (frapela_multiplikator * ((-1)*frapela_spieler1_xgeschw + frapela_spieler2_xgeschw)) + (frapela_multiplikator * (frapela_spieler2_ygeschw - frapela_spieler1_ygeschw))
      end
      player[2]:set_head( 180 + frapela_schusswinkel + frapela_schussanpassung)
    else
      if frapela_spieler2_x - frapela_spieler1_x < 0 then
        frapela_schussanpassung = (frapela_multiplikator * (frapela_spieler1_xgeschw + ((-1)*frapela_spieler2_xgeschw))) + (frapela_multiplikator * (frapela_spieler1_ygeschw - frapela_spieler2_ygeschw))
      else
        frapela_schussanpassung = (frapela_multiplikator * ((-1)*frapela_spieler2_xgeschw + frapela_spieler1_xgeschw)) + (frapela_multiplikator * ((-1)*frapela_spieler1_ygeschw + frapela_spieler2_ygeschw))
      end
      if 360 - frapela_schusswinkel + frapela_schussanpassung <= 360 then
        player[2]:set_head( 360 - frapela_schusswinkel + frapela_schussanpassung )
      else
        player[2]:set_head(   0 - frapela_schusswinkel + frapela_schussanpassung )
      end
    end
    if frapela_spieler_dist < 600 then
      frapela_aktion_zeit = globals.game_time
      if mod(globals.game_time, frapela_waffenzeit) == 0 then
        if (player[1]:get_dead() == 0) and (player[2]:get_dead() == 0) then
          player[2]:shoot()
        end
      end
    end
  elseif ( player[1]:get_shrinked() == 0 ) or ( player[2]:get_shrinked() > 0 ) then
    if frapela_spieler2_y - frapela_spieler1_y < 0 then
      player[2]:set_head( frapela_schusswinkel )
      if frapela_spieler_dist < 130 then
        player[2]:set_thrust()
      end
    else
      player[2]:set_head( 180 - frapela_schusswinkel )
      if frapela_spieler_dist < 130 then
        player[2]:set_thrust()
      end
    end
  end
  if random() < 0.2 then
    player[2]:set_thrust()
  elseif player[1]:get_shrinked() > 0 then
    if random() < 0.8 then
      player[2]:set_thrust()
    end
  end

  frapela_spobjekt = objects:get_first_spobject()
  while (frapela_spobjekt) do 
    frapela_next_spobj = frapela_spobjekt:get_next()
    frapela_spobj_anz = frapela_spobj_anz + 1
    frapela_spobj_nr[frapela_spobj_anz]  = frapela_spobjekt:get_nr()
    frapela_spobj_typ[frapela_spobj_anz] = frapela_spobjekt:get_type()
    if (frapela_spobj_typ[frapela_spobj_anz] == 505) or (frapela_spobj_typ[frapela_spobj_anz] == 0) then
      frapela_neu_spobj_x      = frapela_spobjekt:get_x() + (frapela_spobjekt:get_width()/2)
      frapela_neu_spobj_y      = frapela_spobjekt:get_y() + (frapela_spobjekt:get_height()/2)
      frapela_neu_spobj_dist   = sqrt( (frapela_spieler2_x-frapela_neu_spobj_x)^2 + (frapela_spieler2_y-frapela_neu_spobj_y)^2 )
      if frapela_neu_spobj_dist < frapela_alt_spobj_dist then
        if frapela_spobj_typ[frapela_spobj_anz] == 505 then
          frapela_spobj_da       = 505
        else
          frapela_spobj_da       =   1
        end
        frapela_alt_spobj_x      = frapela_neu_spobj_x
        frapela_alt_spobj_y      = frapela_neu_spobj_y
        frapela_alt_spobj_dist   = frapela_neu_spobj_dist
        frapela_alt_spobj_winkel = (asin( (frapela_spieler2_x-frapela_alt_spobj_x)/(frapela_alt_spobj_dist) ))
      end
    end
    frapela_spobjekt = frapela_next_spobj
  end
  if ( frapela_spobj_da == 0 ) and ( frapela_spieler_dist > 600 ) then
    frapela_spobjekt = objects:get_first_spobject()
    while (frapela_spobjekt) do 
      frapela_next_spobj = frapela_spobjekt:get_next()
      frapela_spobj_anz = frapela_spobj_anz + 1
      frapela_spobj_nr[frapela_spobj_anz]  = frapela_spobjekt:get_nr()
      frapela_spobj_typ[frapela_spobj_anz] = frapela_spobjekt:get_type()
        if (frapela_spobj_typ[frapela_spobj_anz] == 506) then
          frapela_neu_spobj_x      = frapela_spobjekt:get_x() + (frapela_spobjekt:get_width()/2)
          frapela_neu_spobj_y      = frapela_spobjekt:get_y() + (frapela_spobjekt:get_height()/2)
          frapela_neu_spobj_dist   = sqrt( (frapela_spieler2_x-frapela_neu_spobj_x)^2 + (frapela_spieler2_y-frapela_neu_spobj_y)^2 )
          if frapela_neu_spobj_dist < frapela_alt_spobj_dist then
            frapela_spobj_da         = 506
            frapela_alt_spobj_x      = frapela_neu_spobj_x
            frapela_alt_spobj_y      = frapela_neu_spobj_y
            frapela_alt_spobj_dist   = frapela_neu_spobj_dist
            frapela_alt_spobj_winkel = (asin( (frapela_spieler2_x-frapela_alt_spobj_x)/(frapela_alt_spobj_dist) ))
          end
        end
      frapela_spobjekt = frapela_next_spobj
    end
  end
  if frapela_alt_spobj_dist < 600 then
    if abs(frapela_alt_spobj_winkel) < 45 then
      frapela_multi_spobj = abs(frapela_alt_spobj_winkel) * ( (600-frapela_alt_spobj_dist)/800 )
      --frapela_multi_spobj = ( (abs(frapela_alt_spobj_winkel)^2)/45 ) * ( (600-frapela_alt_spobj_dist)/800 )
    else
      frapela_multi_spobj = abs( (abs(frapela_alt_spobj_winkel) - 90) ) * ( (600-frapela_alt_spobj_dist)/800 )
      --frapela_multi_spobj = ( (abs( (abs(frapela_alt_spobj_winkel) - 90) )^2)/45 ) * ( (600-frapela_alt_spobj_dist)/800 )
    end
    if frapela_spieler2_y - frapela_alt_spobj_y < 0 then
      if frapela_spieler2_x - frapela_alt_spobj_x < 0 then
        frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) - (frapela_multi_spobj*frapela_spieler2_ygeschw)
      else
        frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) + (frapela_multi_spobj*frapela_spieler2_ygeschw)
      end
      player[2]:set_head( 180 + frapela_alt_spobj_winkel )
    else
      if frapela_spieler2_x - frapela_alt_spobj_x < 0 then
        frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) + (frapela_multi_spobj*frapela_spieler2_ygeschw)
      else
        frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) - (frapela_multi_spobj*frapela_spieler2_ygeschw)
      end
      if frapela_alt_spobj_winkel > 0 then
        player[2]:set_head( 360 - frapela_alt_spobj_winkel )
      else
        player[2]:set_head( 0   - frapela_alt_spobj_winkel )
      end
    end
    if random() < 0.6 then
      player[2]:set_thrust()
    end
  end
  frapela_spobj_da_zufi = frapela_spobj_da

  frapela_enemy = enemies:get_first_enemy()
  while (frapela_enemy) do 
    frapela_next_enemy = frapela_enemy:get_next()
    frapela_neu_gegner_x    = frapela_enemy:get_x() + (frapela_enemy:get_width()/2)
    frapela_neu_gegner_y    = frapela_enemy:get_y() + (frapela_enemy:get_height()/2)
    frapela_neu_gegner_dist = sqrt( (frapela_spieler2_x-frapela_neu_gegner_x)^2 + (frapela_spieler2_y-frapela_neu_gegner_y)^2 )
    if frapela_neu_gegner_dist < frapela_alt_gegner_dist then
      frapela_gegner_da         = 1
      frapela_alt_gegner_x      = frapela_neu_gegner_x
      frapela_alt_gegner_y      = frapela_neu_gegner_y
      frapela_alt_gegner_dist   = frapela_neu_gegner_dist
      frapela_alt_gegner_winkel = (asin( (frapela_spieler2_x-frapela_alt_gegner_x)/(frapela_alt_gegner_dist) ))
      frapela_gegner_xgeschw    = frapela_enemy:get_xspd()
      frapela_gegner_ygeschw    = frapela_enemy:get_yspd()
    end
    frapela_enemy = frapela_next_enemy
  end
  if frapela_alt_gegner_dist < 600 then
    if frapela_alt_gegner_dist < 50 + ( ( abs(frapela_spieler2_xgeschw + frapela_spieler2_ygeschw) ) * 50 ) then
      if frapela_spieler2_y - frapela_alt_gegner_y < 0 then
        player[2]:set_head( frapela_alt_gegner_winkel )
        frapela_wandzeit = globals.game_time
        frapela_zufi_wand = 20 + random(10)
      else
        player[2]:set_head( 180 - frapela_alt_gegner_winkel )
        frapela_wandzeit = globals.game_time
        frapela_zufi_wand = 10 + random(10)
      end
    elseif frapela_alt_gegner_dist < 600 then
      if frapela_spieler_dist > 600 then
        if frapela_alt_spobj_dist == 600 then
          if frapela_waffe_single  >= 0 then
            player[2]:set_active_weapon( frapela_waffe_single )
          end
          if abs( frapela_alt_gegner_winkel ) <= 45 then
            frapela_gegner_multiplikator = 9
          else
            frapela_gegner_multiplikator = 9
          end
          if frapela_spieler2_y - frapela_alt_gegner_y < 0 then
            if frapela_spieler2_x - frapela_alt_gegner_x < 0 then
              frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * ((-1)*frapela_gegner_xgeschw + frapela_spieler2_xgeschw)) + (frapela_gegner_multiplikator * (frapela_gegner_ygeschw - frapela_spieler2_ygeschw))
            else
              frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * ((-1)*frapela_gegner_xgeschw + frapela_spieler2_xgeschw)) + (frapela_gegner_multiplikator * (frapela_spieler2_ygeschw - frapela_gegner_ygeschw))
            end
            player[2]:set_head( 180 + frapela_alt_gegner_winkel + frapela_gegner_schussanpassung)
          else
            if frapela_spieler2_x - frapela_alt_gegner_x < 0 then
              frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * (frapela_gegner_xgeschw + ((-1)*frapela_spieler2_xgeschw))) + (frapela_gegner_multiplikator * (frapela_gegner_ygeschw - frapela_spieler2_ygeschw))
            else
              frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * ((-1)*frapela_spieler2_xgeschw + frapela_gegner_xgeschw)) + (frapela_gegner_multiplikator * ((-1)*frapela_gegner_ygeschw + frapela_spieler2_ygeschw))
            end
            if 360 - frapela_alt_gegner_winkel + frapela_gegner_schussanpassung <= 360 then
              player[2]:set_head( 360 - frapela_alt_gegner_winkel + frapela_gegner_schussanpassung )
            else
              player[2]:set_head(   0 - frapela_alt_gegner_winkel + frapela_gegner_schussanpassung )
            end
          end
          --if random() < 0.2 then
          --  player[2]:set_thrust()
          --end
          if mod(globals.game_time, 8) == 0 then
            if frapela_alt_gegner_dist < 200 then
              frapela_aktion_zeit = globals.game_time
              player[2]:shoot()
            end
          end
        end
      end
    end
  end
  frapela_gegner_da_zufi = frapela_gegner_da

  frapela_objekt = objects:get_first_object()
  while (frapela_objekt) do
    frapela_next_obj = frapela_objekt:get_next()
    frapela_obj_x1   = frapela_objekt:get_x()
    frapela_obj_y1   = frapela_objekt:get_y()
    if sqrt( (frapela_spieler2_x-frapela_obj_x1)^2 + (frapela_spieler2_y-frapela_obj_y1)^2 ) < 300 then
      frapela_obj_x2 = frapela_objekt:get_x() + frapela_objekt:get_width()
      frapela_obj_y2 = frapela_objekt:get_y() + frapela_objekt:get_height()
      if     ( frapela_obj_x2 < frapela_spieler2_x ) and ( frapela_obj_y1 > frapela_spieler2_y ) and ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) < frapela_obj_x2 ) and ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) > frapela_obj_y1 ) then
          player[2]:set_head( 45)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 20 + random(20)
      elseif ( frapela_obj_x1 > frapela_spieler2_x ) and ( frapela_obj_y1 > frapela_spieler2_y ) and ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) > frapela_obj_x1 ) and ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) > frapela_obj_y1 ) then
          player[2]:set_head(315)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 20 + random(20)
      elseif ( frapela_obj_x2 < frapela_spieler2_x ) and ( frapela_obj_y2 < frapela_spieler2_y ) and ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) < frapela_obj_x2 ) and ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) < frapela_obj_y2 ) then
          player[2]:set_head(135)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 10 + random(10)
      elseif ( frapela_obj_x1 > frapela_spieler2_x ) and ( frapela_obj_y2 < frapela_spieler2_y ) and ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) > frapela_obj_x1 ) and ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) < frapela_obj_y2 ) then
          player[2]:set_head(225)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 10 + random(10)
      elseif ( frapela_obj_x1 < frapela_spieler2_x ) and ( frapela_obj_x2 > frapela_spieler2_x ) and ( frapela_obj_y1 > frapela_spieler2_y ) and ( frapela_obj_y1 < (frapela_spieler2_y+3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) then
          player[2]:set_head(  0)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 20 + random(30)
      elseif ( frapela_obj_y1 < frapela_spieler2_y ) and ( frapela_obj_y2 > frapela_spieler2_y ) and ( frapela_obj_x2 < frapela_spieler2_x ) and ( frapela_obj_x2 > (frapela_spieler2_x-3) + (frapela_spieler2_xgeschw*frapela_geschw_multi) ) then
          player[2]:set_head( 90)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 10 + random(20)
      elseif ( frapela_obj_y1 < frapela_spieler2_y ) and ( frapela_obj_y2 > frapela_spieler2_y ) and ( frapela_obj_x1 > frapela_spieler2_x ) and ( frapela_obj_x1 < (frapela_spieler2_x+3) + (frapela_spieler2_xgeschw*frapela_geschw_multi) ) then
          player[2]:set_head(270)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 10 + random(20)
      elseif ( frapela_obj_x1 < frapela_spieler2_x ) and ( frapela_obj_x2 > frapela_spieler2_x ) and ( frapela_obj_y2 < frapela_spieler2_y ) and ( frapela_obj_y2 > (frapela_spieler2_y-3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) then
          player[2]:set_head(180)
          frapela_wandzeit = globals.game_time
          frapela_zufi_wand = 10 + random(7)
      end
    end
    frapela_objekt = frapela_next_obj
  end

end

  local frapela_u     = 0
  local frapela_ul    = 0
  local frapela_l     = 0
  local frapela_ol    = 0
  local frapela_o     = 0
  local frapela_or    = 0
  local frapela_r     = 0
  local frapela_ur    = 0
  local frapela_ul_ol = 0
  local frapela_ol_or = 0
  local frapela_or_ur = 0
  local frapela_ur_ul = 0
  if     ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x, (frapela_spieler2_y+3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_ygeschw > 0 ) then
    frapela_u  = 1
  elseif ( playmap[frapela_playmap]:is_pixel( (frapela_spieler2_x-3) + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y) == 1 ) and ( frapela_spieler2_xgeschw < 0 ) then
    frapela_l  = 1
  elseif ( playmap[frapela_playmap]:is_pixel( (frapela_spieler2_x+3) + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y) == 1 ) and ( frapela_spieler2_xgeschw > 0 ) then
    frapela_r  = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x, (frapela_spieler2_y-3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_ygeschw < 0 ) then
    frapela_o  = 1
  end
  if     ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_ygeschw > 0 ) then
    frapela_ur_ul = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_xgeschw < 0 ) then
    frapela_ul_ol = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_xgeschw > 0 ) then
    frapela_or_ur = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_ygeschw < 0 ) then
    frapela_ol_or = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_xgeschw < 0 ) and ( frapela_spieler2_ygeschw > 0 ) then
    frapela_ul = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_xgeschw > 0 ) and ( frapela_spieler2_ygeschw > 0 ) then
    frapela_ur = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_xgeschw < 0 ) and ( frapela_spieler2_ygeschw < 0 ) then
    frapela_ol = 1
  elseif ( playmap[frapela_playmap]:is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) and ( frapela_spieler2_xgeschw > 0 ) and ( frapela_spieler2_ygeschw < 0 ) then
    frapela_or = 1
  end
  if     ( frapela_ur_ul == 1 ) and ( frapela_l + frapela_r == 0 ) then
    if random() < 0.5 then
      player[2]:set_head( random(5) )
    else
      player[2]:set_head(360 - random(5))
    end
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(40)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif ( frapela_ul_ol == 1 ) and ( frapela_u + frapela_o == 0 ) then
    player[2]:set_head( 95 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(20)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif ( frapela_or_ur == 1 ) and ( frapela_u + frapela_o == 0 ) then
    player[2]:set_head(275 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(20)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif ( frapela_ol_or == 1 ) and ( frapela_l + frapela_r == 0 ) then
    player[2]:set_head(185 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 10 + random(10)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_ul == 1 then
    player[2]:set_head( 50 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(30)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_ur == 1 then
    player[2]:set_head(320 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(30)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_ol == 1 then
    player[2]:set_head(140 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 10 + random(20)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_or == 1 then
    player[2]:set_head(230 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 10 + random(20)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_u  == 1 then
    if random() < 0.5 then
      player[2]:set_head( random(5) )
    else
      player[2]:set_head(360 - random(5))
    end
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(40)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_l  == 1 then
    player[2]:set_head( 95 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(20)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_r  == 1 then
    player[2]:set_head(275 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 20 + random(20)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  elseif frapela_o  == 1 then
    player[2]:set_head(185 - random(10))
    frapela_wandzeit  = globals.game_time
    if frapela_aktion_zeit + 180 > globals.game_time then 
      frapela_zufi_wand = 10 + random(10)
    else
      frapela_aktion_zeit = globals.game_time - 120
      frapela_zufi_wand = 120
    end
  end
  if     frapela_spobj_da_zufi == 505 then
    if random() < 0.998 then
      frapela_zufi_wand     = 10
    else
      frapela_spobj_da_zufi = 0
      frapela_zufi_wand     = 180
    end
  elseif frapela_spobj_da_zufi == 506 then
    if random() < 0.998 then
      frapela_zufi_wand     = 10
    else
      if random() < 0.3 then
        frapela_wandzeit    = globals.game_time
      end
      frapela_spobj_da_zufi = 0
      frapela_zufi_wand     = 180
    end
  end
  if frapela_wandzeit + frapela_zufi_wand > globals.game_time then
    player[2]:set_thrust()
  end

end