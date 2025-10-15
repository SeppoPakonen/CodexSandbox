/*
 *   GRAVITY STRIKE -- gslvlnw.h
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSNETLEVEL_
#define _GSNETLEVEL_

#include "gsosd.h"

class gsNetLevel
{
  private:
    char error_str[100];
    int  fading_in;                                // fade in in progress?
    int  exit_level;                               // exit level on end...
    bool client_quit;                              // if client quit game
    int  replay_mode;
    int  cur_send_enemy;                           // current enemy to send (number in linked list)
    int  cur_send_object;                          // current object to send
    int  cur_send_spobject;                        // current special object to send

    OsdBitmap *osd_chat_input;                     // osd for writing chat messages
    OsdBitmap *osd_chat_messages;                  // osd with chat messages
    char chat_input_text[100];                     // current chat text
    
    // functions
    void init();                                   // initializes players & other things
    void flip();                                   // flips vscreen to screen
    void do_logic();                               // does game logic
    void draw_stuff();                             // draws graphics
    void clear_stuff();                            // clears graphics
    
    void do_network_stuff();                       // network updates
    void update_chat_osd();                        // updates chat input osd
    void update_osd_chat_input();                  // graphically updates chat input osd
    void update_osd_chat_messages();               // graphically updates chat messages osd

  public:
    // constr. / destr.
    gsNetLevel();                                     // create the level
    ~gsNetLevel();                                    // ...and destroy it

    // get/set
    char *get_error_str()                          { return error_str; }

    // functions
    int  init_stuff();                             // init level
    void play();                
};

#endif
