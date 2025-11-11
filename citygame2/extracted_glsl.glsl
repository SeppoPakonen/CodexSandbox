// --- Renderpass 1 ---
// City Game  II by KASTORP
// Fork of "City Game" by kastorp. https://shadertoy.com/view/3d3fR4
// 2020-11-12 14:49:09
/*------------------------------------------------------

the city needs your help!
- connects all road to the center (yellow square)
- connect all buildings to roads
- convert congested semaphores & overpass into roundabouts
- let all taxis (yellow cars) reach the center

Unconnected roads/building are darker
The overall % of connected is shown up/right
The counter of remaining taxis is shown uu/right 

block types and actions:
   ROAD: add road
   SEMAphore:  use with care! if 4 connections --> overpass
   ROUNdabout: best solution for crossing
   BUILding: add skyscraper
   GRASs: clear block 

Controls:
   click on block type: select 
   click or drag on map: add selected block 
   CAPS-LOCK:  view mode / game mode
   click + SHIFT or CTRL: special actions for selected block type
		ROAD: +SHIFT: disabile auto roundabout, +CTRL= remove single connection between blocks
		GRASS: +SHIFT=increase altitude, +CTRL=decrease altitude
   SPACE BAR: reset map and show demo mode
   mouse in right panel: zoom & rotate view
   Z: switch 2D/isometric view in right panel (2D is faster)

other features:
  multiple lanes: set LANES 2 in common file (requires >20s compilation)
  set CITY_SIZE to 100 to see mega city

todo features:
   rendering: more decorations
   vehicles: pathfinding with overpass

The game is tested only on windows + chrome + Angle 
(sorry I can't help with compatibility issues)
------------------------------------------------------*/

#define GCOL(th) (th +float(THEIGHT)*.3 )/float(THEIGHT)/1.3*(.8+.2*hash(P0*4.))

vec4 char(vec2 p, int C) {
    if (p.x<0.|| p.x>1. || p.y<0.|| p.y>1.) return vec4(0,0,0,1e5);
    return textureGrad( iChannel2, p/16. + fract( vec2(C, 15-C/16) / 16. ) , 
                       dFdx(p/16.),dFdy(p/16.) );
}

// --- display int4
vec4 pInt(float d, vec2 p, float n) {
    vec4 v = vec4(0);
    if (n < 0.) 
        v += char(p - vec2(-.5,0), 45 ),
        n = -n;

    for (float i = d-1.; i>=0.; i--) 
        n /= 10.,
        v += char(p - vec2(.5*i,0), 48+ int(fract(n)*10.) );
    return v;
}

#define pString(p,st)  for(int _i= st.length()-1 ;_i>=0;_i-- ) Q+= char(p -vec2(.5*float(_i),0.), st[_i]).x; 
#define pStringC(p,st,c) for(int _i= st.length()-1 ;_i>=0;_i-- ) Q+= c* char(p -vec2(.5*float(_i),0.), st[_i]).x; 

mat2 rot(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat2(c,s,-s,c);
}

vec2 rpos(int ni,int no, float off, int di, int dout){
    #if LANES >1
    float ki= ni==1? -.5:.5,  ko= no==1? -.5:.5 ;
    #else    
    float ki=0.,ko=0.;
    #endif
    vec2 pOut=DIRS[dout]+ ko*DIRS[(dout+1)%4];
    vec2 pIn=DIRS[di]+ki*DIRS[(di+3)%4];

    return mix(
        //45° path
        .5 + mix(pIn, pOut,.5+off)*.5,
         
        //90° path
        (off<0.? .5- pIn*off: .5+ pOut*off),
        1. -float(LANES)/2.);
}

float vdist(vec2 p, vehicle nv,bool top){
    
    #if LANES >1
    	int lane_out=nv.lane_out;
    #else
        int lane_out=nv.lane;
    #endif
    vec2 dir = normalize(rpos(nv.lane,lane_out, nv.offset+.5,nv.dir_in,nv.dir_out)-rpos(nv.lane,lane_out,nv.offset-.5,nv.dir_in,nv.dir_out));
    vec2 pos=p-rpos(nv.lane,lane_out,nv.offset,nv.dir_in,nv.dir_out);
    pos= vec2(pos.x* dir.x + pos.y*dir.y , pos.y*dir.x-pos.x*dir.y);
    return sdBox(pos,vec2(top? 0.2: 0.3 ,0.2));
}

vec2 position(vec2 U, float j){
    //smooth position change 
    float 
        rz= CONF(vec2(3.,0.)).z
        ,zz= .2 + CONF(vec2(3.,0.)).w*8. *.2
        ,zh= 3.5 - CONF(vec2(3.,0.)).w*2.5;

    
    vec2 M =CONF(vec2(3.,0.)).xy/ZOOM;  

    float h=  ZOOM2V* float(j)* zz ;

    if(zoomMode|| WIN || DEMO)
        return  (rot( rz +3.14)* ((U - vec2(0.,-R.y/5.+h)) *vec2(1.,zh)-vec2(R.x*(.5+ (DEMO||WIN?0.: ZX*.5)),R.y*zh/2.)) /ZOOM/ZOOM2H /zz  +M ); 
    else
        return  (rot( rz )* ((U - vec2(0.,-R.y/5.+h)) -vec2(R.x*(.5+ (DEMO||WIN?0.: ZX*.5)),R.y*.5)) /ZOOM/ZOOM2H /zz  +M ); 

}


