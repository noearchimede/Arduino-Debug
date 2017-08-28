/**
\file

\brief Funzioni per ottenere il tipo di una variabile
\date 20 agosto 2017

\note Lo so, il file è in inglese, ma l'ho comunque scritto io!

Questa classe permette di ottenere informazioni sul tipo di una variabile.
Per usarla basta crearne un'istanza passando come argomento al constructor la
variabile su cui servono informazioni. Poi si possono ottenere alcune proprietà,
il nome del tipo e/o la dimensione in byte.
```cpp

int x; //una variabile qualsiasi

TypeTest testX (x); //l'istanza testX è impostata per dare informazioni su int

testX.isBool                 ==  false
testX.isInteger              ==  true
testX.isSigned               ==  true
testX.isCharacter            ==  false
testX.isFloat                ==  false
testX.isArithmetic           ==  true
testX.name                   ==  TypeTest::TypeName::i16; //lista nomi: vedi sotto
testX.size                   ==  2

```

I nomi dei tipi sono definiti nell'`enum` `TypeTest::TypeName`:
```cpp

enum TypeName {
b,                   //bool
u8, u16, u32, u64,   //unsigned integers
i8, i16, i32, i64,   //integers
c,                   //character
f, d,                //float
};

```

TypeTest funziona solo con i tipi aritmetici. Se la variabile passata al
constructor è di un altro tipo il compilatore restituisce l'errore
`no matching function for call to 'TypeTest::TypeTest(##)` dove ## è il
tipo della variabile.


Alla fine del file in un commento c'è un programma di test per provare il
funzionamento della classe con qualsiasi tipo.

*/

#ifndef Type_test_hpp
#define Type_test_hpp


#include "inttypes.h"

struct TypeTest {

    //contrustor: see below

    enum TypeName {
        b,                   //bool
        u8, u16, u32, u64,   //unsigned integers
        i8, i16, i32, i64,   //integers
        c,                   //character
        f, d                 //float
    };

    //public variables

    TypeName    name;
    uint8_t     size;
    bool isBool;
    bool isInteger;
    bool isSigned;
    bool isCharacter;
    bool isFloat;

private:

    void setInfo(bool b, bool i, bool s, bool c, bool f) {
        isBool = b;
        isInteger = i;
        isSigned = s;
        isCharacter = c;
        isFloat = f;
    }
public:

    //contructors
    //bool                                                     b i s c f
    TypeTest(bool)     : name(TypeName::b),   size(1) {setInfo(1,0,0,0,0);}

    //unsigned integers                                        b i s c f
    TypeTest(uint8_t)  : name(TypeName::u8),  size(1) {setInfo(0,1,0,0,0);}
    TypeTest(uint16_t) : name(TypeName::u16), size(2) {setInfo(0,1,0,0,0);}
    TypeTest(uint32_t) : name(TypeName::u32), size(4) {setInfo(0,1,0,0,0);}
    TypeTest(uint64_t) : name(TypeName::u64), size(8) {setInfo(0,1,0,0,0);}

    //signed integers                                          b i s c f
    TypeTest(int8_t)   : name(TypeName::i8),  size(1) {setInfo(0,1,1,0,0);}
    TypeTest(int16_t)  : name(TypeName::i16), size(2) {setInfo(0,1,1,0,0);}
    TypeTest(int32_t)  : name(TypeName::i32), size(4) {setInfo(0,1,1,0,0);}
    TypeTest(int64_t)  : name(TypeName::i64), size(8) {setInfo(0,1,1,0,0);}

    //character                                                b i s c f
    TypeTest(char)     : name(TypeName::c),   size(1) {setInfo(0,1,1,1,0);}

    //floating point                                                  b i s c f
    TypeTest(float x)  : name(TypeName::f),   size(4)        {setInfo(0,0,1,0,1);}
    TypeTest(double x) : name(TypeName::d),   size(sizeof(x)){setInfo(0,0,1,0,1);}


};



#endif








/*
\date 22/08/2017
\author Noè Archimede Pezzoli

This sketch allows you to quickly test the behavior of the TypeTest library, a
library intended to provide some information about the type of an unknown variable.
This library only works for pointers and arithmetic types, i.e the types on wich
you are allowed to perform arithmetic operations like * and -

Write `TEST(typename)` ['typename' is the name of a datatype as you would write
it to declare a variable: `int`, `float`, `bool`, ...] after the `Serial.begin()`
statement as many times as you want, one for each type you want to test.

- If one of the `typenames` you wrote is not arithmetic (or does not exist) the
program will not compile.
- If all the types are arithmetic, the program (uploaded to an Arduino
board connectet to the computer) will print a set of informations for each
TEST statement.

*/
/*

#include "Type_test.hpp"



//test: prints informations about the type passed as argument
//used in `void setup()``
#define TEST(X)                                                       \
{                                                                     \
X x;                                                                  \
TypeTest test(x);                                                     \
Serial.print("TYPE -\t"); Serial.println(#X);                         \
Serial.println();                                                     \
Serial.print("name:\t");  Serial.println(test.name);                  \
Serial.print("size:\t");  Serial.println(test.size);                  \
Serial.println();                                                     \
Serial.print("bool:\t");  Serial.println(test.isBool);                \
Serial.print("int:\t");  Serial.println(test.isInteger);              \
Serial.print("s_int:\t");  Serial.println(test.isSignedInteger);      \
Serial.print("char:\t");  Serial.println(test.isCharacter);           \
Serial.print("float:\t");  Serial.println(test.isFloat);              \
Serial.print("ptr:\t");  Serial.println(test.isArithmetic);           \
Serial.println();                                                     \
Serial.println();                                                     \
}


class MyClass {};


void setup() {

Serial.begin(115200);

TEST(int)
TEST(bool)
TEST(short)
TEST(float)
TEST(double)
TEST(intmax_t)
TEST(int*)//compiler error: TypeTest can only test arithmetic types

//TEST(MyClass) //compiler error: TypeTest can only test arithmetic types

}

void loop() {}
*/
