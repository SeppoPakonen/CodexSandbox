// iChannel0 = Buffer A
// iChannel1 = Keyboard
// iChannel2 = rusted steel image
// iChannel3 = alpha character image

//Polygon Bridge by Kastorp
//-------------------
// SPACE BAR=  toggle simulation on/off
// mouse=  select/move nodes
// A= node mode (hold and click to add/remove) -Only unconnected nodes can be removed
// S= segment mode (hold and click to add/remove) - to add a segment click between the two nodes
// D= pin/unpin a node
// R= reset
//-------------
void mainImage( out vec4 O, in vec2 U )
{
    U=U/R.y -R.xy/R.y/2.;
    int N =int(A(0).x); 
    int M =int(B(0).x);
    
    //background
    O=vec4(0,0.5,.8,0)* (!editMode?1.+U.y*.8: .9+ .1*smoothstep(0.,.001,sin(U.x*62.8)*sin(U.y*62.8 )));
    O=mix(O,vec4(0,.8,0.,0) - texture(iChannel2,U*2.), smoothstep(-0.001,.001,min(-U.y,-U.y*.1+abs(U.x)-.498)));    
    O=mix(O,vec4(0,.2,1.,0)*(.7+.03*sin(U.y*500.+sin(U.x*50.)*5.)), smoothstep(-0.001,.001,min(-.35-U.y-sin(U.x*50.)*.01,.5+U.y*.1-abs(U.x))));
    O=mix(O,vec4(0,.8,0.,0) - texture(iChannel2,U*3.), smoothstep(-0.001,.001,min(-U.y-.3,-U.y*.1-abs(U.x)+.02)));
    
    //segments
    for(int i =1;i<= M;i++){
          vec2 l = B(i).xy;
          vec4 s =vec4(V(l.x).xy,V(l.y).xy);
          float dm = line(Mouse.xy,s.xy,s.zw),
                d = line(U,s.xy,s.zw),
                t = 1.- length(s.xy-s.zw)/length(A(l.x).xy-A(l.y).xy);          
          O=mix(O,editMode?vec4(1,1,0,0): mix(vec4(0,1,0,0),vec4(10.,0,0,0),abs(t)*2000./STIFF),smoothstep(.001,-.001,d-.005));
    }
    
    //nodes
    if(editMode) for(int i =1;i<=N;i++){
        vec4 p=V(i);
         O=mix(O, A(i).z>0.?vec4(0,0,0,0):vec4(1,0,0,0),smoothstep(.001,-.001,length(p.xy -U)-.01));
    }    
    if(editMode) drawChars(iChannel3, O.xyz, vec3(1), U, vec2(-.1,-.45), vec2(.05), int[] (80,82,69,83,83,32,83,80,65,67,69));
   
}         
