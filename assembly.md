Na azért atmegyek az alapokon is csak hogy teljes legyen.
(Előre is sorry nincs hosszú i a laptopomon xd)

Az assembly a gépi kód aka nullák és eggyesek egy dialektusa. Avagy gyakorlatilag nem kell forditani eggyiket a masikra - ezt úgy is szoktak mondani hogy 1 az 1hez aranyulnak.

Az assemblyt két felé képpen tudod leírni mindkettő ugyan az csak az eggyik érthetőbb az embereknek. Az eggyok a hexadecimalis bytokkal (pl E9 DE AD BE EF) a másik pedig a szöveges (pl mov eax, [edi])

Az assembly és ezaltal a gépi kód is minden procin más. Manapság a számítógépekben hasznalt assemblyk elég hasonlóak de minden procival jön egy két új utasítás.

Amit mi fogunk tanul az az Intel x86 és Intel x64
És ha nem felejtem el majd azért megemlitem az amd és arm eseket is. Szerencsekre az inteles a legatlathatobb.

Igyekszem most csak a szükséges dolgokat leírni.
# Registerek
## General Purpose
a,b,c,d,source,destination,stack pointer,base pointer.
Ezeknek a mérete 32 vagy 64 bit attól függően hogy hány bites a processzorod.

Nem mindig van szükségünk egy egész registerre ezért mindeggyik register részekre van bontva. Például most vegyük az A registert.
![rax](rax.png)

És akkor mégegyszer a registerek ahogy általában látod őket, plusz hogy mit csinálnak. (amihez nem irok semmit azt gyakorlatilag arra használod mindig amire épp kell)

EAX - Ebben a registerben van a **return** értéke a meghivott funkciónak. 
EBX 
ECX
EDX
ESI - Source pointer      
EDI - Destination pointer,  az  esivel eggyüt szokás használni pédául két memória swapolására , ezekbe is szeretnek passolni argumentumokat a funkciók
ESP - Stack pointer, Megmutatja hogy épp hol vagyunk a stacken de át lehet irni
EBP - **Mindig** az adott funkcio kezdő cime van benen , gyakorlatilag nem is lenne erre szükség de egyszerűsiti a debuggolást

ezen felül szokott lenni még
R7-től egészen R15-ig 8db extra register arra hsaználod őket amire szeretnéd. Többet nem esik róluk szó ,nem olyan fontosak.

## FLAGS

EFLAGS 32 biten

Minden bitjének saját jelentése van és a különböző utasitások hatására változik az értékük.
Nem olyan fontos ezeket se tudni majd ha eljutunk a CMP ig akkor lesz néha érdekes.

De csak a példa kedvéért

```S
mov eax,1    ; eax ba rakunk 1 et
sub eax,1    ; kivonunk eax értékéből eggyet
jz UGRO_CIM  ;jump zero instrukcio akkor ugrik ha a zero flag be van nyomva

```

## Instruction Pointer

EIP - Mindig a jelenlegi utasitásra mutat. Alapból eggyesébel magától növekedik. A jump utasitásokkal lehet például változtatni.

## 64 bit
Egyetlen egy dolgot érdemes megjegyezni róla.
64 biten **NINCS** EIP,ESI,EDI,EBP,ESP register (mivel ezek 32 bites változatok)
Ezeknek csak a 64 bites verzióját fogod tudni használni 64 bites programban.
RIP,RSI,RDI,RBP,RSP

# A program felépitése
## Assemblyben
3 részből áll

### .text

Ez az a része a programnak ahova meguk az utasitások kerülnek.

### .data

ide kerülnek a hardcodolt változók

```c++
const char bytes[5] = {0xE9,0xDE,0xAD,0xBE,0xEF};
printf("Hello world"); //A Hello Word se varázslatosan lett ott, ezt is előzetesen tároljuk a memóriában
```

### .rss

félrerakot memória olyan változóknak aminek még nem tudom az értékét

```c++
std::cout << "Hogy hivnak?\n";
std::string name;
std::cin >> name;
std::cout << "Szia " << name.c_str(); 
```

