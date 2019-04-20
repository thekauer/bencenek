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

# CMP, JMP, Labelek

## Uncoditional Jump

csak úgy ugrok egy helyre a memóriában.

```S
Func:               ;int Func() {return 1;}
push ebp
mov ebp,esb
mov eax,1
mov esp,ebp
pop ebp
ret


...

JMP Func        ;ugrás a funkciómra
```

Na ebben sok ujdonság volt de ami inne fontos egyrész a labelek.
Akármennyi lehetó,bárhol csak különbözzön a neve, a loopoknál és funkcióknál használatos.
Arra használjuk hogy oda tudjunk ugrani a kód bizonyos részeire

```S
;cim          utasitas
              L1:
FF000001      mov eax,1
              L2:
FF000002      mov ebx,3
              L3:
FF000003      add eax,ebx
```

Ami igazából érdekes az az hogy ez hogy működik amikor átirjuk az egészet byte okra.
A labeleknek nincs cime valójában az utánnuk következő utasitás cimét jelentik tehát:
L1 = FF000001
L2 = FF000002
L3 = FF000003

A labelek azért vannak hogy megspóroljanak nekünk pár számitást.

A legfontosabb közülök hogy a jmp utasitás **relativ** cimre ugrik azaz

Ha az 1000. cimen lévű jump utasitás a 10000. cimre ugrik

**nem** 
E9 10000 
a helyes kód 
hanem 
E9 9000       //avagy 10000-1000 ahol ugye vagyok


## Conditional Jump

A **CMP** utasitás gyakorlatilag kivonja az első registerből a másidok registert/számot és **NEM** tárolja az eredményt azaz a registerek értékei **nem** változnak 

```S
mov eax,3
mov ebx,2
cmp eax,ebx
; EAX:3   EBX:2
```
viszont beálitja a kivonásnak megfelelő flageket, és ezzekhez a flagekhez tartozó ugrásokkal tudunk gyakorlatilag **if** statementeket csinálni


```S
mov eax,10
mov ebx,10
cmp eax,ebx
je  EGYENLŐEK     ;akkor ugrik ha egyenlő volt a két szám
```
a következő conditional jump instrukciók vannak

JE/JZ       EGYENLŐEK         ==          JUMP EQUAL/ZERO
JNZ/JNE     NEM EGYENLŐEK     !=          JUMP NOT ZERO/NET EQUAL
JG/JNLE     NAGYOBB           >           JUMP GREATER/NOT LESS EQUAL
JGE/JNL     NAGYOBB EGYENLŐ   >=          JUMP GREATER-EQUAL/NOT LESS
JL/JNGE     KISSEBB           <           JUMP LESS/NOT GREATER EQUAL
JLE/JNG     KISSEBB EGYENLŐ   <=          JUMP LESS-EQUAL/NOT GREATER

# PUSH,POP ,PUSHA,POPA , a STACK,ESP
(Plusz cpp implementáció hátha igy könnyebb megérteni)
A stackről gyorsan.
Előre meglévő méretű,gyors.
Gyakorlatilag megfeleltethető egy arraynak cpp ban.
```cpp
int stack[255];
```
Három fontos müveletet lehet vele csinálni: **PUSH,POP,PEEK**
(de a peekel nem foglalkozunk most)
A stackhez tartozik egy stack pointer ami mindig a tetejére mutat.
```cpp
int* ESP = stack;
```

### PUSH
A push felrak a stack TETEJÉRE egy értéket
```cpp
void push(int val) {
  *ESP=val;     //a stack tetejére rakom az értéket - a stack tetejére mutat a ESP register
  ESP++;        // a PUSH után egyel arráb kerül a stack teteje hiszen eggyel magasabb lett
  
  //ha zavaros lenne az ESP ugye INT * tipusu az int 4 byte-os ezért amikor azt irom hogy ESP++ a '++' igazából nem eggyet hanem NÉGYET ad az ESP értékéhez
 }
 ```
 Itt már látszik hogy mitől gyors a stack.
 - Nem kell keresnem a memóriában valami távoli cimet rögtön rámutat a tetejére az ESP
 - Muitán felrakok egy elemet nincs realloc mivel ez igazából csak egy nagy array egyedül az ESPhez kell hozzáadjak
 
 Assemblyben:
 ```S
 ;EIP:0
 Push 3       ;felrakok 3mat a stackre ,    Az eip most 0 volt de felkerül egy szám ami 4 byte-os szoval az EIP:4
 mov eax,1
 Push eax     ;felrakom eax tartalmát - eggyet a stackre ,ez megint csak 4 byte az EIP:8
 ;most a stackem igy nézz ki:
 ;1
 ;3
```


### POP

