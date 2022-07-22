$fn = 100;

sw_dist = 19;
sw_x = 14;
// keycap_x = 18;

sw_o = 4;
sw_os = [0, sw_o, sw_o * 2, sw_o * 2, sw_o * 2, sw_o, 0];

p_x = sw_dist * 7;
p_y = sw_dist * 6 + 2 * sw_o;

plate_z = 5;
plate_w = 1.5;
pcb_z = 1;

c_r = 2;
c_h = 0.5;

p0_z = 1.5;
p1_z = 3.5;
p2_z = 3.5;
p3_z = 1;

usb_x = 10;
usb_y = 20;
usb_z = p2_z;

rgb_r = 2.5;
rgb_x = 5;
rgb_y = 5;
rgb_z = 3;

sw_x1 = 5;
sw_y1 = 1;

module hole(di) {
    // 4 corner standoff hole
    for(x1 = [5, p_x - 5],  x2 = [sw_dist, p_x - sw_dist]) {
        for(i = [[x1, p_y - 5, 0], [x2, 5, 0]]) {
            translate(i) {
                cylinder(p2_z, d = di);
            }
        }
    }
    
    // center hole
    for(i = [0, 2, 4, 6], j = [2, 4]) {
        translate([sw_dist * (i + 0.5), sw_os[i] + sw_dist * j, 0]) cylinder(p2_z, d = di);
    }
}

module plate(x, y, z) {
  translate([c_r, c_r, 0]) minkowski() {
    cube([x - c_r * 2, y - c_r * 2, z - c_h]);
    cylinder(h = c_h, r = c_r);
  }
}


p2_x1 = 10;
p2_y1 = 8;

p2_x4 = 10;
p2_y4 = 10;

p2_y5 = 24;

p2_x6 = 13;
p2_y6 = 7 + p2_y1 / 2;

p2_x7 = 6;
p2_y7 = p2_x7;


/* color("Violet", 1) translate([0,0,1]) import("./pcb/sok42-B_Courtyard.svg"); */

module sw() {
    translate([0, 0, plate_z / 2]) cube([sw_x, sw_x, plate_z], true);
    for(i = [-sw_x / 2 - sw_y1, sw_x / 2])
        translate([-sw_x1 / 2, i, 0]) cube([sw_x1, sw_y1, p1_z]);
}


// plate
/* projection(cut = false) */
translate([0, 0, 20]) {
    // p0
    translate([0, 0, p1_z])
    difference() {
        plate(p_x, p_y, p0_z);
        
        // sw
        for(i = [0 : 6]) {
            translate([0, sw_os[i], 0]) for(j = [0 : 5]) {
                translate([(i + 0.5) * sw_dist, (j + 0.5) * sw_dist, 0]) 
                    translate([0, 0, plate_z / 2]) cube([sw_x, sw_x, plate_z], true);
            }
        }

        hole(2);
    }
    
    // p1
    difference() {
        plate(p_x, p_y, p1_z);
        
        // sw
        for(i = [0 : 6]) {
            translate([0, sw_os[i], 0]) for(j = [0 : 5]) {
                translate([(i + 0.5) * sw_dist, (j + 0.5) * sw_dist, 0]) sw();
            }
        }
        
        hole(3.5);
        
        // front rgb
        for(i = [0 : 3]) translate([42.5 + i * 16, 4.5, 0])
            cylinder(h = p1_z, r = rgb_r);
    }
}


// p2 case edge frame
/* projection(cut = false) */
translate([0, 0, p3_z])
difference() {
    union() {
        difference() {
            // outside
            plate(p_x, p_y, p2_z);
            
            // inside
                // cut out
                for(i = [0:5]) {
                    // row
                  translate([0, sw_dist * (i + 0.5), p2_z / 2]) union() {
                    for(j = [0:6]){
                      translate([sw_dist * (j + 0.5), sw_os[j], 0]) cube([p2_x1, p2_y1, p2_z], true);
                    }
                  }
                }
        
            // rgb
            for(y = [0:4]) {
                if(y == 0 || y == 4) {
                  for (x = [1 : 6]) {
                    translate([x * sw_dist, p2_y5 + y * sw_dist, p2_z / 2]) {
                      cube([rgb_x, rgb_y, p2_z], true);
                  }
                    }
                } else {
                    for (x = [1, 6]) {
                      translate([x * sw_dist, p2_y5 + y * sw_dist, p2_z / 2]) {
                          cube([rgb_x, rgb_y, p2_z], true);
                        }
                    }
                }
            }

            // mcus
            translate([sw_dist * 3.5, sw_o * 2 + sw_dist * 3, p2_z / 2]) cube([110, 30, p2_z], true);
        
            for (i = [1, 6]) {
                // usb
                translate([sw_dist * i - usb_x / 2, p_y - usb_y, 0])
                    cube([usb_x, usb_y, usb_z]);
            }

            for(i = [1, 5]) {
              // boot switch
              translate([sw_dist * (i + 0.5), sw_dist * 2 + sw_os[1], p2_z / 2])
                cube([10, 12, p2_z], true);
            }
        }
        
        // hole strength
        for(i = [5, p_x- 5]) {
          translate([i, p_y - 5, 0]) {
            translate([-p2_x7 / 2, -p2_y7 / 2, 0]) plate(p2_x7, p2_y7, p2_z);
          }
        }
        for(i = [1, 7]) translate([sw_dist * (i - 0.5), sw_dist * 4, p2_z / 2]) cube([14, 11, p2_z], true);

    }

    // mos
    for (i = [0 : 6]) {
      translate([(i + 0.5) * sw_dist, 4.5 * sw_dist + sw_os[i] - 5, p2_z / 2]) {
        cube([6, 5, p2_z], true);
      }
    }
    
    hole(3.5);
}


// p3 bottom plate
/* !projection(cut = false) */
difference() {
  plate(p_x, p_y, p3_z);
  hole(2);
}