vec4 map(in vec2 P, in float j, in block b,bool zoomView){

    vec4 Q=vec4(0.);

    vec2 P0 = floor(P)+.5; //position center
    vec4 data=CELL(P0);
    vec3 coord= buffer2coord(P0);
    vec3 bcoord= vec3( coord.xy,0.);

    cell c = decodeCell(data,coord); //CURRENT VEHICLE        
    vec2 point = buffer2point(P); //RELATIVE COORDS
    bool outside = bcoord.x <0. || bcoord.y<0. || bcoord.x>=BMAX.x || bcoord.y>= BMAX.y; //true if outside buffer A

    //TERRAIN HEIGHT AND SLOPE

	vec2 pointH=point;
    #if QUALITY>1
    //ROAD  HEIGHT
    
    if((b.btype==4 || b.btype==2 || b.btype==5) && isRoad_(b,point)){ 
        if(b.conns==10) pointH.x=point.x;
        else if(abs(point.x-3.)<1.) pointH.x=3.;
        else if(point.x>4.) pointH.x=6.- (6.-point.x)*1.5;
        else pointH.x=point.x*1.5;

        if(b.conns==5) pointH.y=point.y;
        else if(abs(point.y-3.)<1.) pointH.y=3.;
        else if(point.y>4.) pointH.y=6.- (6.-point.y)*1.5;
        else pointH.y=point.y*1.5;

    }
    #endif  
    //BUSH HEIGHT
    bool bushPoint=((b.btype==0 && b.ltype==0) ||(b.btype>0 && abs(point.x-3.)> 1.5 &&  abs(point.y-3.)> 1.5 )) && hash(P0*3.)>.8 && !isRoad_(b,point) ;
    if(bushPoint ) pointH=floor(point)+.5;

      
    //TERRAIN HEIGHT INTERPOLATION
    float th = +float(b.th);
    if(int(mod(bcoord.x,2.))==1 && int(mod(bcoord.y,2.))==0) {
        vec3 bcoord1 = vec3(coord.xy + DIRS[0],0.);
        float th1= float(decodeBlock(BLOCK(coord2buffer(bcoord1)),bcoord1.xy).th);
        vec3 bcoord2 = vec3(coord.xy + DIRS[2],0.);
        float th2= float(decodeBlock(BLOCK(coord2buffer(bcoord2)),bcoord2.xy).th);
        th=(th1*pointH.y +th2*(6.-pointH.y))/6.;
    }
    else if(int(mod(bcoord.x,2.))==0 && int(mod(bcoord.y,2.))==1){
        vec3 bcoord1 = vec3(coord.xy + DIRS[1],0.);
        float th1= float(decodeBlock(BLOCK(coord2buffer(bcoord1)),bcoord1.xy).th);
        vec3 bcoord2 = vec3(coord.xy + DIRS[3],0.);
        float th2= float(decodeBlock(BLOCK(coord2buffer(bcoord2)),bcoord2.xy).th);
        th=(th1*pointH.x +th2*(6.-pointH.x))/6.;
    }
    else if( int(mod(bcoord.x,2.))==0 && int(mod(bcoord.y,2.))==0){
        vec3 bcoord1 = vec3(coord.xy + DIRS[0]+ DIRS[1],0.);
        float th1= float(decodeBlock(BLOCK(coord2buffer(bcoord1)),bcoord1.xy).th);
        vec3 bcoord2 = vec3(coord.xy + DIRS[2]+ DIRS[1],0.);
        float th2= float(decodeBlock(BLOCK(coord2buffer(bcoord2)),bcoord2.xy).th);
        vec3 bcoord3 = vec3(coord.xy + DIRS[2]+ DIRS[3],0.);
        float th3= float(decodeBlock(BLOCK(coord2buffer(bcoord3)),bcoord3.xy).th);
        vec3 bcoord4 = vec3(coord.xy + DIRS[0]+ DIRS[3],0.);
        float th4= float(decodeBlock(BLOCK(coord2buffer(bcoord4)),bcoord4.xy).th);

        th=(th1*pointH.y*pointH.x 
            +th2*(6.-pointH.y)*( pointH.x)  
            + th3*(6.-pointH.y)*(6.- pointH.x) 
            +th4*pointH.y*(6.-pointH.x))/36.;


    }

    #if QUALITY>1
    //ROAD OVER WATER LEVEL
    if((c.road || b.btype==1 && sdBox(point-3.,vec2(1.))<0.)  &&  c.pos>0 && !outside) th=max(th,5.);
    #endif
    


    //OPTIMIZATION #2
    if( float(j)<=max(th+10.,float(b.bh)) ) {

		//used by stripes 
        float fJun= (b.btype==1)?2.:(b.btype==2 ?1.: 0.);        
    	//roundabout center
    	bool roadP= c.road || (abs(length(point.xy-3.)-1.5)<.5 && !c.road && b.btype==1) ;

        #if DEBUG>0
        if( DEBUG>0 && (abs(point.x-3.)>2.8 || abs(point.y-3.)>2.8)) Q=vec4(0.); //DEBUG borders
        else if (             
            (b.th>0 && th<1. && b.conns==0 && b.ltype==2 && b.btype==0)|| 
            (b.th>0 && th<1. && b.btype>0 && UNCONNECTED)
        ) Q=vec4(1.,0.,.0,0.); //DEBUG HIDDEN UNCONNECTEDS
        else
        #endif
            //RENDERING---------------------


            //BUILDINGS
        if(j<=float(b.bh) + th && b.btype==0 && b.ltype>0 && b.th>=1 &&!outside 
               &&  sdBox(point - vec2(3.),vec2(1.7))<1. 

              ) {

               float h= float(b.bh) + th;
               float shadow= abs(point.x-3.)>  abs(point.y-3.)?.5:1.+sign(point.y-3.);
               //if(zoomMode &&  iMouse.x>R.x/2. && iMouse.y >R.y*.55) h=th+15.;
               h= (floor(h/10.)*10.);

               float fl=j >= (h) ||  int(j)%5==0  ? .5:.5+float(j-th)/float(HEIGHT)*2.;

               if(b.ltype==3 &&b.dist<=1) Q= vec4(1.6*fl,1.6*fl,0. ,h  );
               
               else Q= vec4(vec3(.8+ hash22(coord.xy).x*.3,.6, .6+hash22(coord.xy).y*.3)*fl*(b.conns==0 && GAME ?.3:1.),h  );
               Q.xyz*= (.4+shadow*.3);
               //Q= vec4(.5*fl,.3*fl,.3*fl + hash22(coord.xy).x*.3 ,h); 

            }



        #if QUALITY>1
        //RONDABOUT SIDEWALKS
        else if(j<= max(th+1.5,5.)  && b.btype==1 //&&  sdBox(point.xy-3.,vec2(1.))<2. 
                && abs(length(point.xy-3.)-2.5)<.2 
                && min(abs(point.x-3.),abs(point.y-3.))>1.
               )  Q= vec4(vec3(UNCONNECTED?.2:.6)*(j>0.&& max(th+1.5,5.)-j>1.?.5:1.),max(th+1.5,5.));
        #endif   
		
        //else if(c.shift && c.v[0].vtype>0) Q= vec4(vec3(1.),max(th+1.5,5.)); //DEBUG OVERPASS

        //NOT ROAD
        else if( j<= th +12.   &&( isBlockCoord(coord) || !roadP  || b.btype==0  || outside  ) ){

                
                #if QUALITY>0
            	//ROUNDABOUT CENTER GRASS
                bool rag= (j<= th+1.5  && b.btype==1 && sdBox(point.xy-3.,vec2(1.))<0.);                    
                    
         		vec2 rp= mod(point+.5,1.)-.5;
            
               //SEMAPHORE
                 if(j<= th+10.  && b.btype==2 && sdBox(point.xy-3.,vec2((zoomView?1.2 : 1.5)))<0. && min(abs(point.x-3.),abs(point.y-3.) )>1. ) 
                   Q=vec4( abs(j-th-4.)>3. && sign((point.x-3.)*(point.y-3.))!=sign(float(b.semaphore*2-1))?1.:0. ,0.,0.,th +10.);


                //ROAD SIDEWALKS
                else if(j<= max(5.,th+1.5)  && b.btype!=0 && !roadP && !rag && (
                    (abs(point.x-3.)<1.3   && ((b.conns&1)==1 ||( b.btype==1&& point.y<5.4 )) && point.y>2.)|| 
                    (abs(point.x-3.)<1.3  && ((b.conns&4)==4 ||( b.btype==1 && point.y>.6 ) )&& point.y<4.)|| 
                    (abs(point.y-3.)<1.3   && ((b.conns&2)==2 || ( b.btype==1 && point.x<5.4)) && point.x>2.)||
                    (abs(point.y-3.)<1.3  && ((b.conns&8)==8 || ( b.btype==1 && point.x>.6) ) && point.x<4.)||
                    sdBox(point-3.,vec2(1.3))<0. 

                )) Q=vec4(vec3(UNCONNECTED?.2:.6)*(j>0.&& max(th+1.5,5.)-j>1.?.2:1.),max(th+1.5,5.));
                     

                //BUILDING SIDEWALK
                else if(j<= th+1.5  && b.btype==0 && b.ltype>0 && b.th>=1 &&!outside 
                        &&  sdBox(point -3.,vec2(1.7))<1.3       
                       ) Q=vec4(vec3(b.conns>0?.6:.2),th+1.5);
              
                else  
                #endif
                    //WATER
                 if( th<1.) Q=vec4(  vec3(0.,.7,.9)*GCOL(th)*2. ,0.);
                            
                 //BUSH
                 #if QUALITY>0
                else if(j<= th+12. && bushPoint  && length(rp)*20.>j-th ){
                    
                    float shadow= //abs(atan(-rp.y,rp.x)/3.14-1.57);
                        abs(rp.x)<  abs(rp.y)?.5:1.+sign(rp.y);

                    Q=vec4(0.,GCOL(th)*(.7-shadow*.3),0.,10.+th);
                    
                }
                #endif
                //GRASS
                else if(j<= th+.5 && j>=th || j<1. ) Q=vec4(0.,GCOL(th)*1.2,0. ,th+.5);
                    
                //DIRT
                else Q=vec4(vec3(.6,.4,.4)*GCOL(th),th);

        } 

        //ROAD       
        else if (j<= th +(b.btype==5?15.:10.)  &&  roadP  ) {
            float thr=th;
            float thm= -1.;
            
            //OVERPASS 
            if(b.btype==5) {
                if(abs(point.x-3.)<1. && abs(point.y-3.)<1. && j>th ) thr=th+5.;
                else if(abs(point.x-3.)<1. && abs(point.y-3.)<1. && j<=th) thr=th-5.;
            	else if(point.x>4. && abs(point.y-3.)<1.) thr = th +(6.-point.x)*2.5;
                else if(point.x<2. && abs(point.y-3.)<1.) thr = th + (point.x)*2.5;
                else if(point.y<2.) thr = th - (point.y)*2.5;
                else if(point.y>4.) thr = th - (6.-point.y)*2.5;
                thm=  thr-2.;
            
                if(j<=th ){
                    vec3 ncoord =vec3(c.block_pos,float(c.pos));
                    if(c.pos==15) ncoord.z=10.;
                     if(c.pos==14) ncoord.z=7.;
                     if(c.pos==20) ncoord.z=25.;
                     if(c.pos==21) ncoord.z=28.;
                    vec4 ndata = CELL(coord2buffer(ncoord)); 
                    c = decodeCell(ndata,ncoord);
                    
                }
            }
            //VEHICLE
            float pd= 10.;
            int vtype=0;
            bool brake =false;
            bool lanechange=false;
            if(j<= thr+5.){

            if(float(j)<thr+5. && (j>=thr || j<=0.)){
                for(int n =0;n<LANES;n++){
                    float pdi =  c.v[n].vtype==0? 10.:vdist(mod(point,1.),c.v[n],float(j) -thr>1.5); //DISTANCE FROM VEHICLE
                    if(pdi<pd){
                        pd=pdi;
                        vtype =c.v[n].vtype; //closest vheicle type
                        brake = c.v[n].brake;
                        #if LANES>1
                        lanechange =c.v[n].lane_out!=n;
                        #endif
                    }
                }


                //neghtbours
                for(int i =min(iFrame,0);i<4;i++){
                    for(int n=0;n<LANES;n++){
                        vec3 ncoord =nextCoord(c,i,b.btype);
                        vec4 ndata = CELL(coord2buffer(ncoord)); 
                        cell nc = decodeCell(ndata,ncoord);
                        bool show= b.btype!=5 || ((i%2)==1  && j>=th )||  ((i%2)==0  && j< th );
                        
                        if(nc.v[n].vtype>0 && show ) {
                            float npd=vdist(mod(point,1.) - DIRS[i],nc.v[n],float(j) -thr>1.5);
                            if(pd>npd) {
                                pd=npd;
                                brake=nc.v[n].brake;
                                vtype=nc.v[n].vtype;
                                #if LANES>1
                        		lanechange =nc.v[n].lane_out!=n;
                        		#endif
                            }
                        }
                    }
                }
            }
            }
             if( pd<.0 ) Q=vec4( (brake||lanechange) && abs(j-thr-1.)<.4 ? vec3(brake&&(!lanechange)?1.:0.,lanechange?1.:0.,0.): VCOLOR[vtype-1]*(j-thr>=1.5?.9:1.)*(pd >-.05?.5:1.),3.+thr);

             //ROAD LIGHTS... TODO should be moved on road sides
             #if QUALITY>1
             //else if(j<= thr+10.&& j>= thm && b.btype==4&& abs(point.x-3.05 ) <.1 &&  abs(point.y-3.05 )<.1 )  Q=vec4(j-thr<8.?vec3(.5):vec3(1.,1.,.7),thr+10.);
  	          

            //STRIPES            
  	        else if(j<= thr+.5 && j>= thm && j<th +.5  && mod(point.y,1.)<.5 && (b.conns&1)>0 &&  abs(point.x-3.05 ) <(zoomView?.03:.15)   &&  point.y>3. + fJun)  Q=vec4(1.,1.,1.,thr+.5);
            else if(j<= thr+.5 && j>= thm && j<th +.5 && mod(point.y,1.)<.5 && (b.conns&4)>0 &&  abs(point.x-3.05 ) <(zoomView?.03:.15)   &&  point.y<3. - fJun)  Q=vec4(1.,1.,1.,thr+.5);
            else if(j<= thr+.5 && j>= thm && (j>th -.5 ||j==0.) && mod(point.x,1.)<.5 && (b.conns&2)>0 &&  abs(point.y-3.05 ) <(zoomView?.03:.15)  &&  point.x>3. + fJun)  Q=vec4(1.,1.,1.,thr+.5);
            else if(j<= thr+.5 && j>= thm && (j>th -.5 ||j==0.) && mod(point.x,1.)<.5 && (b.conns&8)>0 &&  abs(point.y-3.05 ) <(zoomView?.03:.15)   &&  point.x<3. - fJun)  Q=vec4(1.,1.,1.,thr+.5);
			#endif
             //ASPHALT 
            else if(( j<= thr && j>= thm) || (j==0.&&b.btype==5))
                Q=vec4(// (isDoubleLane(c.pos,b)?1.:.5)*
                       vec3(.4)*(j>0. && thr-j>1.?.5:1.) *(.9- (th-thr)/20.) * (UNCONNECTED ?.3:.8),thr);
            
            
			else Q=vec4(vec3(0.),j >thm? thr: th-5.);
		 }
    }
    return Q;
}

