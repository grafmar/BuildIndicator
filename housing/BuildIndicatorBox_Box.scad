
ledRows = 8;        // number of LED-rows
ledCols = 2;        // number of LED-columns
wall = 3;           // wall sickness [mm]
separator = 1;      // separator thickness [mm]
ledWidth = 50;      // width of 1 LED-indication segment
ledHeight = 50/3;   // height of 1 LED-indication segment (1.67mm LED pitch)
ledDepth = 20;      // depth of 1 LED indication segment (Box thickness = ledDepth+wall)
hookThickness = 2;

$fs=.5;
/*intersection(){
    translate([55,-40,-1]){cube([50,45,30]);}
    box();
}*/
box();
//translate([0,0,0]){boxCover();}
//translate([wall,ledHeight*ledRows+wall,ledDepth+wall]){rotate([180,0,0]){diffusor();}}
//diffusor();

/*
difference() {
    union() {
        box();
        translate([wall,ledHeight*ledRows+wall,ledDepth+wall]){rotate([180,0,0]){diffusor();}}
    }
    sphere(r=60,$fn=200);
}
*/

module hook() {
  translate([0,-2.5,-3]){cube([2, 5, 3],center=false);}
  hull() {
    translate([0,0,-2]){sphere(r=1);}
    translate([-0.5,0,-2]){sphere(r=1);}
  }
}

module boxCover() {
  boxWidth = ledWidth*ledCols + 2*wall;
  boxHeight = ledHeight*ledRows + 2*wall;
  dispCenterX = wall+3+52+10 + 22.5/2;
  dispCenterY = -32/2 -25/2 + 23/2 + 2.1;
  color("DimGray") {
    difference() {
      translate([0,-(32+wall),ledDepth+wall]){cube([boxWidth, 32+wall-0.5, 2]);}
      translate([dispCenterX, dispCenterY, ledDepth+wall]){cube([21.8+2, 10.9+2, 3*ledDepth], center=true);}
/*
    // Mounting holes Display
      -(32+wall)
    translate([wall+3+52+10,-32/2-25/2,wall]){nodeMCUMountingHole(ledDepth-3.5);}
    translate([wall+3+52+10,-32/2-25/2+23,wall]){nodeMCUMountingHole(ledDepth-3.5);}
    translate([wall+3+52+10+22.5,-32/2-25/2,wall]){nodeMCUMountingHole(ledDepth-3.5);}
    translate([wall+3+52+10+22.5,-32/2-25/2+23,wall]){nodeMCUMountingHole(ledDepth-3.5);}
*/
    }
    
    // stiffener
      translate([10,-wall-0.5,ledDepth]){cube([boxWidth-20, wall, wall]);}
    
    
    // hooks
    translate([wall+0.5,-32 + 5,ledDepth+wall]){hook();}
    translate([boxWidth-wall-0.5,-32 + 5,ledDepth+wall]){rotate([0,0,180]){hook();}}
    translate([wall+0.5,- 5,ledDepth+wall]){hook();}
    translate([boxWidth-wall-0.5,- 5,ledDepth+wall]){rotate([0,0,180]){hook();}}
  }
}

module box() {
  boxWidth = ledWidth*ledCols + 2*wall;
  boxHeight = ledHeight*ledRows + 2*wall;
    
  // Bodenhaltung
  bhDia=15;
  bhH=0.3;
  translate([0, -(32+wall),0]){cylinder(d=bhDia,h=bhH);}
  translate([boxWidth, -(32+wall),0]){cylinder(d=bhDia,h=bhH);}
  translate([0, boxHeight,0]){cylinder(d=bhDia,h=bhH);}
  translate([boxWidth, boxHeight,0]){cylinder(d=bhDia,h=bhH);}
  
