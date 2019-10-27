# Introduction
Calcium-silicate-hydrates (C-S-H) is the main binding materials in the cement hydrates. To dates, its atomistic configuration remains unclear and is under extensive discussion. [Nat. Comm.](https://doi.org/10.1038/ncomms5960) [ACS. JPCC.](https://doi.org/10.1021/acs.jpcc.7b02439) However, a straightforward model construction method is still absent and lack of handy tool to build the tailored compositions of C-S-H. __csh4lmp__ realized direct modeling through removing silciate units (SiO2), appending hydrogen (H) to form hydroxyl group, and inserting water molecules (H2O) based on the anhydrous supercell of mineral tobermorites. 

![Removal Regime](/image/Removal.png)
Removing regime to generate higher Ca/Si ratio. The atoms in gray color are removed in neutral SiO2 groups including one silicon, one bridging and one hydroxyl oxygen as an example marked in yellow circle. Green, purple and orange rectangular represent dimer, pentamer and octamer respectively.

[Chiang Yuan](www.linkedin.com/in/yuanchiang) Mar/2019 @ NTUCE

# Build

```
make
```

# Command

- __I/O__ : input and output of LAMMPS data file

**_input_**
```
	< [input file name]
```
**_output_**
```
	> [output file name] hint (y/n)
```
- __add water__ : add water molecules to current model

```
	-aw add [#] [typeO] [typeH]        
```
syntax | definition
------ | ----------
\# | addition number of water molecules
typeO | added oxygen type
typeH | added hydrogen type

- __delete water__ : remove all of water molecules
```
	-dw delete [typeO] [typeH]
```
syntax | definition
------ | ----------
typeO | removed oxygen type in water molecules
typeH | removed hydrogen type in water molecules

- __remove SiO2__ : remove multiple silicate units from current models
```
	-s number [typeSi] [#] [BT/PT ratio]
```
syntax | definition
------ | ----------
typeSi | silicon type
\# | removal number of silicate units
BT/PT ratio | ratio of removal number of bridging tetrahedra (BT) to pairing tetrahedra (PT)

The example of effect of BT/PT removal ratio on the Qn factors
![BT/PT](/image/1.57.jpg)


- __add H__ : append multiple hydrogen to undercoordinated oxygen
```
	-h valence (n/#) [typeO] [typeH] [typeO-H] 
```
syntax | definition
------ | ----------
n/\# | _n_ : append hydrogen to meet electroneutrality / number of appended hydrogen
typeO | atom type of oxygen to be bonded
typeH | atom type of hydrogen to be added
typeO-H | bond type of new bonds between O and H

- __delete atom__ : delete atoms with specific type
```
	-d delete atom type [type]
```
syntax | definition
------ | ----------
type | atom type of atoms to be deleted
- __delete bond__ :
```
	-d delete bond type [type]                 
```
syntax | definition
------ | ----------
type | bond type of bonds to be deleted
- __change box__ :
```
	-c boundary [pbcx] [pbc] [pbcz]                 
```
syntax | definition
------ | ----------
pbcx pbcy pbcz | periodic boundary condition in x y z direction = p (periodic), m (non-periodic)

This command will reset the image flag consistent and migrate the atoms whose bonds across boundary
# License
MIT