Main {

    if(RESET<20.) {Q=vec4(0.); return; }//Q=vec4(.4); return;} 

    int sel = int(CONF(vec2(2.,0.)).x);

    //if isometric 3d, the tracing is on horizontal slices
    float dj=2.5;
    int k=0;
    block b;
    b.block_pos=vec2(-100.);

    for(float j =float(!zoomMode && !WIN && !DEMO ?0 : HEIGHT+THEIGHT) ; j>=0. ;j-=dj){

        //SETUP VIEW---------------------
        vec2 P= (U -SHIFT_VIEW)/ ZOOM; 
        if( U.x>R.x*ZX || WIN || DEMO)  {
            P=position(U,float(j));
        } 

        //BLOCK VARIABLES----------------------      
        vec2 P0 = floor(P)+.5; //position center
        vec3 coord= buffer2coord(P0);
        vec3 bcoord= vec3( coord.xy,0.); 

        //OPTIMIZATION #3
        if(length(b.block_pos-coord.xy)>.5)  
            b = decodeBlock(BLOCK(coord2buffer(bcoord)),bcoord.xy); //CURRENT BLOCK
        	//if(b.btype==5 ) b.conns &= 10; //overpass down
        	//if(b.btype==6 ) b.conns &= 5; //overpass up


        //OPTIMIZATION #1     
        if(j<= float(b.th)  + (b.btype==0 && b.ltype>0 ? float(b.bh)+5.:HEIGHT_MARGIN)) {
            k++;
            Q= map(P,j,b,zoomMode|| WIN || DEMO);
            float th=Q.w;
            //if(k>30) Q*=.2;

            //HIGHLIGHT
            vec2 m =  CONF(vec2(0.,0.)).xy;
            if( !DEMO && U.x< R.x*ZX &&   length(buffer2coord(  (m )/ZOOM +.5).xy-bcoord.xy )<1. )  Q+= sel==4|| sel==5 ? vec4(1.,0.,0.,0.): vec4(0.,1.,0.,0.);

            if(th>= float(j)) { 
                break;
            };


            dj=2.5;
            #if QUALITY>0           
            if(j <= th+10.) dj=.5;
            if(j <= th+1.) dj=.2;
            #else
            if(j > th +10.) dj=2.5;  
            if(j <= th+5.) dj=1.;         
            #endif
        }


    } 

   Q.rgb =pow( clamp(Q.rgb*.75,0.,1.) , vec3(.6));
	

    //DEBUG HEIGHT    
    //Q=vec4(Q.a/float(THEIGHT+ HEIGHT),mod(Q.a,10.),.5,0.);


    #ifdef HELP 

    //VERTICAL BAR
    if(((!WIN && !DEMO )) && abs(U.x-iResolution.x*ZX)<2. ) {Q=vec4(0.);return;}


    //INSTRUCTIONS
    #if QUALITY>0
    if((DEMO || WIN) && iMouse.z>0. ){
        if(length(U-vec2(R.x/2.,R.y/3.))<R.x/150.) Q=vec4(1.,0.,0,0.);
        if(abs(U.x-iResolution.x*ZX)<2.  && abs(iMouse.x-iResolution.x*ZX)<R.x/10.) Q=vec4(0.);
        if(iMouse.x> R.x*ZX){
	        pString( U/R*22. -vec2(17.,20.), int[] (82,79,84,65,84,69));
            pString( U/R*22. -vec2(17.,19.), int[] (90,79,79,77));
        }
        else{
        	pString( U/R*22. -vec2(2.,20.), int[] (77,79,86,69));
        }
    }else{
    	if(iMouse.z>0.  && length(U-vec2(R.x*(ZX*.5+.5),R.y/3.))<R.x/150.) Q=vec4(1.,0.,0,0.);
    }
    #endif        

    if(DEMO){
        #if QUALITY>0
        if(RESET<100.) pString( U/R*22. -vec2(7.5,1.), int[] (80,82,69,83,83,32,83,80,65,67,69));
        #endif
    }else{
        //COUNTER
        vec4 c= CONF(vec2(1.,0.));
        //CARS
        if(c.g>0.){
            pString( U/R*22. -vec2(16.,19.), int[] (67,65,82,83,58));
            if(pInt(5., U/R*22. -vec2(19.,19.),(c.g ) ).x >0.4) Q=vec4(1.,1.,0.,0.); 
        }


        //WIN
        if(GAME){
            if(c.b<1. && c.g<1.){
                U.x+=  (-1.+ mod(iTime/3.,2.))*R.x;
                pString( U/R*3. -vec2(1.,1.), int[] (64+23,64+9,64+14));
            }else {
                if(c.b>0.){
                    Q += char(U/R*11. -vec2(8.,10.) ,29   ).x;
                    Q +=pInt(2., U/R*11. -vec2(9.,10.),(100.- c.b/c.r*100. ) ).x; 
                    Q += char(U/R*11. -vec2(10.2,10.) ,37   ).x;
            }
        }
    }
        vec4 Q0=Q;
        if(sdBox(U/R*20. -vec2(10.*ZX,.6 ),vec2(10.*ZX -.2 ,.4 ))<0. &&!keyToggle(CH_CAPS) ){
            Q= vec4(1.);

            vec4 C=vec4(1,.5,.5,1);
            if(abs(floor(U.x/R.x/(ZX-.1)*4.5 +.8*(ZX/.7))-float(sel))<.5) C=vec4(.5,1.,.5,1);
            pStringC( U/R*20.*vec2(.7/ZX) -vec2(1.,.1), int[] (82,79,65,68,32, //ROAD
                                                               83,69,77,65,32, //SEMA
                                                               82,79,85,78,32, //ROUN
                                                               66,85,73,76,32, //BUIL
                                                               71,82,65,83     //GRAS
                                                              ),C);

            if(Q.x>1.) Q-=vec4(1.,1.,1.,1.);else Q=mix(Q0,Q,.3);
        }
        
      
    }
    #endif    

}




