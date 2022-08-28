Backwards
=========

Backwards Scripting Language number 3

WAT? This is, basically, ESL2 in C++. There are some changes, though. I removed types for doing 3D physics problems and changed how functions work (for the better, I feel). And it's called Backwards because I wanted a scripting language for a Backrooms-themed game. Collections still aren't classes, but it shouldn't matter.

# The Language
This is the language as implemented. Some of the GoogleTests have good examples, others, not so much.

## Data Types:
* Double
* String
* Array (ordinally indexed Dictionary?)
* Dictionary
* Function Pointer

## Operations
* \+  double addition; string catenation; for collections, the operation is performed over the contents of the collection
* \-  double subtraction; for collections, the operation is performed over the contents of the collection
* \-  double unary negation; for collections, the operation is performed over the contents of the collection
* \*  double multiplication; for collections, the operation is performed over the contents of the collection
* /   double division; for collections, the operation is performed over the contents of the collection
* ^   double exponentiation; this operator is right-associative
* !   logical not
* \>  greater than, only defined for strings and doubles
* \>= greater than or equal to, only defined for strings and doubles
* <   less than, only defined for strings and doubles
* <=  less than or equal to, only defined for strings and doubles
* =   equality, defined for all types
* <>  inequality, defined for all types
* ?:  ternary operator
* &   logical and, short-circuit
* |   logical or, short-circuit
* []  collection access
* .   syntactic sugar for collection access; x.y is equivalent to x["y"]
* {}  collection creation: `{}` is an empty array; `{ x, y, z }` creates an array; `{ x : y , z : w , a : b }` creates a dictionary

## Operator Precedence
* ()  -- function call
* {}
* . []
* ^
* ! -  -- unary negation
* \* /
* \+ \-
* = <> > >= < <=
* | &
* ?:

## Statements
I'm going to mostly use BNF. Hopefully, I'm not doing anything fishy. Note that [] is zero-or-one and {} is zero-to-many.
### Expression
`"call" <expression>`  
To simplify the language, we have specific keywords to find the beginning of a statement. The "first set" of a statement is intentionally small, so that parsing and error recovery is easier. It makes the language a little verbose, though. Sometimes you want to just call a function (for instance, to output a message), and this is how. It will also allow you to add three to the function's result.
### Assignment
`"set" <identifier> { "[" <expression> "]" } "to" <expression>`  
`"set" <identifier> { "." <identifier> } "to" <expression>`  
Assigning to an undefined variable creates it. The parser will catch if you try to create a variable and access it like an array/dictionary in the same statement. However, the parser probably won't catch `set x to x` until runtime.  
### If
`"if" <expression> "then" <statements> { "elseif" <statements> } [ "else" <statements> ] "end"`  
### While
`"while" <expression> [ "call" <identifier> ] "do" <statements> "end"`  
The "call" portion gives the loop a name. See break and continue.
### For
`"for" <identifier> "from" <expression> ( "to" | "downto" ) <expression> [ "step" <expression> ] [ "call" <identifier> ] "do" <statements> "end"`  
`"for" <identifier> "in" <expression> [ "call" <identifier> ] "do" <statements> "end"`  
The second form iterates over an array or dictionary. When a dictionary is iterated over, the loop control variable is successively set to a two element array of { key, value }. If the variable does not exist, it will be created, and it will remain alive after the loop.
### Return
`"return" <expression>`
### Select
`"select" <expression> "from"`  
`    [ "also" ] "case" [ ( "above" | "below" ) ] <expression> "is"`  
`    [ "also" ] "case" "from" <expression> "to" <expression> "is"`  
`    [ "also" ] "case" "else" "is"`  
`"end"`  
Select has a lot of forms and does a lot of stuff. Cases are breaking, and "also" is used to have them fall-through. Case else must be the last case.
#### Break
`"break" [ <identifier> ]`  
Breaks out of the current while or for loop, or the named while or for loop. This isn't completely a statement, in that it requires a loop to be in context.
#### Continue
`"continue" [ <identifier> ]`  
#### Function Definition
`"function" [ <identifier> ] "(" [ <identifier> { "," <identifier> } ] ")" "is" <statements> "end"`  
This is actually an expression, not a statement. It resolves to the function pointer of the defined function. As such, a function name is optional (but assists in debugging). Function declaration is static, all variables are captured by reference (no closures), and a function cannot access the variables of an enclosing function. All arguments to function calls are pass-by-value, semantically.

