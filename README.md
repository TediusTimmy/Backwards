Backwards
=========

Backwards Scripting Language number 3

WAT? This is, basically, ESL2 in C++. There are some changes, though. I removed types for doing 3D physics problems and changed how functions work (for the better, I feel). And it's called Backwards because I wanted a scripting language for a Backrooms-themed game. Collections still aren't classes, but it shouldn't matter. Also note that the argument separator is ';', from DB14, for the same reasons: while all keywords are English, numbers can mix-and-match the ',' and '.' as the decimal separator. It's the only internationalization I provide. Also note, that, like DB14, Backwards now has a decimal-oriented numeric type. It won't be as fast, but it will be more user-friendly (I feel).

# The Language
This is the language as implemented. Some of the GoogleTests have good examples, others, not so much.

## Data Types:
* Float - this is a decimal floating-point number with nine digits of precision.
* String
* Array (ordinally indexed Dictionary?)
* Dictionary
* Function Pointer

## Operations
* \+  float addition; string catenation; for collections, the operation is performed over the contents of the collection
* \-  float subtraction; for collections, the operation is performed over the contents of the collection
* \-  float unary negation; for collections, the operation is performed over the contents of the collection
* \*  float multiplication; for collections, the operation is performed over the contents of the collection
* /   float division; for collections, the operation is performed over the contents of the collection
* ^   float exponentiation; this operator is right-associative
* !   logical not
* \>  greater than, only defined for strings and floats
* \>= greater than or equal to, only defined for strings and floats
* <   less than, only defined for strings and floats
* <=  less than or equal to, only defined for strings and floats
* =   equality, defined for all types
* <>  inequality, defined for all types
* ?:  ternary operator
* &   logical and, short-circuit
* |   logical or, short-circuit
* []  collection access
* .   syntactic sugar for collection access; x.y is equivalent to x["y"]; in addition . and , are interchangeable, so x.y is the same as x,y
* {}  collection creation: `{}` is an empty array; `{ x; y; z }` creates an array; `{ x : y ; z : w ; a : b }` creates a dictionary

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
`"function" [ <identifier> ] "(" [ <identifier> { ";" <identifier> } ] ")" "is" <statements> "end"`  
This is actually an expression, not a statement. It resolves to the function pointer of the defined function. As such, a function name is optional (but assists in debugging). Function declaration is static, all variables are captured by reference (no closures), and a function cannot access the variables of an enclosing function. All arguments to function calls are pass-by-value, semantically.  
New addition : the function name must now be unique with respect to: all functions that are being defined, global variables, and scoped variables. Functions can use their name to recursively call themselves.  
Previous way:  
`set fib to function (x) is if x > 1 then return fib(x - 1) * x else return 1 end end`  
New way:  
`set x to function fib (y) is if y > 1 then return fib(y - 1) * y else return 1 end end`

## Comments
`"(*" Comment "*)"`  
Old-style Pascal comments really round out the language as being valid even when newlines are replaced with spaces.