// --- Renderpass 2 ---
/*   POSITIONS
5	11	17	23	29	35
4	10	16	22	28	34
3	9	15	21	27	33
2	8	14	20	26	31
1	7	13	19	25	31
0	6	12	18	24	30
--------------*/


    
    //ROUTING

vec2  next_(vec2 point, in block b,bool turn){
    
    //block position is always excluded
	if( abs(point.x-.5) <.5 && abs(point.y-.5)<.5) return vec2(0.);
     
    vec2 d = (vec2(point.x-3.,point.y-3.));
    
    float dm=max(abs(d.x),abs(d.y));
          

    // CROSS   
    if(b.btype==1  ){
        //TURN DIRECTION
        if(turn==true){
        	if(abs(point.x-4.5) <.5 && abs(point.y-2.5)<.5 && (b.conns&2)>0) return vec2(1.,2.);
            if(abs(point.y-4.5) <.5 && abs(point.x-3.5)<.5 && (b.conns&1)>0) return vec2(0.,2.);
            if(abs(point.x-1.5) <.5 && abs(point.y-3.5)<.5 && (b.conns&8)>0) return vec2(3.,2.);
            if(abs(point.y-1.5) <.5 && abs(point.x-2.5)<.5 && (b.conns&4)>0) return vec2(2.,2.);
        
        }
        // DEFAULT DIRECTION
        else{
            if( (abs(point.x-3.5) <1.5 && abs(point.y-4.5)<.5 ) || (abs(dm-2.5) <.5 && abs(d.y-.5)<.5 )) return vec2(3.,15.);   
            if( (abs(point.x-4.5) <.5 && abs(point.y-2.5)<1.5)  || (abs(dm-2.5) <.5 && abs(d.x-.5)<.5 ))return vec2(0.,15.);
            if( (abs(point.x-2.5) <1.5 && abs(point.y-1.5)<.5)  || (abs(dm-2.5) <.5 && abs(d.y+.5)<.5 ))return vec2(1.,15.);
            if( (abs(point.x-1.5) <.5 && abs(point.y-3.5)<1.5)  || (abs(dm-2.5) <.5 && abs(d.x+.5)<.5 )) return vec2(2.,15.);
        }
    }
    //SEMAPHORE
    if(b.btype==2 && turn==true){
        int pos= 1+int(floor(mod(point.x, BLOCK_BUFFER.x))* BLOCK_BUFFER.x +floor(mod((point.y-1.), BLOCK_BUFFER.y)));
    	int k=100;//set to 0 to enable left turn
        if((pos==14+k || (pos==20 && (b.conns&2)>0)) && b.semaphore==0) return vec2(1.,2.);
        if((pos==21+k || (pos==15 && (b.conns&8)>0))  && b.semaphore==0) return vec2(3.,2.);
        if((pos==20+k || (pos==21 && (b.conns&1)>0)) && b.semaphore==1) return vec2(0.,2.);
        if((pos==15+k || (pos==14 && (b.conns&4)>0)) && b.semaphore==1) return vec2(2.,2.);

    }
    
    //JUNCTION
	if((b.btype==4 || b.btype==2 || b.btype==5 || b.btype==6)&& turn==false){ 
        

        
        bool central = (abs(dm) <1.);
        if(b.btype==2 && central){
       		 if(b.semaphore>0) b.conns &= 10; else b.conns &= 5;
   		}
        
        
        //OVERPASS

        float shift_out = 0.;
        if(b.btype==5){                    
        	if( central ) b.conns &= 10; //overpass down              
            int pos= 1+int(floor(mod(point.x, BLOCK_BUFFER.x))* BLOCK_BUFFER.x +floor(mod((point.y-1.), BLOCK_BUFFER.y)));
   			
            if(pos==25 || pos==28) return vec2(0.,15.);  //overpass up
            if(pos==7 || pos==10) return vec2(2.,15.);  //overpass up
        }

        
        //UP
        if(abs(point.x-3.5)<.5 &&(
            ( (b.conns&1)>0 && abs(point.y-5.)<1.  )  
            ||( (b.conns&4)>0 && abs(point.y-1.)<1.  )  
            ||( (b.conns&2)==0 && abs(point.y-2.5)<.5  )  
            ||( (b.conns&1)==1 && abs(point.y-3.5)<.5  )  
        )) return vec2(0.,15.);
		//DOWN
         if(abs(point.x-2.5)<.5 &&(
            ( (b.conns&4)>0 && abs(point.y-1.)<1.  )  
            ||( (b.conns&1)>0 && abs(point.y-5.)<1.  )  
            ||( (b.conns&8)==0 && abs(point.y-3.5)<.5  )  
            ||( (b.conns&4)==4 && abs(point.y-2.5)<.5  )  
        )) return vec2(2.,15.);
        //RIGHT
        if(abs(point.y-2.5)<.5 &&(
            ( (b.conns&2)>0 && abs(point.x-5.)<1.  )  
            ||( (b.conns&8)>0 && abs(point.x-1.)<1.  )  
            ||( (b.conns&4)==0 && abs(point.x-2.5)<.5  )  
            ||( (b.conns&2 )==2 && abs(point.x-3.5)<.5  )  
        )) return vec2(1.,15.);
        //LEFT
        if(abs(point.y-3.5)<.5 &&(
            ( (b.conns&2)>0 && abs(point.x-5.)<1.  )  
            ||( (b.conns&8)>0 && abs(point.x-1.)<1.  )  
            ||( (b.conns&1)==0 && abs(point.x-3.5)<.5  )  
            ||( (b.conns&8 )==8 && abs(point.x-2.5)<.5  )  
        )) return vec2(3.,15.);
        
    }
    
    return vec2(0.);

    
}
vec2  next(int pos, in block b,bool turn){
    vec2 point=  vec2(
         floor(float(pos)/BLOCK_BUFFER.x),
         floor(mod(float(pos),BLOCK_BUFFER.x)) 
    ) +.5;
    return  next_(point,  b, turn);
}

cell nextCell(cell c, int i , int btype){
    vec3 ncoord = nextCoord(c,i,btype); 
    vec4 ndata = CELL(coord2buffer(ncoord));       
    return decodeCell(ndata,ncoord); 
}



//STOP
bool stop(block b, cell c){
    if(b.btype==2 && b.semaphore==1 && (c.pos==16 || c.pos==117 || c.pos==118 || c.pos==19 ) ) return true;
    if(b.btype==2 && b.semaphore==0 && (c.pos==102 || c.pos==8 || c.pos==27 || c.pos==133 ) ) return true;
    return false;
}


bool isShift(in block b, int pos){
    if(b.btype==5 &&  (pos==7 || pos==10 || pos==25 || pos==28 ) ) return true;    
	return false;
}


