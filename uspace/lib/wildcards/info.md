# Wildcard support for HelenOS - MSoC Project by Patrik Pritrsky  

In this project, I added support for wildcards in the HelenOS shell.  
So, if the user wants to delete all text files in a directory, they just  
need to type 'rm *.txt', instead of listing them all one by one.  

## Features  

### Standard wildcard *  

Expands to zero or more characters, evaluation happens recursively at  
all levels where it occurs.  

For example, 'folder*/file*.txt' will find all text files starting with  
'file', in all subdirectories of the current directory that start with  
'folder'.  

### Recursive wildcard **  

Used to find files at arbitrary depth.  

For example, '**/*.txt' will find all text files, at any depth within  
the current directory.  

## List of changes to HelenOS  

- Added automated tests for wildcards  
- Created a function to detect whether a string contains a wildcard  
- Created a function to check whether a wildcard pattern matches a  
  file/directory name  
- Created a function for recursive expansion and finding all  
  occurrences of files/directories that match a path/filename containing  
  wildcards  
- Modified the HelenOS shell tokenizer to support wildcard expansion

# Podpora zástupných znaků (wildcards) v HelenOSím shellu - MSoC Project by Patrik Pritrsky  

V tomto projekte som pridal podporu pre zástupné znaky (wildcards) v HelenOS shelle. 
Teda, ak používateľ bude chcieť vymazať všetky textové súbory v priečinku, 
tak mu stačí napísať 'rm *.txt', namiesto toho, aby ich všetky vymenoval.

## Funkcie

### Štandardný zástupný znak/wildcard *

Expanduje sa na nula alebo viacero znakov, 
vyhodnocovanie prebieha rekurzívne na všetkých úrovniach, kde sa nachádza.

Teda napríklad 'priecinok*/subor*.txt', nájde všetky textové súbory začínajúce sa na 'subor',
vo všetkých podprečinkoch aktuálneho priečinku začínajúcich sa na 'priecinok'.

### Rekurzívny zástupný znak/wildcard **

Funguje na nájdenie súborov, ktoré sú ľubovoľne hlboko.

Teda napríklad '**/*.txt', nájde všetky textové súbory, ľubovoľne hlboko v aktuálnom priečinku.

## Zoznam zmien do HelenOS

- Pridanie automatizovaných testov pre zástupné znaky
- Vytvorenie funkcie na detekovanie, či reťazec obsahuje zástupný znak
- Vytvorenie funkcie na porovnanie, či sa zástupný znak pattern zhoduje s názvom súboru/priečinku
- Vytvorenie funkcie na rekurzívne expandovanie a nájdenie všetkých výskytov súborov/priečinkov,
ktoré sa zhodujú s cestou/názvom súboru obsahujúcim zástupné znaky
- Zmena v tokenizátore HelenOS shellu, tak, aby podporoval expanziu zástupných znakov
