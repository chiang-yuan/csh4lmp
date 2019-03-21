# csh4lmp
The computational modeling tool for custom atomistic model of calcium-silicate-hydrates in cement pastes

2019 Chiang Yuan [LinkedIn](www.linkedin.com/in/yuanchiang)

# Build


# Command

- __add water__

```
	-aw add [#] [typeO] [typeH]        
```
	syntax | definition
	------ | ----------
	\# | addition number of water molecules
	typeO | added oxygen type
	typeH | added hydrogen type

- __delete water__
```
	-dw delete [typeO] [typeH]                 
	-remove SiO2    |    -s number [typeSi] [#] [BT/PT ratio]
	-add H          |    -h valence (n/#) [typeO] [typeH] [typeO-H] 
	-delete atom    |    -d delete atom type [type]                 
	-delete bond    |    -d delete bond type [type]                 
```
# License
MIT
