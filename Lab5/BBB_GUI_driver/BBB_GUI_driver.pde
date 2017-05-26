/* Code Written by Roy Pe'er. I've explained all the code in the grey comments.
I recommend you to go over the code, examine it, play with it, improve it and modify it according to your needs. 
For more awesome tutorials, subsrice to my channel:
http://www.youtube.com/subscription_center?add_user=Roypeer1
*/

import processing.serial.*; //Importing the Serial library.
Serial myPort; // Creating a port variable.
int r,g,b; // initializing colours.
String T= "Beagle Bone Black Driver"; // Creating word strings for the interface.

void setup() {
  size(1000,600); // Creating the display window and defining its' size.
 
  r = 0; // Setting up the colours.
  g = 0;
  b = 0;

  println(Serial.list()[2]); // IMPORTANT: prints the availabe serial ports.
  String portName = Serial.list()[2]; // change the 0 to a 1 or 2 etc, to match your port (Play with it until you find the one that works for you- It's probably 11!)
  myPort = new Serial(this, portName, 115200, 'N', 8, 1); // Initializing the serial port.
}

void draw() {
   
  background(105,105,105); // Setting up the background's colour- Yellow.

  fill (105,105,105); // Painting the Arrows White.
  rect(750, 250, 100, 100,7); // BWD rectangle
  triangle(750, 235, 800, 160, 850, 235); //FWD triangle
  triangle(735, 350, 660, 300, 735, 250); //RHT triangle
  triangle(865, 250, 940, 300, 865, 350); //LFT triangle
  rect(50, 250, 400, 100,5); //BRAKE rectangle
  rect(50, 190, 80, 50, 5); // FIRE rectange
  textSize(32); // Defining the headline's size- 32
  fill (0,0,255); // painting the headline blue.
  //text(T, 25, 50);  // placing the headline in a specific location
  textSize (20); // The arrow keys text size- 20
  fill (0,255,0); // painting it green.
  text("FWD", 780, 220); //FWD
  text("RT", 875, 310);//RHT
  text("LFT", 690, 310); //LFT
  text("BWD", 780, 310);//BWD
  text("BRAKE", 225, 310); //BRAKE
  text("X: Fire", 65, 225); 
}
 
void keyPressed() {
 
  switch (keyCode) { //Switch case: Sending different signals and filling different arrows red according to which button was pressed.   
    
    case UP: //In case the UP button was pressed:
       myPort.write("w\n"); // Send the signal 1
       println("UP"); // + Print "UP!" (Debugging only) 
       fill(255,0,0); // + Fill the up triangle with red.
       triangle(750, 235, 800, 160, 850, 235);
       break;
       
    case DOWN:
       myPort.write("s\n");
       fill(255,0,0);
       rect(750, 250, 100, 100,7);
       break; 
    
    case LEFT:
       myPort.write("a\n");
       fill(255,0,0);
       triangle(735, 350, 660, 300, 735, 250);
       break;
     
    case RIGHT:
       myPort.write("d\n");
       println("RIGHT");
       fill(255,0,0);
       triangle(865, 250, 940, 300, 865, 350);
       break;
  
    case ' ' :
       myPort.write("b\n");
       println("STOP");
       fill(255,0,0);
       rect(50, 250, 400, 100,5);
       break;

    case 'X':
       println("Fire!");
       myPort.write(5);
       fill(255, 0, 0);  
       rect(50, 190, 80, 50, 5);
       break;
  
    default:
       break;  
  }

}