Main {
    
 	Q =vec4(0.);
 
	vec3 coord = buffer2coord(U);
    vec3 bcoord= vec3( coord.xy,0.);
    
      
    
    //discard texels outside view
    if(coord.x>=BMAX.x || coord.y>= BMAX.y) { return;}
    
    //reset:
    if (iFrame<2 || TS.x != CONF(vec2(0.)).z
		|| (RESET <2. && (DEMO|| WIN))
       ){
        
        

       	int ltype= int(.8+ fbm_noise(100. + bcoord.xy/30.+iTime*100.,2))*2;
        int btype=min(int(.5 + hash(U +iTime)*(ltype==2?1.8: .7)),1)*4;
       
     
        if(isBlockCoord(coord)){
            
            float thp=float(THEIGHT+5) * (1.- length(coord.xy -vec2(BMAX/2.)) /length(BMAX/2.)  );
             int th=  max(0, -5+ int(fbm_noise(bcoord.xy/15.+iTime*100.,2)*thp));
				
            th= int(float(th+6)/10.)*10;
            
            if(mod(coord.x,2.)+mod(coord.y,2.)<.5)  btype= 0;     
            
            if(th<=1) {btype=0; ltype=0;}
            
            int bh= 50+ int(floor(hash(bcoord.xy)*float(HEIGHT-50)/10.)*10.);
            
            float ctr=sdBox(bcoord.xy-vec2(8.),vec2(.5));
            if(ctr<=0.) {
                ltype=3;
                btype=0;
                bh=HEIGHT;
                th=max(th,10);
            }
            else if(ctr<=1.) {btype=1;}
            
        	block b = newBlock(coord,btype,ltype,bh,th);
            
            Q= encodeBlock(b);
        
        }       
        else {
            
            vehicle empty0 = newVehicle(0, 0.,0. ,vec2(0.),0,0,0);
            vehicle empty1 = newVehicle(0, 0.,0. ,vec2(0.),0,0,0);
            cell c = newCell( coord, false,false, vehicle[LANES] (empty0
                                                            #if LANES>1
                                                            ,empty1
                                                            #endif
             ));
            
            for(int n=0;n<LANES;n++){
                if(hash(U*float(n+2))<VEHICLE_DENSITY) {
                     c.v[n] = newVehicle(
                        LANES>1 ? n+1:int(1.5+ hash(U*5.)*1.4),
                        3./  VSTEP,
                        -.52,
                        vec2(vec2(mod(U.x , BMAX.x), mod(U.y ,  BMAX.y))),
                        0,
                        0,
                         n
                        );

                }
            }
        	//packing
            Q= encodeCell(c);
        }
        
    
    } else if(U.y>0.5){
        
        Q = BLOCK(U);
        
        block b = decodeBlock(BLOCK(coord2buffer(bcoord)),bcoord.xy);
        
        int sel = int(CONF(vec2(2.,0.)).x);
        
        //block logic
        if(isBlockCoord(coord)){
            

            //mouse ACTIONS
            vec3 mcoord=buffer2coord(  (MOUSE_INPUT-SHIFT_VIEW) /ZOOM +.5);
            vec2 mcoords= buffer2point( (MOUSE_INPUT-SHIFT_VIEW) /ZOOM +.5)-vec2(3.);
            int mdir =abs(mcoords.x)>abs(mcoords.y)?(mcoords.x>0.?2:8):(mcoords.y>0.?1:4);
            if(!keyToggle(CH_CAPS) && !DEMO &&!WIN  && !zoomMode  &&b.ltype!=3) 
            { 
                
                float l= length(mcoord.xy-bcoord.xy );
                
                if(l<2.&&sel==5 && iMouse.z>0.){
                    int thd=0;
                    if(keyDown(CH_SH)) thd+= 6 -int(l*3.);
                    if(keyDown(CH_CTRL)) thd-= 6 -int(l*3.);
                	 b.th =clamp( b.th + thd,0,THEIGHT);
                }
                
                if(l<1.){
                    if(sel==5) { b.btype= 0; b.ltype=0;b.dist=MAX_DIST;b.aconns=15;}
                    else if(sel==4) {b.btype= 0; b.ltype=2;b.dist=MAX_DIST; b.aconns=15;}
                    else if(sel==3) {b.btype=1; b.aconns=15;}
                    else if(sel==2)  {b.btype=2; b.aconns=15;}
                    else {
                        
                        if(keyDown(CH_CTRL)) b.aconns = b.aconns & (15-mdir);
                        else if(!keyDown(CH_SH) && (b.conns&1) + (b.conns&2)/2 + (b.conns&4)/4 + (b.conns&8)/8 >=3 ) {b.btype=1; } 
                        else {b.btype=4;}
                    }
                }
            }
            

            
            
        	//connections:
        	int conX =0,conY=0, conns=0 , dmin=MAX_DIST,dir=0,buildings=0,th=0;
            for(int i=0;i<4;i++){
            	vec3 nbCoord=vec3(coord.xy + DIRS[i], coord.z);
                if(nbCoord.x>=0. && nbCoord.x< BMAX.x && nbCoord.y>=0. && nbCoord.y< BMAX.y){
                    block nb = decodeBlock(BLOCK(coord2buffer(nbCoord)),nbCoord.xy);
                    int bit =int(pow(2., float(i))), bitn=int(pow(2., float( (i+2)%4))) ;
                    if ((b.aconns & bit )> 0 && (nb.aconns&bitn)>0){
                        if((nb.btype>0  )  && i%2==0) conY++;
                        if((nb.btype>0 ) && i%2==1) conX++;
                        if(nb.btype>0 || nb.ltype==3 ) conns +=  bit;// 1>>i;
                        if(nb.btype==0 && nb.ltype>0 ) buildings +=1;
                    }
                    if(nb.dist<dmin) {dmin= nb.dist;dir=i;}
                    
                    
                    //max th
                    if(int(mod(bcoord.x,2.))==0 && int(mod(bcoord.y,2.))==0 ) {
                        th=b.th;
                    }
                    else th=max(th,nb.th); 
					
                    
                } 
            }
            #if QUALITY>1
            if(b.btype==0 && b.ltype==2 && th>b.bh) b.bh=th+40;
            if(b.btype==0 && b.ltype==2 && th<1) b.ltype=0;
            #endif
            
            //DISTANCE FROM CENTER
            if(b.ltype==3 ) b.dist=!DEMO?0:MAX_DIST;
            else if( dmin< b.dist && b.btype>0 ){b.dist=dmin+1; b.dir=dir;}
            else if( b.dist<  dmin ){ b.dist =MAX_DIST;}
            
            //if(mod(coord.x,2.)+mod(coord.y,2.)<.5) b.btype=0; else 
            
            
            if( RESET <5. ){
                if(conX+conY<1 && b.btype!=0 ) b.btype=0;
                if(conX+conY==1  && b.btype!=0  && buildings==0 ) b.btype=0; 
                if(conX+conY==3 &&  b.btype!=0  ) b.btype=2; 
                else if(conX+conY==4 &&  b.btype!=0  ) b.btype=5;
                else if(conX+conY==2 &&  b.btype!=0  ) b.btype=4;
                else if(b.btype!=0 ) b.btype=4;
            }
            b.conns=conns;
            
            //semaphore-->OVERPASS
            if(b.btype==5 && b.conns<15){
            	b.btype==2;
            }
            if(b.btype==2 && b.conns==15){
            	b.btype=5; 

            }

            
             //SEMAPHORE switch every 5 secs
            if(b.btype==2){
            	b.semaphore = int(mod(iTime/5. +hash(coord.xy*7.),2.));
            }
                
                
			//COUNTER
            b.counter=0.;
            for(float k=1.;k<36.;k++){
            	vec3 vcoord =vec3(coord.xy,k);
        		vec4 vdata = CELL(coord2buffer(vcoord));
        		cell c = decodeCell(vdata,vcoord);
                for(int n=0;n<LANES;n++){
                    bool carCounter= GAME?c.v[n].vtype==1: c.v[n].vtype>0;
                	if(carCounter &&c.road)b.counter+=1.;
                }
            }     
                
            Q= encodeBlock(b);
                
                
        }
        //vehicle logic
        else{
            //unpacking
            cell c = decodeCell(Q,coord);
            
            
            c.road=isRoad( b,  c.pos);          
            c.shift=isShift( b,  c.pos);
                     

			vehicle empty0 = newVehicle(0, 0.,0. ,vec2(0.),0,0,0);
            vehicle empty1 = newVehicle(0, 0.,0. ,vec2(0.),0,0,1);
            
			//clean vehicles outside road
            if(!c.road && !c.shift ) c = newCell( coord, false,false, vehicle[LANES] (empty0
                                                                    #if LANES >1
                                                                    ,empty1
                                                                    #endif
           ));
                      
            else for(int n=0;n<LANES;n++){
                
             
                if(c.v[n].vtype>0 ) {
                
                    c.v[n].brake=false;
                    //next direction (initialization)
                    if(c.v[n].offset <=-.5){
                        vec2 nx =next(c.pos,b,false);
                        if(nx.y>0.) {c.v[n].dir_out=int(nx.x); }

                        vec2 nx2 =next(c.pos,b,true);
                        if(nx2.y>0.  ) {c.v[n].dir_out=int(nx2.x); nx=nx2;}
                    }

					
                    cell nc = nextCell(c,c.v[n].dir_out,b.btype);
                    #if LANES>1
                    // in every momenty, revert lane change if other lane is busy
                    if(c.v[n].lane_out!= n && c.v[1-n].vtype>0) c.v[n].lane_out=n;
                    
                    int n_out = c.v[n].lane_out;
                    #else
                    int n_out =n;
                    #endif
                    
					//check vehicle in same target  lane of next cell
                    float dist =(nc.v[n_out].vtype==0) ? ACC_DIST : (1.+ nc.v[n_out].offset-c.v[n].offset +nc.v[n_out].vel- c.v[n].vel);                
 					
                        
                    #if LANES >1
                     
                        
                    int n2 = 1- n;
                    int n_ch = 1- c.v[n].lane_out;
                    
                    //check distance of vehicle changing line in the next cell
                    float dist2 =(nc.v[n2].vtype==0 || nc.v[n2].lane_out!=n_out ) ? ACC_DIST : (1.+ nc.v[n2].offset-c.v[n].offset +nc.v[n2].vel- c.v[n].vel);                
          			// distance on alternate lane
                    float dist_ch =(nc.v[n_ch].vtype==0) ? ACC_DIST : (1.+ nc.v[n_ch].offset-c.v[n].offset +nc.v[n_ch].vel- c.v[n].vel);                
    
                    dist=min(dist,dist2);
                    dist_ch=min(dist_ch,dist2);
                    
                    #endif
					
                    vec2 nx1 =next(nc.pos ,b,false);
                                      
                    
                    //precedence of from other cells
                    for(int i =0;i<4;i++){

                       cell nc2 =nextCell(nc,i,b.btype);
                        
                        //check vehicle on cells with precedence
                        if(nc2.road && nc2.v[n].vtype>0 
                           && abs( nc2.v[n].dir_out -i)==2 
                           && c.v[n].dir_out!=nc2.v[n].dir_out
                           && int(nx1.x) == nc2.v[n].dir_out
                          
                          ) dist=-1.; //  brake if an incoming car (same lane)has precedence over next cell/lane

                        #if LANES >1     
                        if(nc2.road && nc2.v[n2].vtype>0 
                           && abs( nc2.v[n2].dir_out -i)==2 
                           && c.v[n].dir_out!=nc2.v[n2].dir_out
                           && int(nx1.x) == nc2.v[n2].dir_out
                           //&& nc2.v[n2].lane_out==n // c.v[n].lane_out
                           
                          ) dist=-1.; // brake if an incoming car  (other lane) has precedence over next cell/lane
                        
                        //collision with other lane (only in semaphere and roundabout)
                        if(c.v[n2].vtype>0  &&(b.btype==1 && b.btype==2) && n==1 && c.v[n2].dir_out== ((c.v[n].dir_out+1)%4) ) dist=-1.;
                        #endif
                    	
  
                    }
                    
                    
                    #if LANES >1            
                    if( 
                        c.v[n].offset <=-.4 
                       && ((dist<=DEC_DIST &&  dist_ch>DEC_DIST) //begin passing 
                       //|| (dist_ch>= ACC_DIST   && n==1) ) //end passing
                       )
                       //&& c.v[n].vtype==1
                      ){
                        
                        float dist_inc=ACC_DIST;
                        for(int i =min(iFrame,0);i<4;i++){

                        	cell pc = nextCell(c,i,b.btype);
                            //incoming vehicle distance
                        	if(pc.v[n2].vtype>0 && abs(nc.v[1-n].dir_out -i)==2 )
                    			 dist_inc = min (dist_inc, 1.- nc.v[n2].offset+c.v[n].offset -nc.v[n2].vel+ c.v[n].vel);                

                         }
                        
                        
                        //other line is free
                        if(c.v[n2].vtype==0 && c.v[n].lane_out==n && dist_inc>=ACC_DIST){
                        	c.v[n].lane_out=n2;
                            dist=dist_ch;
                        }
                        //change idea...
                        else {
                        	c.v[n].lane_out=n;
                            dist=dist_ch;
                        }                              
                    } 
                    #endif
                   
                    
					
                     if(dist < DEC_DIST || stop(b,c) ) {
                         
                        c.v[n].vel=  clamp(c.v[n].vel-1./VSTEP,0.,3./VSTEP)  ;
                        c.v[n].brake=true;
                    } 
                    
                    
                    else if(dist >=ACC_DIST){
                        float vmax= VMAX[c.v[n].vtype-1];
                        if(n==1) vmax+=1.;
                        c.v[n].vel = clamp(c.v[n].vel+1./VSTEP,1.,vmax/VSTEP)  ;
                    }

                    //update offset                          	                
                    c.v[n].offset += c.v[n].vel;

                    //vehicle leaving cell
                    if(c.v[n].offset>=.5) {
                        c.v[n].offset=-.5;c.v[n].vel=1./VSTEP;
                        c.v[n] = newVehicle( 0, 0.,0. ,vec2(0.),0,0,n);
                        c.road= !c.shift;

                    };


                }

 	            else if(c.v[n].vtype==0){ 
                    //incoming vehicles   
                    for(int i =min(iFrame,0);i<4;i++){

                        cell nc = nextCell(c,i,b.btype);
                        bool incoming=false;
                            #if LANES >1
                                int nlane_out=nc.v[n].lane_out;
                            #else
                                int nlane_out=n;
                            #endif
                        if(nc.v[n].vtype>0 && nlane_out==n && nc.v[n].offset+nc.v[n].vel>=.5 && abs(nc.v[n].dir_out -i)==2 ){

                                c.v[n].vtype=nc.v[n].vtype;
                                c.v[n].offset=nc.v[n].offset+nc.v[n].vel -1.;
                                c.v[n].vel=nc.v[n].vel;
                                c.v[n].dir_in= i;
                            	
                            #if LANES>1
                            	c.v[n].lane_out=n;
                            #endif
                            	incoming=true;
                         }
#if LANES >1
                        // incoming vehicle with line change
                        if(!incoming && nc.v[1-n].vtype>0 && nc.v[1-n].lane_out==n && nc.v[1-n].offset + nc.v[1-n].vel>=.5 && abs(nc.v[1-n].dir_out -i)==2 ){

                                c.v[n].vtype=nc.v[1-n].vtype;
                                c.v[n].offset=nc.v[1-n].offset+nc.v[1-n].vel -1.;
                                c.v[n].vel=nc.v[1-n].vel;
                                c.v[n].dir_in= i;
                            	c.v[n].lane_out=n;
                            	incoming=true;
                         }
#endif
                                
                         if(incoming==true){
                                //next direction                          
                                vec2 nx =next(c.pos,b,false);

                                //default direction
                                if(nx.y>0.) {c.v[n].dir_out=int(nx.x); }

                                //turn direction (if allowed)
                                vec2 nx2 =next(c.pos,b,true);
                                //connected... turn only if direction to destination
                                if(nx2.y>0. && b.dist<MAX_DIST  &&nc.v[n].vtype== 1){
                                   if( int(nx2.x)==b.dir ) {c.v[n].dir_out=int(nx2.x); nx=nx2;}
                                }
                                //else turn randomly 
                                else if(nx2.y>0. && hash(iTime +U)> TURN ) {c.v[n].dir_out=int(nx2.x); nx=nx2;}
              
                         }
                    }
                   
    
            	}
             
        	}
            //packing
            
            Q= encodeCell(c);
        }
   
    } else {
    //COUNTERS
    
        int sh = int(mod(U.x -.5,BLOCK_BUFFER.x));
        Q=vec4(0.); //r=?,g=?, b=unconnected,a=?
                

         if(sh==1){
            vec3 coord = buffer2coord(U+vec2(-1.,1.));

            //subtotal over columns
            for(float k=  -1.;k<BMAX.y;k++){
                vec3 nbCoord=vec3(coord.xy +DIRS[0]*k, coord.z);
                block nb = decodeBlock(BLOCK(coord2buffer(nbCoord)),nbCoord.xy);

                Q.g += nb.counter;
				if(nb.th>=1 && ( nb.btype>0  || (nb.ltype==2 && nb.btype==0))) Q.r+=1.;
                if(nb.th>=1 && nb.conns==0 && nb.ltype==2 && nb.btype==0) Q.b +=1.;
                if(nb.th>=1 && nb.btype>0 && nb.dist>=MAX_DIST) Q.b +=1.;
            }

            //total over rows
            if(U.x <6.)for(float k=1.;k<BMAX.x+1. ;k++){
                Q +=CONF(U + vec2(BLOCK_BUFFER.x,0)*k );
            }
        } 
        else if( U.x <6. && sh==2){
            
            //MOUSE POSITION
            
            if(sdBox(iMouse.xy/R*20. -vec2(10.*ZX,.6 ),vec2(10.*ZX -.2 ,.4 ))<0.)             
            Q.x= floor(iMouse.x/R.x/(ZX-.1)*4.5 +.8*(ZX/.7));
            else Q.x=max(CONF(vec2(2.,0.)).x,1.);
            
            Q.y= 0.;// keyToggle(CH_SPACE)?1.:0.;
            Q.z=  mod((iMouse.x>R.x*ZX)?((iMouse.x-R.x*ZX)/(R.x*ZX) *3.14*2.) : (iMouse.x<1.? iTime/4.:3.4),6.28);           
            Q.w= clamp(RESET<5.? 1.5:  (iMouse.x>R.x*ZX)? iMouse.y/R.y:.3,.0,10.);
        }
        else if( U.x <6. && sh==3){
            
            
            //SMOOTH CAMERA POSITION
            //xy=position, z=rotation w= zoom 
             
            
            float mx =  RESET<5.  ?1.: clamp(1./iFrameRate,0.,1.); //(iFrameRate>10.?.07:.15) ;                  
            Q.xy=mix(CONF(vec2(3.,0.)), CONF(vec2(0.,0.)),mx).xy;
            
            float rz1=CONF(vec2(3.,0.)).z, rz2= CONF(vec2(2.,0.)).z;
            //if(abs(rz1-rz2)>6.28) {mx=1.;}
            
            if(rz2<rz1  && rz1-rz2>3.14) {rz2=rz1+.5; }
            
            Q.z= mod(mix(rz1, rz2,mx),6.28); 
            Q.w=  mix(CONF(vec2(3.,0.)), CONF(vec2(2.,0.)),mx).w;
        }
    }
    
    // save CURRENT TEXTURE SIZE
    if(max(U.x,U.y)<1.) {
        Q= vec4(
            iMouse.x<1. || RESET<5.? R/4.: iMouse.x>iResolution.x*ZX? CONF(U).xy: iMouse.xy-SHIFT_VIEW,
            TS.x, 
            iFrame==0 || TS.x != CONF(vec2(0.)).z || ( keyDown(CH_SPACE) ) ? float(iFrame):CONF(vec2(0.)).w);
    } 
}

