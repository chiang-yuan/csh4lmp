# csh4lmp
Calcium-silicate-hydrates (C-S-H) is the main binding materials in the cement hydrates. To date, its atomistic configuration remains uncertain and is under extensive discussion. [Nat. Comm.](https://doi.org/10.1038/ncomms5960) [ACS. JPCC.](https://doi.org/10.1021/acs.jpcc.7b02439) A straightforward model construction method is still absent and lack a handy tool to build the full atomistic C-S-H with tailored compositions. __csh4lmp__ realized the atomic structure modification through removing silciate monoxdide (SiO), appending hydrogen (H), and inserting water molecules (H2O) based on the anhydrous supercell of [Hamid's mineral 11-$\AA$ tobermorites](https://doi.org/10.1524/zkri.1981.154.3-4.189). 

[Yuan Chiang (CY)](https://www.linkedin.com/in/yuanchiang) Mar/2019 @ NTUCE

![relaxation](/example/relaxation.gif)

![removal regime](/image/removal.jpg)
This figure shows how to achieve higer Ca/Si ratio through removing silicate units. The atoms in gray color are removed in neutral SiO2 groups including one silicon atom and two oxgygen atoms. Additional hydrogen atoms are appneded to the undercoordinated oxygen atoms. One appended hydrogen atom and one oxygen atom are bonded together and become a hydroxyl group where the oxygen atom turns into a hydroxyl oxygen atom. 

## Nomenclature and Color Convention

| Type | Name | Element | Color |
| --- | --- | --- | --- |
| 1 | O (oxygen) | oxygen | red |
| 2 | Ca (intralayer calcium) | calcium | blue |
| 3 | Si (silicon) | silicon | brown | 
| 4 | H (hydrogen) | hydrogen | white |
| 5 | Oh (hydroxyl oxygen) | oxygen | yellow |
| 6 | Ow (oxygen in water) | oxygen | cyan |
| 7 | Cw (interlayer calcium) | calcium | magenta |
| 8 | Hw (hydrogen in water) | hydrogen | white |
| 9 | Ob (bridging oxygen) | oxygen | purple |

# Build

```
make -f Makefile
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

Effect of BT/PT removal ratio on the silicate connectivity indicated through Qn factors
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
	-c boundary [pbcx] [pbcy] [pbcz]                 
```
syntax | definition
------ | ----------
pbcx pbcy pbcz | periodic boundary condition in x y z direction = p (periodic), f (non-periodic)

This command will reset and make the image flag consistent and traslate the atoms whose bonds across boundary
# License
MIT