## Standard Library
* double Abs (double)  # absolute value
* double Acos (double)  # inverse cosine, result in degrees
* double Asin (double)  # inverse sine, result in degrees
* double Atan (double)  # inverse tangent, result in degrees
* double Atan2 (double, double)  # two-argument inverse tangent, result in degrees
* double Cbrt (double)  # cube root
* double Ceil (double)  # ceiling
* double ContainsKey (dictionary, value)  # determine if value is a key in dictionary (the language lacks a means to ask for forgiveness)
* double Cos (double)  # cosine, argument in degrees
* double Cosh (double)  # hyperbolic cosine
* string Date ()  # get the current date as mm/dd/yyyy (because I'm a Yank)
* string DebugPrint (string)  # log a debugging string, returns its argument
* double DegToRad (double)  # degrees to radians
* double EnterDebugger ()  # enters the integrated debugger (if present), returns zero
* string Error (string)  # log an error string, returns its argument
* value Eval (string)  # parse and evaluate the given string, return its evaluated value
* double Exp (double)  # raise Euler's constant to some power
* Fatal (string)  # log a fatal message, this function does not return, calling this function stops execution
* double Floor (double)  # floor
* double FromChar (string)  # return the ASCII code of the only character of the string (the string must have only one character)
* value GetIndex (array, double)  # retrieve index double from array
* array GetKeys (dictionary)  # return an array of keys into a dictionary
* value GetValue (dictionary, value)  # retrieve the value with key value from the dictionary, die if value is not present (no forgiveness)
* double Hypot (double, double)  # hypotenuse function : Sqrt(xx + yy) with (I hope) hardening for underflow
* string Info (string)  # log an informational string, returns its argument
* dictionary Insert (dictionary, value, value)  # insert value 2 into dictionary with value 1 as its key and return the modified dictionary (remember, this DOES NOT modify the passed-in dictionary)
* double IsArray (value)  # run-time type identification
* double IsDictionary (value)  # run-time type identification
* double IsDouble (value)  # run-time type identification
* double IsFunction (value)  # run-time type identification
* double IsInfinity (double)  # too big?
* double IsNaN (double)  # is this not a number?
* double IsString (value)  # run-time type identification
* double Length (string)  # length
* double Ln (double)  # logarithme naturel
* double Log (double, double)  # second arg is base (divisor)
* double Max (double, double)  # if either is NaN, returns NaN; returns the first argument if comparing positive and negative zero
* double Min (double, double)  # if either is NaN, returns NaN; returns the first argument if comparing positive and negative zero
* array NewArray ()  # returns an empty array
* array NewArrayDefault (double, value)  # returns an array of size double with all indices initialized to value
* dictionary NewDictionary ()  # returns an empty dictionary
* double PI ()  # delicious
* array PopBack (array)  # return a copy of the passed-in array with the last element removed
* array PopFront (array)  # return a copy of the passed-in array with the first element removed
* array PushBack (array, value)  # return a copy of the passed-in array with a size one greater and the last element the passed-in value
* array PushFront (array, value)  # return a copy of the passed-in array with a size one greater and the first element the passed-in value
* double RadToDeg (double)  # radians to degrees
* dictionary RemoveKey (dictionary, value)  # remove the key value or die
* double Round (double)  # ties to even
* array SetIndex (array, double, value)  # return a copy of array where index double is now value
* double Sin (double)  # sine, argument in degrees
* double Sinh (double)  # hyperbolic sine
* double Size (array)  # size of an array
* double Size (dictionary)  # number of key,value pairs
* double Sqr (double)  # square
* double Sqrt (double)  # square root
* double SubString (string, double, double)  # from character double 1 to character double 2 (java style)
* double Tan (double)  # tangent, argument in degrees
* double Tanh (double)  # hyperbolic tangent
* string Time ()  # get the current time as hh:mm:ss (24 hour)
* string ToCharacter (double)  # return a one character string of the given ASCII code (or die if it isn't ASCII)
* string ToString (double)  # return a string representation of a double: scientific notation, 16 significant figures
* double ValueOf (string)  # parse the string into a double value
* string Warn (string)  # log a warning string, returns its argument