## Standard Library
* float Abs (float)  # absolute value
* float Acos (float)  # inverse cosine, result in degrees
* float Asin (float)  # inverse sine, result in degrees
* float Atan (float)  # inverse tangent, result in degrees
* float Atan2 (float; float)  # two-argument inverse tangent, result in degrees
* float Cbrt (float)  # cube root
* float Ceil (float)  # ceiling
* float ContainsKey (dictionary, value)  # determine if value is a key in dictionary (the language lacks a means to ask for forgiveness)
* float Cos (float)  # cosine, argument in degrees
* float Cosh (float)  # hyperbolic cosine
* string Date ()  # get the current date as mm/dd/yyyy (because I'm a Yank)
* string DebugPrint (string)  # log a debugging string, returns its argument
* float DegToRad (float)  # degrees to radians
* float EnterDebugger ()  # enters the integrated debugger (if present), returns zero
* string Error (string)  # log an error string, returns its argument
* value Eval (string)  # parse and evaluate the given string, return its evaluated value
* float Exp (float)  # raise Euler's constant to some power
* Fatal (string)  # log a fatal message, this function does not return, calling this function stops execution
* float Floor (float)  # floor
* float FromChar (string)  # return the ASCII code of the only character of the string (the string must have only one character)
* value GetIndex (array; float)  # retrieve index float from array
* array GetKeys (dictionary)  # return an array of keys into a dictionary
* value GetValue (dictionary; value)  # retrieve the value with key value from the dictionary, die if value is not present (no forgiveness)
* float Hypot (float; float)  # hypotenuse function : Sqrt(xx + yy) with (I hope) hardening for underflow
* string Info (string)  # log an informational string, returns its argument
* dictionary Insert (dictionary; value; value)  # insert value 2 into dictionary with value 1 as its key and return the modified dictionary (remember, this DOES NOT modify the passed-in dictionary)
* float IsArray (value)  # run-time type identification
* float IsDictionary (value)  # run-time type identification
* float IsFloat (value)  # run-time type identification
* float IsFunction (value)  # run-time type identification
* float IsInfinity (float)  # too big?
* float IsNaN (float)  # is this not a number?
* float IsString (value)  # run-time type identification
* float Length (string)  # length
* float Ln (float)  # logarithme naturel
* float Log (float; float)  # second arg is base (divisor)
* float Max (float; float)  # if either is NaN, returns NaN; returns the first argument if comparing positive and negative zero
* float Min (float; float)  # if either is NaN, returns NaN; returns the first argument if comparing positive and negative zero
* array NewArray ()  # returns an empty array
* array NewArrayDefault (float, value)  # returns an array of size float with all indices initialized to value
* dictionary NewDictionary ()  # returns an empty dictionary
* float PI ()  # delicious
* array PopBack (array)  # return a copy of the passed-in array with the last element removed
* array PopFront (array)  # return a copy of the passed-in array with the first element removed
* array PushBack (array; value)  # return a copy of the passed-in array with a size one greater and the last element the passed-in value
* array PushFront (array; value)  # return a copy of the passed-in array with a size one greater and the first element the passed-in value
* float RadToDeg (float)  # radians to degrees
* dictionary RemoveKey (dictionary; value)  # remove the key value or die
* float Round (float)  # ties to even
* array SetIndex (array; float; value)  # return a copy of array where index float is now value
* float Sin (float)  # sine, argument in degrees
* float Sinh (float)  # hyperbolic sine
* float Size (array)  # size of an array
* float Size (dictionary)  # number of key,value pairs
* float Sqr (float)  # square
* float Sqrt (float)  # square root
* float SubString (string; float; float)  # from character float 1 to character float 2 (java style)
* float Tan (float)  # tangent, argument in degrees
* float Tanh (float)  # hyperbolic tangent
* string Time ()  # get the current time as hh:mm:ss (24 hour)
* string ToCharacter (float)  # return a one character string of the given ASCII code (or die if it isn't ASCII)
* string ToString (float)  # return a string representation of a float: scientific notation, 9 significant figures
* float ValueOf (string)  # parse the string into a float value
* string Warn (string)  # log a warning string, returns its argument


Backway
=======

Backway is a state engine, analogous to the Ants state engine.

As such, conceptually it is a stack of queues. However, the operations that you get allow you to treat it in any way you want, mostly. A queue is just a list. The state machine operates on lists of things to do. Now, if you are baking cookies, you have a list of things to do. If a monster interrupts you baking cookies, then you need to "kill the monster" before you can go back to baking cookies. This is stack behavior: something comes up that takes precedence over the list of things that you were doing earlier. These are the two tools for making sense of how to behave: doing lists of things, and pushing a new list of things to do in the event of a special case.

States need to define the function Update. Update takes one argument, and this argument rolls along. For the first call ever, this argument is an empty dictionary. For subsequent calls, this argument is whatever Update returned last. If a state changes the stack of queues such that the front state of the top queue has changed, then the update function of the front of the top will continue to be called until the state engine stops changing.

How is this supposed to work? In the Update function, the agent looks around the world, considers what it wants to do, considers how its last attempt at doing something turned out, and then makes a new attempt to change the world. It then returns from Update, because Update is not a co-routine, and it needs to do all of that Update stuff every time.

## Standard Library
* float CreateState(string; string) # Create a new state with first argument name and second argument functions, one of which must be Update
* float Enqueue (string) # Add named state to the back of the current queue
* float Enqueue (array of string)
* float Finally (string) # Add named state to its own queue at the bottom of the stack
* float Finally (array of string)
* float Follow (string) # Add named state after the current state in the current queue
* float Follow (array of string)
* value GetInput () # Get the magical "input" thing. Empty Dictionary if there is no input
* string GetName () # Get the name of the current state
* float Inject (string) # Add names state to its own queue in a new queue as the second on the stack
* float Inject (array of string)
* float Leave () # Exit the current state, and continue to the next state in the current queue
* float Precede (string) # Add named state to the current queue in front of the current state
* float Precede (array of string)
* float Push (string) # Add named state to its own queue on the new top of the stack
* float Push (array of string)
* float Rand () # Get a random number
* float Return () # Exit the current queue, and continue to the next queue in the stack
* float Skip (string) # Leave states until at the named state in the current queue
* float Unwind (string) # Return from queues until at the named state in a following queue


SlowFloat
=========

I don't know about you all, but when I was a child, I don't remember my teachers noting to me that .1 * 10 wouldn't be 1.0 on a calculator. That didn't happen until I started programming computers. It didn't happen when I was programming calculators. The reason for that is that calculators handle numbers in a manner differently from computers: it is slower, but less mysterious to the human user. In order to be more human-friendly, I have replaced the base number type with a decimal-centric type.