// --- Renderpass 3 ---

//SETTINGS
#define QUALITY 2 //0=LOW,1=MEDIUM, 2=HIGH
#define MIN_ZOOM 3. //minimum zoom (for small resolutions)
#define CITY_SIZE 30. //20 small 40 medium 100 big
#define LANES 1 //1 = sigle lane, 2 = double lane (takes up to 40s compilation time)

#define VEHICLE_DENSITY 0.5 //0.=no traffic 1.= full
#define ZOOM2H 5. // horizontal zoom for right panel
#define ZOOM2V (3. *ZOOM/6.) // vertical zoom for right panel
#define HEIGHT 80 //max building height
#define THEIGHT 120 // max terrain height (-5 water level)
#define VMAX float[2] (5.,3.) // max speed by vehicle type
#define VCOLOR vec3[2] (vec3(1.,1.,0.) ,vec3(0.,.2,1.)) // color by vehicle type
#define ACC_DIST 1.8 // higher value: less reactive cars
#define DEC_DIST 1.7 //lower values causes vehicle crashes and disappereance
#define TURN 0. //0.= always (no congestions) , 1.= never (high congestions)
#define HEIGHT_MARGIN 25.
#define HELP  //+1s compilation time...



//DEBUG
#define DEBUG 0 // set to 1 to display debug colors
#if DEBUG>0
    #define ALERT_COLOR vec3(0.,1.,1.)
    #define DEBUG_COLOR vec3(.3,0.,0.)
    #define DEBUG_COLOR2 vec3(.5,0.,0.)