Leveszem a stack lefelső elemét, csak registerbe!
```cpp
int pop() {
  int ret = *ESP;
  ESP--;
  return ret;
}
```
Itt újból látszik hogy mitől gyors a stack.
Valójában nem törlöm a stackről az elemet amit leveszek. Csak úgy teszek mintha nem lenne ott azzal hogy csökkentem az ESP-t.
Ezáltal a legközelebbi push úgy is át fogja irni az értéket.


Assemblyben:
```S
push 1
push 2
push 3
pop eax       ;EAX: 3
pop ebx       ;EBX  2
pop ecx       ;ECX  1
```




# Funkciók , EBP, CALL ,RET Calling Convetionök, Változók
általában a funkciók mindig a function prologue kezdődnek és epilogue végződnek.
Prologue:
```S
push ebp
mov ebp,esp
sub esp, N      ;N az az argumentumok méretének összege
```

Epilogue:
```S
mov esp,ebp
pop ebp
ret
```

de ez miért fontos

Ehez meg kell érteni a funkciók müködését.
Vegyünk például egy c nyelven irt funkciót.

A c nyelven irt funkciók alapvetően a cdecl calling convention használják.
Ami azt jelenti hogy az argumentumokat jobbról balra rakjuk fel a stackre és EAXba kerül a return érték.

```c
int add(int a,int b) {
  return a+b;
}


int main() {
add(2,3);

}
```


```S
Add:
push ebp
mov ebp,esp
sub esp,8       ;2 db int ami összesen 8 byte

mov eax,[esp] ; eax-ba rakom a-t
add eax,[esp+4] ; eax-hoz(a-hez) adom b-t

mov esp,ebp
pop ebp
ret

...

;main
push 3      ;argumentumok jobbról-ballra
push 2
call add
```


hogy megértsük mi is történt végig nézzük hogy hogy változik a stack

mainel kezüdik a program itt a stack üres

stack:

ESP:    00000000
EBP:    00000000      (main)



```S
push 3
push 2
```
stack:

00000004      2
00000000      3

ESP:    000000004         //mindig a stack tetejére mutat
EBP:    000000000         //az adott funkció cimére mutat a main a 0. cimen van szoval ez is nulla

```S
call add ;ez simplán csak oda ugrik az add funkcióra ,lényegében jmp is lehetne
```

stack:

00000004      2
00000000      3

ESP:    000000004
EBP:    000000000     (main)

```S
;átugrottunk a funkciomra és ő az első utasitas
push ebp    ;felrakom a stackre a jelenlegi funkció (main) kezdő cimét
```

stack:

0000008       00000000    (main)
0000004       3
0000000       2

ESP:    00000008
EBP:    00000000      (main)



Az ebp mindig az aktuáális funkcióra mutat
```S
mov ebp,esp     ;beleteszem az stack pointert a base pointerbe,       a stack pointer most a jelenlegi funkcióra mutat(add) ami a DEADBEEF helyen van a memóriában mivel elmentettem már a HIVÓ (main) funkciót a stackre ,nyugodtan átirhatom a jelenlegi funkciót ESP az addra
```


stack:

0000008       00000000      (main)
0000004       3
0000000       2


ESP:    00000008
EBP:    DEADBEEF      (add)




```S
sub esp,8       ;kivonom az argumentumok méretét a stackpointerből
```


stack:

0000008       00000000      (main)
0000004       3
0000000       2


ESP:    00000000
EBP:    DEADBEEF      (add)


```S
mov eax,[esp]         ;tehát most látszik hogy 2 raktam eaxba,ami az első argumentum 
```

stack:

0000008       00000000      (main)
0000004       3
0000000       2


ESP:    00000000
EBP:    DEADBEEF      (add)


```S
add eax,[esp+4]     ; ez pedig 3 azaz a második argumentum
```

stack:

0000008       00000000      (main)
0000004       3
0000000       2


ESP:    00000000
EBP:    DEADBEEF      (add)


```S
mov esp,ebp
```

stack:

0000008       00000000      (main)
0000004       3
0000000       2


ESP:    DEADBEEF      (add)
EBP:    DEADBEEF      (add)


```S
pop ebp       ;leveszem a stack tetejéről a 00000000 (maint) és belerakom EBP-be
```

stack:

0000008       00000000      (main)
0000004       3
0000000       2


ESP:    DEADBEEF
EBP:    00000000      (main)

 

```S
ret       ;vissza ugrok EBPre azaz a mainbe
```



Igen sikerült faszán belekavarodnom. hülyeség volt a mainnek 00000000 lennie , azt hittem igy egyszerűbb lesz a számolás de remélem  a lényeget azért érted.


Egyébként annyi hogy a legeslegelején esp nem 00000000-ra mutat hanem már van sok minden a stacken.















