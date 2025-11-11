// iChannel0 = Buffer A (self)
// iChannel1 = keyboard

// y=0: nodes on editor
// y=1: segments
// y=2: nodes on simulation

void mainImage( out vec4 O, in vec2 U )
{
   
   ivec2 c =ivec2(U);
   if(c.y>2) discard;
   O= texelFetch(iChannel0,ivec2(U),0);
   
    if(iFrame<1){
        //init
        vec3 V[] = vec3[] (vec3(-.5,0,1),vec3(-.3,0,0),vec3(-.1,0,0),vec3(.1,0,0),vec3(.3,0,0), vec3(.5,0,1),
                                vec3(-.4,.2,0),vec3(-.2,.2,0),vec3(0,.2,0),vec3(.2,.2,0),vec3(.4,.2,0),
                                                            vec3(0.,-.3,1)
                          );
        vec2 E[] = vec2[] (vec2(1,2),vec2(2,3),vec2(3,4), vec2(4,5),vec2(5,6),
                            vec2(7,1),vec2(8,2),/*vec2(9,3),*/vec2(10,4), vec2(11,5) ,
                            vec2(7,2),vec2(8,3),vec2(9,4),vec2(10,5), vec2(11,6) ,
                            vec2(7,8),vec2(8,9),vec2(9,10),vec2(10,11)                            
                          );
        if(c.y==0 || c.y==2){
           if(c.x==0) O=vec4(V.length(),0,0,0);
           else if(c.x<=V.length()+1)  if(c.y==0) O.xyz= V[c.x-1]; else O= V[c.x-1].xyxy; 

        }
        else if(c.y==1){
           if(c.x==0) O=vec4(E.length(),0,0,0);
           else if(c.x<=E.length()+1)  O.xy= E[c.x-1];
            
        } 
    } else if ( c.y==2){       
        if(!editMode){
           //simulation
           vec4[100] a;
           int N = int(A(0).x);
           int M = int(B(0).x); 
           float G=.003,SS=20.,DT=1./SS;
           
            //init
            for(int i =0;i<N +min(iFrame,0);i++) a[i] = V(i+1);
            
            //substeps 
            for(float s=0.;s<SS;s++){
            
                //verlet + gravity
                for(int i =0;i<N +min(iFrame,0);i++){
                    vec2 po=a[i].zw, p = a[i].xy;
                    vec2 pn= 2.* p - po + .5*DT*DT *vec2(0,-G);  
                   if(A(i+1).z<1.) a[i]=vec4(pn,p);          
                }
               
               //skicks
               for(int k =1;k<=M  +min(iFrame,0);k++){
                    ivec2 v=ivec2(B(k).xy);
                    int i=v.x-1, j=v.y-1;
                    float L = length(A(v.x).xy-A(v.y).xy);          
                    float d = length(a[i].xy -a[j].xy);
                    vec2 m=(a[i].xy+ a[j].xy)*.5,
                         d0= (1.- d/L)*(m-a[i].xy)/STIFF,
                         d1= (1.- d/L)*(m-a[j].xy)/STIFF;

                   if(A(v.x).z<1.) a[i].xy-= d0.xy;
                   if(A(v.y).z<1.) a[j].xy-= d1.xy;
               }
               
               //todo: collisions abd breaking
           } 
           
           //set current node
           O=a[c.x-1];                      
           
        } else O=A(c.x).xyxy; //copy from editor
    } else if(editMode){
       
        int N = int(A(0).x); 
        int M = int(B(0).x); 
        int LA =int(max(A(0).y,B(0).y)); //last action
        vec2 d =Mouse.xy-A(c.x).xy;
        
        //closest node
        vec2 m =vec2(0,1e8);
        for(int i =1;i<=N;i++) {
            float d = length(Mouse.xy-A(i).xy);
            if(m.y>d) m=vec2(i,d);
         } 
 
         //closest existing segment
        int  id=0;
        for(int j =1;j<= M;j++) {
            float d = line(Mouse.xy, A(B(j).x).xy, A(B(j).y).xy);
            if( d<.02) id=j;
        }

        //closest potential segment
         vec3 ms =vec3(0,0,.3);                   
        for(int i =1;i<= N;i++) for(int j =i+1;j<= N;j++){
            float d =    line(Mouse.xy, A(i).xy, A(j).xy),
                  l = length(A(i).xy -A(j).xy);
            if(d<.02 && l<ms.z) ms=vec3(i,j,l);
        }
         if( keyDown(RESET)) {
             O=vec4(0);
             return;
         
         }  
         
        //drag node 
        if(c.y==0 && length(d)<.03 && int(m.x)==c.x && !keyDown(EDGE_MODE) && !keyDown(VERTEX_MODE) && !keyDown(PIN_MODE)) O.xy += d;
        
        if( keyDown(VERTEX_MODE)  && iMouse.z>0. && m.y> .05 && (iFrame - LA)>WAIT) { 
            //add node      
            if(c.y==0 && c.x==N+1)  O=vec4(Mouse.xy,0,0); 
            if(c.y==0 && c.x==0) O.xy=vec2(N+1,iFrame); 
            if(id>0 && c.y==1){
                if( c.x==id) O.x=float(N+1);
                if( c.x==M+1) O.xy =vec2(B(id).x, N+1);
                if( c.x==0) O.xy=vec2(M+1,iFrame); 
                
            }
        }       
        
        if( keyDown(VERTEX_MODE)  && iMouse.z>0. && m.y<.05  && (iFrame - LA)>WAIT ) { 
           int  id=0;
            for(int j =1;j<= M;j++) {
                if( B(j).x==m.x || B(j).y==m.x ) id=j;
            }
            if(id==0){
                //remove node if unconnected
                if(c.y==0 && c.x>= int(m.x) ) O= A(c.x+1);
                if(c.y==0 && c.x==0) O.xy=vec2(N-1,iFrame);
                if(c.y==1 && c.x>0 ){
                    if(O.x>m.x) O.x-=1.;
                    if(O.y>m.x) O.y-=1.;
                }
            }
        } 
        if(c.y==0 && keyDown(PIN_MODE) && iMouse.z>0. && m.y< .1  && (iFrame - LA)>WAIT ) { 
            //pin/unpin
            if(c.x== int(m.x)) O.z=1.-O.z;
            if(c.x==0) O.y=float(iFrame);
        }
 
        
        if(keyDown(EDGE_MODE)  && iMouse.z>0. && (iFrame - LA)>WAIT ) {           
            //remove segment
            if(id>0 && (iFrame - LA)>WAIT){                           
                if(c.y==1  && c.x>= id ) O.xy= B(c.x+1).xy;
                if(c.y==1  && c.x==0) O.xy=vec2(M-1,iFrame);
            }
            //add segment
            else if(id==0 && ms.x!=0.  && (iFrame - LA)>WAIT){
                if(c.y==1  && c.x==M+1)  O.xy = ms.xy;
                if(c.y==1  && c.x==0) O.xy=vec2(M+1,iFrame); 
            }
            //add segment and node
            else if(m.y> .05 &&N>0 ){           
                if(c.y==0 && c.x==N+1) O =vec4(Mouse.xy,0,0); 
                if(c.y==0 && c.x==0) O.xy=vec2(N+1,iFrame); 
                if(c.y==1 && c.x==M+1) O.xy = vec2(N+1,m.x);
                if(c.y==1 && c.x==0) O.xy=vec2(M+1,iFrame); 
            }
        }
        
        
        
    }

}