  color("DimGray"){
    // LED-Part
    difference() {
      union() {
        difference() {
          cube([boxWidth, boxHeight, ledDepth+wall+3]);
          translate([wall,wall,wall]){cube([boxWidth-2*wall, boxHeight-2*wall, ledDepth+wall+3]);}
          // slit
          translate([wall-1,-1,ledDepth+wall]){cube([boxWidth-2*wall+2, boxHeight+2, 1]);}
        }
        // separators
        for(row=[1:ledRows-1]) {
          translate([0,wall+row*ledHeight-separator/2,0]){
            cube([boxWidth, separator, ledDepth+wall-2]);
          }
        }
        // separators
        for(col=[1:ledCols-1]) {
          translate([wall+col*ledWidth-separator/2,0,0]){
            cube([separator, boxHeight, ledDepth+wall-2]);
          }
        }
        // add hooks
        for(col=[0:ledCols]) {
          translate([wall+col*ledWidth,0,wall+2+2*separator]){
            rotate([-90,0,0]){cylinder(d=3*separator,h=boxHeight,$fs=.5);}
          }
        }
      }

      // Mounting hole
      translate([boxWidth/2,boxHeight-20,-wall+1]){cylinder(d=10,h=2*wall);}
      hull() {
        translate([boxWidth/2,boxHeight-20,-wall+1]){cylinder(d=4,h=2*wall);}
        translate([boxWidth/2,boxHeight-wall-5,-wall+1]){cylinder(d=4,h=2*wall);}
      }
      hull() {
        translate([boxWidth/2,boxHeight-20,wall+.001]){cylinder(d=10,h=4);}
        translate([boxWidth/2,boxHeight-wall-5,wall+.001]){cylinder(d=10,h=4);}
      }
      // LED-Stripe holes
      for(col=[0:ledCols-1]) {
        translate([wall+col*ledWidth-separator/2+ledWidth/2-11/2,-wall,wall]){
          cube([11, boxHeight, 3.5]);
        }
      }
    }
          
    // NodeMCU-Part
    translate([0,-(32+wall),0]) {
      difference() {
        cube([boxWidth, 32+wall, ledDepth+wall]);
        translate([wall,wall,wall]){cube([boxWidth-2*wall, 32+wall, ledDepth+wall]);}
        // USB-Plug hole
        hull() {
          translate([-1,wall+32/2+3,wall+9+2.7]){rotate([0,90,0]){cylinder(d=7,h=2*wall);}}
          translate([-1,wall+32/2-1,wall+9+2.7]){rotate([0,90,0]){cylinder(d=7,h=2*wall);}}
        }
        // hooks
        translate([wall+0.25, wall + 5,ledDepth+wall]){hook();}
        translate([boxWidth-wall-0.25, wall + 5,ledDepth+wall]){rotate([0,0,180]){hook();}}
        translate([wall+0.25, (32+wall) - 5,ledDepth+wall]){hook();}
        translate([boxWidth-wall-0.25, (32+wall)- 5,ledDepth+wall]){rotate([0,0,180]){hook();}}
      }
    }
    // Mounting holes NodeMCU
    translate([wall+3,-32/2+25/2,wall]){nodeMCUMountingHole(9);}
    translate([wall+3,-32/2-25/2,wall]){nodeMCUMountingHole(9);}
    translate([wall+3+52,-32/2+25/2,wall]){nodeMCUMountingHole(9);}
    translate([wall+3+52,-32/2-25/2,wall]){nodeMCUMountingHole(9);}
    // Mounting holes Display
    translate([wall+3+52+10,-32/2-25/2,wall]){displayMountingHole(ledDepth-3.5,180);}
    translate([wall+3+52+10,-32/2-25/2+23,wall]){displayMountingHole(ledDepth-3.5,180);}
    translate([wall+3+52+10+22.5,-32/2-25/2,wall]){displayMountingHole(ledDepth-3.5,0);}
    translate([wall+3+52+10+22.5,-32/2-25/2+23,wall]){displayMountingHole(ledDepth-3.5,0);}
  }
}

module nodeMCUMountingHole(height) {
    difference() {
        cylinder(d=7, h=height,center=false);
        cylinder(d=3, h=height+1,center=false);
    }
}

module displayMountingHole(height, rot=0) {
    rotate([0,0,rot]){
        translate([-2,-2,0]){cube([6,4,height-4]);}
        translate([-2,-2,0]){cube([3,4,height]);}
        translate([0,0,height]){sphere(d=1.5,$fn=20);}
        translate([2,-2,0]){cube([2,4,height+2.5]);}
        translate([2,0,height+1.75]){sphere(d=1.5,$fn=20);}
    }
}

module diffusor() {
  boxWidth = ledWidth*ledCols;
  boxHeight = ledHeight*ledRows;
  color( "SteelBlue", 0.3 ) {
    difference() {
      // one big cube
      translate([separator/2,separator/2,0]){
        cube([boxWidth-separator, boxHeight-separator, ledDepth-1]);
      }
      
      // substract col main volume
      for(col=[0:ledCols-1]) {
        translate([col*ledWidth+separator/2+hookThickness+separator/2,-1,1]){
          cube([ledWidth-2*hookThickness-2*separator, boxHeight+2, ledDepth]);
        }
      }
      // substract all inner hooks
      translate([separator+hookThickness,-1,1]){
        cube([boxWidth-2*separator-2*hookThickness, boxHeight+2, ledDepth]);
      }
      // substract row main volume
      /*for(row=[0:ledRows-1]) {
        translate([-1,separator+2+row*ledHeight,1]){
          cube([boxWidth+2, ledHeight-2*2-2*separator, ledDepth]);
        }
      }*/
      // substract spacing for row separators
      for(row=[1:ledRows-1]) {
        translate([-1,row*ledHeight-separator-2,1]){
          cube([boxWidth+2, 2*separator+4, ledDepth]);
        }
      }
      // substract spacing for col separators
      for(col=[1:ledCols-1]) {
        translate([col*ledWidth-separator,-1,1]){
          cube([2*separator, boxHeight+2, ledDepth]);
        }
      }
      // substract hook groove
      for(col=[0:ledCols]) {
        translate([col*ledWidth,0,ledDepth-2-2*separator]){
          rotate([-90,0,0]){cylinder(d=3*separator,h=boxHeight,$fs=.5);}
        }
      }
    }
  }
}