#endif


//INTERNAL
#define CH_Z 90  
#define CH_SPACE 32
#define CH_SH 16
#define CH_CAPS 20
#define CH_CTRL 17
//#define CH_ALT 18
#define keyDown(k) (texelFetch(iChannel1, ivec2(k,0), 0).r>.5)
#define keyPress(k) (texelFetch(iChannel1, ivec2(k,1), 0).r>.5)
#define keyToggle(k) (texelFetch(iChannel1, ivec2(k,2), 0).r>.5)
#define DEMO  !keyToggle(CH_SPACE)
#define GAME !keyToggle(CH_CAPS) //false to disable connection counters 
#define zoomMode  (!DEMO && !keyToggle(CH_Z) &&  U.x > iResolution.x*(!WIN?ZX:0.))
#define CITY_SIZE2  CITY_SIZE //(DEMO?15.:CITY_SIZE)
#define R iResolution.xy
#define ZOOM max(MIN_ZOOM, (R.y)/6./(CITY_SIZE2) ) //zoom factor for left panel
#define VP 5.
#define VSTEP pow(2.,VP)
#define BLOCK_BUFFER vec2(6.,6.)
#define ZX .5 // split screen ratio from 0 to 1 (.3= big preview, .7=small preview )
#define BMAX floor(iResolution.xy/BLOCK_BUFFER/ZOOM*vec2(ZX,1.) -2.)
#define OSTEP VSTEP
#define MAX_DIST min((1<<10)-2,int(CITY_SIZE*3.))
#define UNCONNECTED (GAME &&  b.dist>=MAX_DIST)
#define SHIFT_VIEW vec2(3.,R.y/20. )
#define MOUSE_INPUT  iMouse.xy
#define RZ (R/ZOOM)
#define Main void mainImage(out vec4 Q, vec2 U) 
#define CELL(U) texelFetch(iChannel0, ivec2(mod(U,R) ), 0) //texture(iChannel0,(U)/R)
#define BLOCK(U) texelFetch(iChannel0, ivec2(mod(U,R) ), 0) //texture(iChannel0,(U)/R)
#define CONF(U) texelFetch(iChannel0, ivec2(mod(U,R) ), 0) //texture(iChannel0,(U)/R)
#define WIN (CONF(vec2(1.,0.)).b<1. && CONF(vec2(1.,0.)).g<1. && GAME)
#define TS vec2(textureSize(iChannel0,0))
#define RESET abs(float(iFrame)-max(CONF(vec2(0.)).w,0.)) 

#define sb(f,s,b,v) f+=(clamp(floor(v+.5),0.,pow(2.,b)-1.)-gb(f,s,b))*pow(2.,s)
float gb(float c, float start, float bits){return mod(floor(c/pow(2.,start)),pow(2.,bits));}//get bits    
const vec2 DIRS[4] = vec2[] (vec2(0,1), vec2(1,0),vec2(0,-1) ,vec2(-1,0));  
const float PI = 3.14159265359;

//iq
float sdBox( vec2 p, vec2 b )
{
  vec2 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,q.y),0.0);
}

//Hash/noise utils (copied from others)
float hash(in float x) { return fract(sin(x*.0007)*29835.24389); }
float hash(in vec2 x) { return hash(dot(x,vec2(23.17,17.23))); }
vec2 hash22( vec2 p ) { float t = hash(p);	return vec2( t,hash(t)); }
lowp vec3 permute(in lowp vec3 x) { return mod( x*x*34.+x, 289.); }
lowp float snoise(in lowp vec2 v) {
  lowp vec2 i = floor((v.x+v.y)*.36602540378443 + v),
      x0 = (i.x+i.y)*.211324865405187 + v - i;
  lowp float s = step(x0.x,x0.y);
  lowp vec2 j = vec2(1.0-s,s),
      x1 = x0 - j + .211324865405187, 
      x3 = x0 - .577350269189626; 
  i = mod(i,289.);
  lowp vec3 p = permute( permute( i.y + vec3(0, j.y, 1 ))+ i.x + vec3(0, j.x, 1 )   ),
       m = max( .5 - vec3(dot(x0,x0), dot(x1,x1), dot(x3,x3)), 0.),
       x = fract(p * .024390243902439) * 2. - 1.,
       h = abs(x) - .5,
      a0 = x - floor(x + .5);
  return -0.278 + .5 + 65. * dot( pow(m,vec3(4.))*(- 0.85373472095314*( a0*a0 + h*h )+1.79284291400159 ), a0 * vec3(x0.x,x1.x,x3.x) + h * vec3(x0.y,x1.y,x3.y));
}
float fbm_noise(vec2 uv, int steps) {
	float v = 0.;
    for(int i = 0; i < steps; i++){
        float factor = pow(2.,float(i + 1)) / 2.;
    	v += snoise(uv * factor) / factor;
    }
    return v / ((pow(.5,float(steps))- 1.) / -.5);
}
 
     
//CELL-VEHICLE
struct vehicle {

    //V1
    int vtype; //2 bit 0=none 1=car,2=slowcar 
    int dir_in; // 0..3 DIRS[i]
    int dir_out;   // 0..3 DIRS[i]     
	float offset; // -.5 ... +.5
	float vel; // 0. ...  +1.	
    bool brake; 
    int lane; //lane
    #if LANES>1
    int lane_out;
    #endif
};
 
vehicle newVehicle( int vtype,float vel, float off, vec2 tg , int di, int dout,int lane){
    vehicle o;

    o.vtype=vtype; 
    o.dir_in= di ;
    o.dir_out=dout;
	o.offset= off; 
	o.vel=vel; 
    o.brake=false;
    o.lane=lane;
    #if LANES>1
    o.lane_out=lane;
    #endif
    return o;
}


struct cell {
    vec2 block_pos; // block xy  (not persistent)
    int pos; //  (not persistent)
    bool road;
    bool shift;
    
	vehicle[LANES] v;  //vehicles (max 2)
    
    
};
 
cell newCell(vec3 coord, bool road,bool shift, vehicle[LANES] v){
//cell newCell(vec3 coord, bool road, int vtype,float vel, float off, vec2 tg , int di, int dout){
    cell o;
    o.block_pos=coord.xy;   
    o.pos=int(coord.z); 
    o.road=road;
    o.shift=shift;
    o.v[0]= v[0];
    o.v[0].lane=0;
#if LANES >1    
    o.v[0].lane_out=0;

    o.v[1]= v[1];
    o.v[1].lane=1;
    o.v[1].lane_out=1;
#endif

    return o;
}

cell decodeCell(vec4 t,vec3 coord) {
	cell o;
    o.pos=int(coord.z);
    o.block_pos= coord.xy;
    
    //VEHICLES
    for(int n=0;n<LANES;n++){
		float f = (n==0 ? t.b:t.r);
        o.v[n].offset= gb(f,0., VP) /pow(2.,VP)  -.51; 
        o.v[n].vel= gb(f,5., VP) /pow(2.,VP);
        o.v[n].vtype= int(gb(f,10., 2.));  
        o.v[n].dir_in = int(gb(f,12., 2.));
        o.v[n].dir_out = int(gb(f,14., 2.));
        o.v[n].lane=n;
    }
    
	//CELL
    o.road=gb(t.a,0., 1.)<.5;
    o.shift =gb(t.a,1., 1.)<.5;
    o.v[0].brake=gb(t.a,2., 1.)<.5;
#if LANES>1
    o.v[0].lane_out=int(gb(t.a,3., 1.));

    o.v[1].brake=gb(t.a,4., 1.)<.5;
    o.v[1].lane_out=int(gb(t.a,5., 1.));
#endif
    
    
    return o;
}


