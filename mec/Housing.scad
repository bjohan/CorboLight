module fanHoles(t, d){
    $fn=100;

            cylinder(t, 24,24);
            translate([20, 20, 0])
                cylinder(t, d/2, d/2, $fn=10);
            translate([20, -20, 0])
                cylinder(t, d/2, d/2, $fn=10);
            translate([-20, 20, 0])
                cylinder(t, d/2, d/2, $fn=10);
            translate([-20, -20, 0])
                cylinder(t, d/2, d/2, $fn=10);
}

pcbX = 100;
pcbY = 50;
pcbZ = 2;
H = 30;
wall = 2;

module pcb(){
    color([0,0.5,0])
    cube([pcbX, pcbY, pcbZ]);
}

module pcbComponents(){
    //power connector
    translate([pcbX-20-10, -6, 0])
        color([1,0.5,0.0]) cube([10,13,8]);
    
    aw = 18;
    al = 40;
    at = 2;
    sw = 2;
    sh = 11;
    //arduino board
    translate([pcbX/2-aw/2, pcbY-al+2, sh])
        color([0,0,1]) cube([aw, al, at]);
    translate([pcbX/2-8/2, pcbY-10+2+1, sh+2])
        color([0.5,0.5,0.5]) cube([8, 10, 3]);
    
    //Arduino socket
    translate([pcbX/2-aw/2, pcbY-al+2, 0])
        color([0,0,0]) cube([2, al-2, sh]);
    translate([pcbX/2+aw/2-2, pcbY-al+2, 0])
        color([0,0,0]) cube([2, al-2, sh]);
    
    //Inductor volume
    translate([0,pcbY-12-2,0])
        cube([pcbX, 12, 3]);
        
   //Jumper volume
   translate([53,0,0])
   cube([14,8, 13]);
   
   //Coolers
   translate([0,13,0])
        color([0.5,0.5,0.5]) cube([41, 22, 16]);
   translate([pcbX-41,13,0])
        color([0.5,0.5,0.5]) cube([41, 22, 16]); 
}

module pcbComponentsExtraVolume(){
    //power connector
    translate([pcbX-20-10, -6, 0-pcbZ])
        color([1,0.5,0.0]) cube([10,13,8+pcbZ+1]);
    
    //Hole for 7805
    translate([pcbX-2, 0, -pcbZ])
        cube([10, 12, 22+pcbZ]);
    translate([pcbX-8, 4, -pcbZ])
        cylinder(15, 4, 4);
    
    aw = 18;
    al = 40;
    at = 2;
    sw = 2;
    sh = 11;
    //arduino board
    translate([pcbX/2-aw/2, pcbY-al+2+wall, -pcbZ])
        color([0,0,1]) cube([aw, al+wall, at+sh+pcbZ]);
    //translate([pcbX/2-8/2, pcbY-10+2+1, -pcbZ])
    //    color([0.5,0.5,0.5]) cube([8+pcbZ+1, 10, 3]);
    
    //Arduino socket
    //translate([pcbX/2-aw/2, pcbY-al+2, 0])
    //    color([0,0,0]) cube([2, al-2, sh]);
    //translate([pcbX/2+aw/2-2, pcbY-al+2, 0])
    //    color([0,0,0]) cube([2, al-2, sh]);
    
    //Inductor volume
    //translate([0,pcbY-12-2,0])
    //    cube([pcbX, 12, 3]);
    
}

module pcbWithComponents(){
    pcb();
    translate([0,0,2]){
        pcbComponents();
        pcbComponentsExtraVolume();
    }
}

module mainBoxShell(xi, yi, zi, ledgew, ledgeh, wall){
    color([0,0,0]);
    difference(){
        translate([-wall, -wall, 0])
            cube([xi+2*wall, yi+2*wall, zi+wall]);
        translate([ledgew,ledgew,-1])
            cube([xi-ledgew*2, yi-ledgew*2, zi+1]);
        translate([0,0,-1])
            cube([xi, yi, ledgeh+1]);
    }
}

module mainBoxWithFanHole(){
    difference(){
        mainBoxShell(pcbX, pcbY, H, 1.5,pcbZ, wall);
        translate([pcbX/2, pcbY/2, 0]){
            translate([0, 0, H-1])
                fanHoles(wall+2,3);
            $fn=100;
            hull(){
               
                fanHoles(H, 11.5);
            }
        }
    }
}


module boxWithComponentHoles(){
    difference(){
        mainBoxWithFanHole();
        minkowski(){
            translate([0,0,2]){
                pcbComponents();
                pcbComponentsExtraVolume();
            }
            translate([-0.5,-1,-0.5])
                cube([1,2,1]);
        }
        airHolesLong();
        airHolesShort();
        screwHoles();
    }
}

module airHolesLong(){
    for(i=[0:2]){
        translate([8+i*13, 40, 10])
            rotate([-90,0,0])
                cylinder(100,4,4);
    }
    for(i=[0:2]){
        translate([pcbX-(8+i*13), 40, 10])
            rotate([-90,0,0])
                cylinder(100,4,4);
    }
}

module screwHoles(){
        translate([15, -20, 20])
            rotate([-90,0,0])
                cylinder(100,3/2,3/2);
        translate([pcbX-15, -20, 20])
            rotate([-90,0,0])
                cylinder(100,3/2,3/2);
        //translate([pcbX/2, 0, 5])
        //    rotate([-90,0,0])
        //        cylinder(100,3/2,3/2);

}

module airHolesShort(){
    sep = 7;
    for(y=[-1:1])
        for(z=[-1:1]){
            translate([-10,pcbY/2+y*sep,H/2+z*sep])
                rotate([0,90,0])
                    cylinder(pcbX+20, 2,2);
        }
}

module airGuide(){
    
}


module pcbClamp(){
    translate([-2*wall, -2*wall, -wall]){
        difference(){
            cube([30,30,30+3*wall]);
            translate([7,7,-1])
                cube([30,30,30+3*wall+2]);
            translate([wall, wall, wall])
                cube([30,30,30]);
            translate([-wall, 10, wall])
                cube([30,30,30]);
            translate([10, -wall, wall])
                cube([30,30,30]);
        }
    }
}
//pcbWithComponents();
rotate([180,0,0]){
//    translate([wall+3, wall+3, 0]){
    //boxWithComponentHoles();
    pcbClamp();
    //pcbWithComponents();
//    }
    bx = pcbX+2*wall+6;
    by = pcbY+2*wall+6;
    bz = H+wall;
/*
difference(){
    //translate([0, 0, 0]){
    cube([bx+1, by+1, bz]);
    translate([0.5, 0.5,0])
        cube([bx, by, bz]);
    //}
}*/}
//airHolesShort();
//airHoles();

//pcbWithComponents();