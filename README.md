# split keyboard

split keyboard created in kicad and cad

## what is this project

this is a split keyboard keyboard divided into 2 parts.
i made this to gain experience in pcb design, cad modelling and firmware development.
it has 22 keys per side, an oled display, and rotary encoder.

## why i made it

i wanted to create my own keyboard to get more experience working with hardware project.
i got a lot of information about schematic creation, pcb routing, footprints, cad case modelling and file structure for github.

## what is inside

- split keyboard pcb
- cad case 
- firmware file
- gerber files
- project images
- bom csv

## folder structure

```text
.
├── README.md
├── cad
│   ├── assembled.f3z
│   ├── assembled.step
│   └── pcb3d[pcb only].step
├── firmware
│   └── main.cpp
├── grb
│   ├── split-keyboard-kicad-B_Cu.gbr
│   ├── split-keyboard-kicad-B_Mask.gbr
│   ├── split-keyboard-kicad-B_Paste.gbr
│   ├── split-keyboard-kicad-B_Silkscreen.gbr
│   ├── split-keyboard-kicad-Edge_Cuts.gbr
│   ├── split-keyboard-kicad-F_Cu.gbr
│   ├── split-keyboard-kicad-F_Mask.gbr
│   ├── split-keyboard-kicad-F_Paste.gbr
│   ├── split-keyboard-kicad-F_Silkscreen.gbr
│   ├── split-keyboard-kicad-NPTH.drl
│   ├── split-keyboard-kicad-PTH.drl
│   └── split-keyboard-kicad-job.gbrjob
├── grb.zip
├── image
│   ├── pcb3d-back.png
│   ├── pcbRoute.png
│   ├── pdb3d-front.png
│   ├── schematic-left.png
│   └── schematic-right.png
└── kicad
    ├── left.kicad_sch
    ├── right.kicad_sch
    ├── split-keyboard-kicad.kicad_pcb
    ├── split-keyboard-kicad.kicad_pro
    └── split-keyboard-kicad.kicad_sch
```


## Images

### Schematic Left

![Schematic Left](image/schematic-left.png)

### Schematic Right

![Schematic Right](image/schematic-right.png)

### PCB Routing

![PCB Routing](image/pcbRoute.png)

### PCB 3D Front

![PCB 3D Front](image/pdb3d-front.png)

### PCB 3D Back

![PCB 3D Back](image/pcb3d-back.png)

### PCB Order

![PCB Order](image/pcborder.png)

### Keycaps

![Keycaps](image/keycapsorder.png)

### Cherry MX Switches

![Cherry MX Switches](image/mxcherry.png)

## 3D CAD Model

### Assembled Keyboard

CAD files are available in the `cad/` folder.

* `assembled.f3z`
* `assembled.step`

### PCB 3D Model

* `pcb3d[pcb only].step`

## Repository Structure

```text
split-keyboard/
│
├── cad/
│   ├── assembled.f3z
│   ├── assembled.step
│   └── pcb3d[pcb only].step
│
├── firmware/
│   └── main.cpp
│
├── grb/
│   └── PCB manufacturing files
│
├── image/
│   └── Project screenshots and renders
│
├── kicad/
│   └── KiCad project files
│
├── grb.zip
├── BOM.csv
└── README.md
```
## Files

* `firmware/` – source code for keyboard firmware
* `cad/` – 3D CAD models
* `image/` – screenshots, renders, and reference images
* `kicad/` – schematics, PCBs, and KiCad project files
* `grb/` – gerber files for PCB manufacture
* `grb.zip` – zipped gerber files
* `BOM.csv` – bill of materials

## build notes

- i used kicad for my schematics and PCB design
- i designed the keyboard case in cad
- i exported gerber files for manufacture
- i kept the design simple to make it easy to build and code


## BOM

| Name | Purpose | Quantity | Total Cost (USD) | Link | Distributor |
| --- | --- | ---: | ---: | --- | --- |
| raspberry pi pico board | devboard to code on | 2 | $8.11 | https://robu.in/product/raspberry-pi-pico/ | robu |
| rotary encoder | knob control | 2 | $4.00 | https://robu.in/product/hongyan-ec11h-7ce15p1zy15f7-rotary-encoder-with-push-button-switch-vertical-plug-in/ | robu |
| pcb | to soilder all things onn | 1 | $46.00 | https://robu.in/product/online-pcb-manufacturing-service/ | robu |
| keycaps | to put above mx keys which give more surface area to click the button | 1 | $22.00 | https://meckeys.com/shop/accessories/keyboard-accessories/keycaps/astronaut-keycap-set/ | meckeys |
| mx cherry keys | to make button works, it is the buttons to press | 44 | $17.00 | https://www.amazon.in/Keyboard-Switches-Dust-Proof-Keycap-Puller/dp/B0GZVJZP6V/ref=sr_1_1?crid=1EHWTYDETVJK0&dib=eyJ2IjoiMSJ9.EPKf_Chov9UdC52FZTZhqD9gxnQ2H53JUXcx-ExCRIuWW5M0qEhEaj_EA4Hh8svkmQaRo_iwuATTiiKnUm_249SRJG3SZchNICn6NqN_LnjG7vWbbi8DNZnYo_UyOs1PBskO9rMlKtHwcH0_-CwgYq2Hd9GEhnLxf2cab6ss66qjs4INdDIkbpSQtbI0jD_gbd9Xzn6TXAXXerfTdFoA0XUWCOw7caRT4xZMyWuczbQ.EnDgPv4DcOxBSh3BzIr-p_Og38qAgpwU2yX1HpgDfDA&dib_tag=se&keywords=cherry+mx+switches&nsdOptOutParam=true&qid=1782203293&sprefix=mx+cherry%2Caps%2C295&sr=8-1 | amazon |
| 1N4148 diodes | key matrix isolation | 44 | $3.00 | https://robu.in/product/diode-1n4148/ | robu |


## files

- `kicad/` for all pcb and schematic files
- `cad/` for the 3d model and case files
- `grb/` for gerber files
- `image/` for project pictures
- `firmware/` for code

## note

this project is still being worked on, and i will keep improving the case, layout, and code.