vec4 encodeCell(cell o) {
	vec4 t=vec4(0.);
  
    //VEHICLES
    for(int n=0;n<LANES;n++){
        float f=0.;
        sb(f,0.,VP,(o.v[n].offset +.51)* pow(2.,VP));
        sb(f,5.,VP,o.v[n].vel * pow(2.,VP));  
        sb(f,10.,2.,float(o.v[n].vtype));
        sb(f,12.,2.,float(o.v[n].dir_in));
        sb(f,14.,2.,float(o.v[n].dir_out));       
        if(n==0) t.b=f; else t.r=f;
    }

    
    //CELL
    sb(t.a,0.,1.,o.road?0.:1.);
    sb(t.a,1.,1.,o.shift?0.:1.);
	sb(t.a,2.,1.,o.v[0].brake?0.:1.);
#if LANES>1
    sb(t.a,3.,1.,float(o.v[0].lane_out)); 

    sb(t.a,4.,1.,o.v[1].brake?0.:1.); 
    sb(t.a,5.,1.,float(o.v[1].lane_out));
#endif
    
    return t;
}

//BLOCK (or Tile)
struct block {
    vec2 block_pos;
    int btype; //3 bit 0=none 1=roundabout 2=semaphore 4=street 5 =overpass 
    int ltype; //0=none  2=building 3=center
    int bh; //building height
    int th; //terrain height
    int conns; // current connections (bitmask)
    float counter;
    int dist; //distant from center
    int dir; //dir to center
    int aconns;// allowed connections (bitmask)
    
    int semaphore; //0:YELLOW 2:VERTICAL 1:HORIZONTAL
       
};

block newBlock(vec3 coord, int btype,int ltype, int bh, int th ){
	block o;
    o.block_pos= coord.xy;
    o.btype=btype; 
    o.ltype=ltype;
    o.th=th;
    o.bh=bh;
    o.conns=15;
  	o.counter=0.;
    o.dist=MAX_DIST;
    o.dir=0;
    o.semaphore=0;
    o.aconns=15;
    return o;               
}              
                 
block decodeBlock(vec4 t,vec2 coord) {
	block o;
    o.block_pos= coord;
    o.btype=int(gb(t.r,0., 3.));
    o.ltype = int(gb(t.r,3., 3.)); 
    o.conns = int(gb(t.r,6., 4.));
    o.semaphore = int(gb(t.r,10., 2.));
    o.aconns =int(gb(t.r,12., 4.));
    
    o.bh = int(gb(t.g,0., 8.));
    o.th = int(gb(t.g,8., 8.));
    o.counter=t.b;
    o.dist= int(gb(t.a,0., 10.));
    o.dir= int(gb(t.a,10., 2.));
    return o;
}

vec4 encodeBlock(block o) {
	vec4 t=vec4(0.);
    
    sb(t.r,0.,3.,float(o.btype));
    sb(t.r,3.,3.,float(o.ltype));
    sb(t.r,6.,4.,float(o.conns));
    sb(t.r,10.,2.,float(o.semaphore));
    sb(t.r,12.,4.,float(o.aconns));
    sb(t.g,0.,8.,float(o.bh));
    sb(t.g,8.,8.,float(o.th));
    
    t.b=o.counter;
    sb(t.a,0.,10.,float(o.dist));
    sb(t.a,10.,2.,float(o.dir));
    return t;
}

//MAPPING FUNCTIONS
bool isBlockCoord(vec3 coord) {
    return (int(coord.z)== 0);
}
    
vec3 buffer2coord(vec2 b) {  
    return vec3(
        floor(b.x/BLOCK_BUFFER.x),
        floor((b.y-1.)/BLOCK_BUFFER.y),
        floor(mod(b.x, BLOCK_BUFFER.x))* BLOCK_BUFFER.x +floor(mod((b.y-1.), BLOCK_BUFFER.y))
     );

}

vec2 buffer2point(vec2 b){
	return vec2(
    		mod(b.x, BLOCK_BUFFER.x),
        	mod((b.y-1.), BLOCK_BUFFER.y)
    );
}
    
vec2 coord2buffer(vec3 c){
	return vec2(
        floor(c.x) * BLOCK_BUFFER.x  + floor(c.z/BLOCK_BUFFER.x),
        floor(c.y) * BLOCK_BUFFER.y  + floor(mod(c.z,BLOCK_BUFFER.x)) +1.
    );
}

vec3 nextCoord(cell c, int d, int btype){
    
    vec2 dir=DIRS[d];
    if(btype==5){
       	int npos=-1;
        if(d==0) switch (c.pos){
			case 19: npos=25;break;
            case 25: npos=28;break;
            case 28: npos=22;break;
            case 10: npos=16;break;
            case 7: npos=10;break;
            case 13: npos=7;break;
            default: break;

        }else if(d==2) switch (c.pos){
        	case 25: npos=19;break;
            case 28: npos=25;break;
            case 22: npos=28;break;
            case 16: npos=10;break;
            case 10: npos=7;break;
            case 7: npos=13;break;
            default: break;
        }
        
        if((d%2)==0 &&
            (c.pos==9 || c.pos==14 ||c.pos==15 ||c.pos==20 ||c.pos==21 ||c.pos==26 )    
                ) npos=5;

        if(npos!=-1) return vec3(c.block_pos,float(npos));
    }
	return buffer2coord(coord2buffer(vec3(c.block_pos,c.pos)) +dir);
}

/*
//SLOW COMPILATION
vec3 nextCoord(cell c, int d, int btype){
    
    vec2 dir=DIRS[d];
    if(btype==5){
    	if(c.pos==19 && d==0) return vec3(c.block_pos,25.);
        if(c.pos==25 && d==2) return vec3(c.block_pos,19.);
        if(c.pos==25 && d==0) return vec3(c.block_pos,28.);
        if(c.pos==28 && d==2) return vec3(c.block_pos,25.);
        if(c.pos==28 && d==0) return vec3(c.block_pos,22.);
        if(c.pos==22 && d==2) return vec3(c.block_pos,28.);
        
        if(c.pos==16 && d==2) return vec3(c.block_pos,10.);
        if(c.pos==10 && d==0) return vec3(c.block_pos,16.);
        if(c.pos==10 && d==2) return vec3(c.block_pos,7.);
        if(c.pos==7  && d==0) return vec3(c.block_pos,10.);
        if(c.pos==7  && d==2) return vec3(c.block_pos,13.);
    	if(c.pos==13 && d==0) return vec3(c.block_pos,7.);
        
        if(c.pos==15 && (d%2)==0) return vec3(c.block_pos,5.);
        if(c.pos==9 && (d%2)==0) return vec3(c.block_pos,5.);
        if(c.pos==26 && (d%2)==0) return vec3(c.block_pos,5.);
        if(c.pos==14 && ( d%2)==0 ) return vec3(c.block_pos,5.);
        if(c.pos==20 && ( d%2)==0) return vec3(c.block_pos,5.);
        if(c.pos==21 && ( d%2)==0) return vec3(c.block_pos,5.);
    }
	return buffer2coord(coord2buffer(vec3(c.block_pos,c.pos)) +dir);
}*/

//ROAD
bool isRoad_(in block b, vec2 point){
            
    float fCross= (b.btype==1)?1.:0.;
    
    bool road=true;
    //if(b.btype==5 ) b.conns &= 10; //overpass down
    //if(b.btype==6 ) b.conns &= 5; //overpass up
    
    if( b.btype==0 )  road=false;
    else if(b.btype==1 &&  (abs(point.x-3.) +abs(point.y-3.) >3.  || abs(point.x-3.) +abs(point.y-3.) <2. )) road=false;    
        
    else if((b.conns&1)==0 &&  abs(point.y-1. ) >3. +fCross  )  road=false;
    else if((b.conns&2)==0 &&  abs(point.x-1. ) >3. +fCross )  road=false;
    else if((b.conns&4)==0 &&  abs(point.y-5. ) >3. +fCross  )  road=false;
    else if((b.conns&8)==0 &&  abs(point.x-5. ) >3. + fCross )  road=false;
    else if(( b.btype==4 || b.btype==2 || b.btype==5  )&&  (min(abs(point.x-3.),abs(point.y-3.)) >1.)    ) road=false;
        
	return road;
}
bool  isRoad(in block b, int pos){
    vec2 point=  vec2(
         floor(float(pos)/BLOCK_BUFFER.x),
         floor(mod(float(pos),BLOCK_BUFFER.x)) 
    ) +.5;
    return  isRoad_( b, point);
}




