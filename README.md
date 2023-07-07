# Mouse-Push-Pull-Lever-Task

## Task Design

The task design is as follows in the diagram below:

![Task Design](https://github.com/RajHarsor/Mouse-Push-Pull-Lever-Task/blob/main/Instructions%20+%20README/TaskDesign.png)

## Creating a Behavior Box

### Materials

#### Structure

To create the behavior box, there are some structural components that you will need. These include:

| Name of Item  | Item Catalog #  | Supplier | Quantity | Link |
|---|---|---|---|---|
| Aluminum Breadboard, 6" x 12" x 1/2", 1/4"-20 Taps | MB612F | ThorLabs | 1 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=MB612F |
| Mounting Base, 1" x 2.3" x 3/8 | BA1S | ThorLabs | 1 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=BA1S |
| Ø1/2" Post Holder, Spring-Loaded Hex-Locking Thumbscrew, L = 3"    | PH3   | ThorLabs | 1 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=PH3 |
| Ø1/2" Post Holder, Spring-Loaded Hex-Locking Thumbscrew, L = 2" | PH2 | ThorLabs | 2 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=PH2 |
| Ø1/2" Optical Post, SS, 8-32 Setscrew, 1/4"-20 Tap, L = 10" | TR10 | ThorLabs | 1 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=TR10 |
| Right-Angle Clamp for Ø1/2" Posts, 3/16" Hex | RA90 | ThorLabs | 4 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=RA90 |
|Ø1/2" Optical Post, SS, 8-32 Setscrew, 1/4"-20 Tap, L = 6"| TR6 | ThorLabs | 4 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=TR6 |
| Ø1/2" Optical Post, SS, 8-32 Setscrew, 1/4"-20 Tap, L = 1" | TR1 | ThorLabs | 1 | https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=159&pn=TR1 |
|Headplate Holder | N/A | Custom-Made (Machine Shop) | 1 | N/A |
|Aluminum Male-Female Hex Thread Adapter 1/4"-20 to M5 X 0.8 mm Thread Size | 91646A216 | McMaster-Carr | 1 | https://www.mcmaster.com/91646A216/ |
| FANOOL Soldering Helping Hands Third Hand Tools Vise Table Clamp PCB Holder - 4 Soft and Flexible Arms with Adjustable Clips for Electronics Repair Assembly Painting Jewelry DIY Arts Craft Hobby | B0BVFNB1S8 | Amazon | 1 | https://www.amazon.com/gp/product/B07Q2ZQX2L/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1 |
| TK84857337T Arm Only Arm Type Articulating Base Height 7.4 mm Base Size 7.4 mm H Base Type Non-Magnetic Compatible Indicator Mounts 6 mm Fine Adjustment Position Fine Adjust Top FAT Holding Power 0 lbf Maximum Reach 178 mm Working Length of Post/Lower Arm | 56LA99 | Grainger | 1 | https://www.grainger.com/product/TRIMOS-Arm-Only-56LA99 |
| 

#### Electronics

To create the behavior box, there are some electronic components that you will need. These include:

| Name of Item  | Item Catalog #  | Supplier | Quantity | Link |
|---|---|---|---|---|
| PJRC Teensy 4.1 ARM Cortex-M7 Processor at 600MHz with a NXP iMXRT1062 (Without pins)| B088JY7P2H | Amazon | 2 | https://www.amazon.com/gp/product/B088JY7P2H/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1 |
| Audio FX sound board | 2220 | Adafruit | 1 | https://www.adafruit.com/product/2220 |
| LHDA0533115H LEE HDI SOV, PTD-5V-30 PSI-V1 | LHDA0533115H | Lee Products | 1 | https://www.leeproducts.com/products/lee-hdi-solenoid-valves/ |
| Joystick TSAT1S02A | 679-3657-ND | Digikey | 1 | https://www.digikey.com/en/products/detail/c-tsats1s02a/679-3657-ND/11223311 |
| Speaker - 3" Diameter - 8 Ohm 1 Watt | 1314 | Adafruit | 1 | https://www.adafruit.com/product/1314 |
| FAYLEZ TIP120 NPN 5 A 60 V Silicon Epitaxial Power Transistor 5 amp 60 Volt Darlington Transistors TO-220 20 PCS | B08B3QZQZ1 | Amazon | 1 | https://www.amazon.com/gp/product/B08B3QZQZ1/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1 |
| MIKEDE Neodymium Bar Magnets 25 Pack, Strong Permanent Rare Earth Magnets with Double-Sided Adhesive, Powerful Metal Neodymium Magnet for Craft, Fridge, Kitchen, Office - 60x10x3mm | B08B3QZQZ1 | Amazon | 2 (Project comes in a pack of 25) | https://www.amazon.com/gp/product/B08B3QZQZ1/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1 |
| Custom Circuit Board | N/A | Aisler | 1 | N/A |

#### 3D Printed Parts (STL Files)

To create the behavior box, there are some 3D printed parts that you will need. These include:

[comment]: <> (#TODO: Add STL Files)

| Part | STL File |
|---|---|
| Mouse Holder | |
| Joystick Holder| |
| Joystick Reel | |
| Joystick Reel Holder | |

### Prerequisite Software Downloads

| Software Name  | Download Link  | Steps
|---|---|---|
| Arduino IDE  | https://www.arduino.cc/en/software#future-version-of-the-arduino-ide  | 1. Go to the link and download the IDE using the correct installer <br> 2. Follow the steps given in the installer (please note to fully install this software you will need Administrator privileges or have someone put in their Administrator password)|
| Teensyduino | https://www.pjrc.com/teensy/td_download.html | Teensyduino is downloaded in the Arduino IDE <br> <br>
| CoolTerm | https://freeware.the-meiers.org/  | 1. Go to the link and scroll down to download the software using the correct installer <br> 2. Once downloaded, open the program <br> 3. Open the .stc files [here] to open up the correct settings configuration 

### Assembly

1. Print the 3D printed parts (STL files) using a 3D printer
2. Put all the ThorLabs components on the board (see image below)
3. Put the 3D printed parts on the board (see the same image below)
4. Upload the sound files onto the Audio FX sound board which can be found here (this should be done before soldering the electronic components onto the custom circuit board)
5. Solder the electronic components onto the custom circuit board (Reference the circuit diagram here)
    5.1. Before soldering the joystick onto the custom circuit board, make sure to thread the wires through the Joystick holder (trust me this will make your life easier)
6. Plug both of the Teensy's into the computer
7. Upload the coordinate code onto the Teensy that is connected to the joystick (this will be the bottom Teensy if you're holding the circuit board upright)
    7.1. To do this step, you can either clone the repository, open the code in the Arduino IDE, and upload it to the Teensy or you can upload it straight from VSCode if you have Intellisense configured and get the Arduino extension working 