## Az os részéről

Amikor elinditasz egy programot annak a méreténél egy kicsit több memóriát fog neki biztositani az oprendszer.
Ide betölt a teljes program minden utasitással.
De még a programok elött lesz egy header a memóriában ez olyan információkat táról a programról mint például a jelenlegi 
**Base Address** azaz hogy hol kezdődik a program, a **Module Size** ami megmondja hogy milyen hosszú a program, vagy pédául azt is tárolja hogy a memóriában hova szeret betöltődni a programod.

A stack **gyors** de a mérete **véges**. Mindennek amit a stackre kerül **előre tudnom kell a méretét**. Ebből adódig a probélma hogy mi van akkor ha nem tudom előre mekkora lesz egy tömb (vecotor). A válasz A **Heap**, a heap gyakorlatilag végtelen memória ,mindig szépen megkérjük az os-t hogy kéne még memória az pedig biztosit nekünk valahol egy táboli memória cimen a heapen tárhelyet és cserébe vissza ad egy pointert. A pointerek mérete **állandó** és ezáltal lehetnek a stacken.


# Alap utasitások

### MOV, LEA
Átrak egy értéket az eggyik helyről a másikra.

```S
mov eax,1     ;eax ba beleteszünk eggyet
mov ebx,2     ;ebx be kettőt
mov eax,ebx   ;eax ba beletesszük ebx értékét azaz kettőt

;int a =3;
;int*b =&a;
;ITT MOST KEVERM A CPP-t és az assemblyt erre van lehetőséged visual studioban 
;A valóságban nem lesznek ilyen változók assemblyben hogy a vagy hogy b  később megmutatom ezek hogynéznek ki valójában

mov eax,a     ;beleteszzük eaxba a értékét ami 3
mov ebx,b     ;ebx ba beleteszük b értékét ami egy pointer szóval ebx ben most egy pointer van és NEM 3
mov eax,[ebx] ;beletesszük eax ba azt AMIRE mutat ebx
```

A példából látszik hogy assemblyben a kockás zárójelek ugyan azt csinálják mint a csillag a egy pointer előtt.
Például:
```S
mov eax,3       ;int a = 3;
lea ebx,eax     ;int *b=&a       
mov [ebx],4     ;*b=4;
```
Na de mit csinál a lea? Nagyon egyszerú gyakorlatilag a c++ ból ismert **&** jel megfelelője tehát a változó értéke **helyett a cimét** teszi a registerbe.
Például
```S
mov eax,3     ;int a=3
mov ebx,eax   ;int b=a;
lea ecx,eax   ;int*c=&a;
```


# Arhimetika és Logika
**ADD,SUB,MUL,IMUL,DIV,IDIV,AND,OR,XOR**
(az i betű az idiv ben és imul ban az integert jelenti és egyben arra utal hogy ezek lehetnek minuszosak azaz signed számokról van szó, mig a sima mul és div unsigned.)
Gyakorlatialg ugyan úgy müködik mint a mov.
Ez talán a legegyszerűbb az egészből, úgyhogy csak pár pédát irok.

```S
mov eax,3
add eax,2       ;eax ban mostmár 5 van
mov ebx,5       
add eax,ebx     ;EAX ban mostmár 10 van
sub eax,3       ;EAX most már 7
mov cl,2        ;ecx első bytejaba rakunk kettőt, ezzel fogunk osztani      
idiv cl         ;ez ax-et fogja osztani cl el. AL be kerül a HÁNYADOS.  AH -ba pedig a maradék
;a div és a mul ennél bonyolultabb de nem érdemes többet tudni roluk
;tehát most AL:3 AH:1         mivel 7/2 megvan 3x és marad 1
```

Ennél többet nem érdemes róluk tudni.

Egyetlen egy dolog jöhet jól, mégpedig hogy ugye nullával nem lehet osztani.Ha megpróbálod az konkrétan a proceszorban fog okozni expectiont és nem a softwaresen amit ki használ néhány hack
```S
idiv 0      ;error
```

# Jumpok



