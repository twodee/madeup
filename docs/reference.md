#Language Reference
Every fun thing that has ever happened started with some rules (gravitational law, oldest player rolls first, etc.), and Madeup is no exception. Let's describe the grammar of the Madeup language.

##Comments
Code is mixture of executable instructions and notes for the humans working with the code. The notes are called comments. Single-line comments start with `--` and appear on their own lines or at the end of lines containing instructions.

{:.mup}
~~~
-- This program generates a rocket ship.
-- It will fly.
-- Someday.

print "<=>" -- See it fly!
~~~

Use `---` to get a multiline comment, which spans until the next occurrence of `---`. You might use it to write paragraphs of documentation or to prevent chunks of code from being run as you experiment:

{:.mup}
~~~
--- Let's disable this code for the time beiing.
print pi
nsides = 100
print where
---

print 10
~~~

##Expressions
Madeup programs are built out of expressions, utterances that yield some value. Every construct of Madeup is an expression, but not all provide meaningful values. Like `yaw`. The `yaw` command yields a special value named `nothing`, which is good for nothing.

In this section, we focus on expressions that do yield meaningful values. Such expressions can be assembled together to produce more interesting values yet. When multiple expressions appear on a line, we must decide which one gets evaluated first. In 5 + 3 * 8, the convention of mathematical precedence tells us to multiply first. This yields the intermediate expression `5 + 24`. After evaluating the remaining operation, we get the expression's final value of `29`.

The expressions that Madeup supports are listed below from highest precedence to lowest.

###Literals
The simplest expressions in the Madeup language describe raw integers, real numbers, logical notions called booleans, strings of text, or arrays of values. Here are few literal expressions of various types:

{:.mup}
~~~
6.7 -- real
2 -- integer
"i: " -- string
true -- boolean
false -- boolean
nothing -- the not-a-thing
~~~

####Arrays
Values can be grouped together into collections called *arrays*. The simplest way to make an array is as an enumerated list:

{:.mup}
~~~
{1, "flat", true} -- an array of three elements
~~~

An array's elements do not have to be the same type.

If you need a large array or an array of arrays, use dimensional specifiers instead:

{:.mup}
~~~
hundred = 100 of false -- a 100-element array of falses
grid = 5 by 4 of 0 -- a 5x4 array of zeroes
tictactoe = 3 by 3 -- a 3x3 array of nothing
~~~

###Association (`(expr)`)
Expressions can be surrounded by parentheses to help control the order of evaluation or to emphasize grouping:

{:.mup}
~~~
print (5 + 3) * 8
~~~

Note that unlike many programming languages, Made does not require parentheses around function parameter lists:

{:.mup}
~~~
print "triangle"
print abs -5
a = cos pi
~~~

Given Madeup's spare use of punctuation, parentheses are often needed to enforce unary negation of variables, as in:

{:.mup}
~~~
x = cos (-theta)
~~~

The alternative

{:.mup}
~~~
x = cos -theta
~~~

is interpreted as subtraction: the value of `theta` is being taken away from the value of `cos`.

###Absolute Value (`|expr|`)
Find the absolution value of an expression by surrounding it with pipe (`|`) characters:

{:.mup}
~~~
x = -3.14
print |x + 1| -- prints 2.14
~~~

Note that nesting absolute value expressions is illegal:

{:.mup}
~~~
x = -3.14
print |x + |-1|| -- illegal
~~~

###Indexing (`[i]`)
Index into an array or string with `[]`, the subscript operator. To access a single element, the index expression `i` must yield an integer.

{:.mup}
~~~
counts = 10 of 0
counts[0] = 17
counts[9] = 32
counts[10] = 5 -- illegal, out-of-bounds index

name = "bingo"
for i to size name
  print name[i]
end
~~~

One can also access a subrange:

{:.mup}
~~~
alpha = "abcdefghijklmnopqrstuvxyz"
print alpha[0..3] -- prints "abcd"

vec3 = {10, 20, 30}
vec2 = vec3[0..1] -- vec2 is {10, 20}
~~~

Indices can be negative, in which case they count backward from the end of the array or string:

{:.mup}
~~~
alpha = "abcdefghijklmnopqrstuvxyz"
print alpha[-1] -- prints "z"
print alpha[-3..-1] -- prints "xyz"
~~~

###Exponential (`^`)
Raise a number to a power with the `^` operator. This operator is left-associative, meaning that in an expression containing multiple `^` operators at the same level of precedence, the leftmost operation will be evaluated first.

The base and exponent are expected to be numbers. If the exponent is less than 1 and the base is negative, by the rules of mathematics, you will get a complex number. By the rules of Madeup, you will get an undefined value.

{:.mup}
~~~
print 5 ^ 3 -- prints 125
print 49 ^ 0.5 -- prints 7
print 2 ^ 3 ^ 2 -- same as (2 ^ 3) ^ 2 = 8 ^ 2 = 64
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print {1, 2, 3} ^ 2 -- prints {1, 4, 9}
print {10, 10, 10} ^ {1, 2, 3} -- prints {10, 100, 1000}
~~~

###Multiplicative (`*`, `/`, `//`, `%`)
Madeup provides the conventional multiplication and division operators, along with a real division operator and a remainder or modulus operator. The `/` operator yields an integer if its two operands are integers, and a real if at least one of the operands is a real. To ensure a real number result, even if the operands are integers, use `//`. The remainder operation has the form `a % b`, where `a` and `b` are integers. This operation yields the integer remainder of `a / b`.

If both operands are integers, the result is an integer. Otherwise, the result is a real.

{:.mup}
~~~
print 100.7 * 2 -- 201.4
print 7 / 3 -- 2
print 7 / 3.0 -- 2.3333...
print 7 // 3 -- 2.3333...
print 7 % 3 -- 1
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print {1, 2, 3} * 2 -- prints {2, 4, 6}
print {10, 10, 10} / {1, 2, 3} -- prints {10, 5, 3}
~~~

###Additive (`+`, `-`)
Find the sum or difference of two numbers with `+` or `-`. If both operands are integers, the result is an integer. Otherwise, the result is a real.

{:.mup}
~~~
print 100 + -100 -- 0
print 0.7 + 2 -- 2.7
x = 0.3
print 6 - x -- 5.7
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print {1, 2, 3} + 2 -- prints {3, 4, 5}
print {10, 10, 10} + {1, 2, 3} -- prints {11, 12, 13}
~~~

###Relational (`<`, `<=`, `>`, `>=`)
Query the relationship between two numbers with these operators, which yield a boolean value indicating whether or not the relation holds.

{:.mup}
~~~
print 5 < 3 -- false
print 6 <= 6 -- true
print 7 >= 3 -- true
print 2 > -2 -- true
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print {1, 2, 3} >= 2 -- prints {false, true, true}
print {10, 10, 10} < {5, 10, 15} -- prints {false, false, true}
~~~

###Equality (`==`, `!=`)
Query whether two values are the same with the `==` operator. Query whether two values are different with the `!=` operator. These operators can be applied to numbers, booleans, and strings.

{:.mup}
~~~
print 5 == 0 -- false
print true == not false -- true
print 3 / 2 == 3 // 2 -- false
print 5.7 != 5.69 -- true
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print {1, 1} == {1, 1} -- prints true
print {1, 1} == 1 -- prints false
~~~

###Assignment (`=`)
Bind a value to a name using the `=` operator. This operator is right-associative, meaning that in an expression with multiple `=` operations, the rightmost `=` will be evaluated first.

{:.mup}
~~~
a = b = 3
print a -- 3
print b -- 3
sum = a + b -- sum holds 6
diff = a - b -- diff holds 0
~~~

###Logical Negation (`not`)
Flip a boolean expression to its opposite with the `not` operator:

{:.mup}
~~~
print not true -- prints false
x = 15
if not x < 15
  print "yes!"
end -- prints yes!
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print not {true, false} -- prints {false, true}
~~~

###Conjunction (`and`)
Query whether two booleans are both true with the `and` operator.

{:.mup}
~~~
if x > 0 and y > 0
  print "in quadrant 1"
end
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print {true, false} and true -- prints {true, false}
print {true, true} and {false, true} -- prints {false, true}
~~~

###Disjunction (`or`)
Query whether at least one of two booleans is true with the `or` operator.

{:.mup}
~~~
if i < 0 or i >= length values
  print "bad index"
end
~~~

This operator may also be applied to arrays:

{:.mup}
~~~
print {true, false} or true -- prints {true, true}
print {true, true} or {false, true} -- prints {true, true}
~~~

##Builtin Variables
Several variables are automatically defined by Madeup. Some are frequently-used constants while others influence the behavior of the various solidifiers.

###`.radius` or `.outerRadius` (default: 1)
The value of `.outerRadius` is examined at each `move` and `moveto` command. Solidifiers `dowel`, `tube`, `dots`, and `boxes` will generate geometry around this vertex sized according to this value.

Internally, this variable is identified as `.outerRadius` to distinguish it from `.innerRadius`, which is referenced only by the `tube` solidifier. When you are not dealing with `tube`, you may find it more convenient to use the synonym `.radius.`

###`.innerRadius` (default: 0.5)
The value of `.innerRadius` is examined at each `move` and `moveto` command. The `tube` solidifier will generate inner geometry around this vertex sized according to its value.

###`.rgb` (default: {1, 0, 0})
The value of `.rgb` is examined at each `move` and `moveto` command to determine the vertex's RGB color. This variable is expected to hold a three-element array.

###`nsides` (default: 4)
The value of `nsides` controls the smoothness of the generated geometry. The cross section of `dowel`s and `tube`s will have a number of sides determined by `nsides`. Solidifier `dots` will create spheres with a number of wedges determined by `nsides`. Solidifier `revolve` will spin around its axis, placing vertices at intermediate stops during the revolution. The number of stops is determined by `nsides`.

###`pi` (default: 3.14159...)
The mathematical constant. I suggest you don't reassign it, though nothing in Madeup will stop you from doing so.

###`e` (default: 2.71828...)
The mathematical constant. I suggest you don't reassign it, though nothing in Madeup will stop you from doing so.

##User Variables
You may also create your own variables using the `=` operator:

{:.mup}
~~~
a = 1
ndogs = random 0, 10
tau = 2 * pi
~~~

Variables defined before a function definition are visible within the function.

{:.mup}
~~~
radius = 10

to polarizeX x = radius * cos x
to polarizeY y = radius * sin y
~~~

If the function reassigns the variable, the value also changes in the outer scope:

{:.mup}
~~~
n = 3

to increment
  n = n + 1
end

increment
increment
print n -- prints 5
~~~

##Loops
Madeup supports several kinds of loops to help you repeat processes. 

###`repeat`
Running a sequence multiple times is very simply accomplished using the `repeat` loop:

{:.mup}
~~~
repeat 10
  print ":)"
end
~~~

The count may be any integer expression:

{:.mup}
~~~
sum = 0
n = random 0, 100
repeat n
  sum = sum + n
end
~~~

###repeatwich
Sometimes half of the repeated sequence needs to run one more time than the other half. We refer to this sometimes as a "loop and a half." These situations arise frequently enough in 3D construction and path walking that Madeup provides support for such loops. The "and a half" portion sandwiches or surrounds the internal portion. Therefore, we call this a `repeatwich` loop.

{:.mup}
~~~
-- prints xoxox
repeatwich 3
  print "x"
around
  print "o"
end
~~~

Sometimes we need to know which iteration of a loop we are on. The `repeat` loop doesn't provide this, though we could pretty easily create and increment a variable ourselves. However, the various `for` loops will do this automatically.

###for-to
Like `repeat`, the `for-to` is a count-controlled loop. However, it automatically tracks what iteration of the loop we are on in a variable of our choice. The following loop stores the iteration index in a variable named `i`:

{:.mup}
~~~
-- prints 0, 1, 2
for i to 3
  print i
end
~~~

The upper-bound is exclusive and must be an integer. This loop starts at 0.

###for-through
The `for-through` loop is almost identical to `for-to`, except the upper-bound is inclusive:

{:.mup}
~~~
-- prints 0, 1, 2, 3
for i through 3
  print i
end
~~~

###for-in
The `for-in` loop is like `for-through`, except we can also specify the lower-bound.

{:.mup}
~~~
-- prints 100, 101, 102, 103
for i in 100..103
  print i
end
~~~

###while
The `while` loop is most general. It repeats as long as its condition is true.

{:.mup}
~~~
-- Stop once we go past 100.
x = 0
while x < 100
  x = x + random 0, 10
  moveto x, 0, 0
end
~~~

##Conditionals
To alternate between two chunks of code, use a conditional statement. The condition must be a boolean expression. Two forms of conditional statements exist.

###if-else-end
For alternating between possibly multiline sequences, use the block-oriented `if-else-end` statement:

{:.mup}
~~~
-- Zigzag.
for i in 10
  if i % 2 == 0
    moveto 1, 0, 0
  else
    moveto -1, 0, 0
  end
end
~~~

###if-then-else
For alternating between values, use the single-line ternary `if-then-else`:

{:.mup}
~~~
to abs x = if x > 0 then x else -x
~~~

##Builtin Functions
Madeup provides a modest library of builtin functions to aid in debugging, computation, or manipulating paths. Each function along with its expected parameters is described below.

###IO Functions

####`print message`
Print a given message. Message may be a string, number, boolean, or array.

{:.mup}
~~~
print 9 * 7 -- prints "63"
~~~

####`debug message`
Print a given message, preceded by the expression that computes it. Message may be a string, number, boolean, or array.

{:.mup}
~~~
debug 9 * 7 -- prints "9 * 7: 63"
~~~

###Movement Functions

####`where`
Get the current location as a three-element array.

{:.mup}
~~~
moveto 0, 0, 0
move 10
print where -- prints {0, 10, 0}
~~~

####`moveto x, y, z`
Move directly to location (`x`, `y`, `z`). The location is transformed by any transform modifiers (such as `rotate`, `scale`, `translate`) that have been previously applied.

####`move length`
Move `length` units along the current heading.

####`yaw degrees`
Alter the current heading by turning right or left the given number of degrees. The direction of the turn depends on the sign of `degrees` and your point of view. If you are looking down on the cursor (and it is pointing up at you) and if degrees is positive, a right turn is made.

####`pitch degrees`
Alter the current heading by flipping forward or backward the given number of degrees. The direction of the turn depends on the sign of `degrees` and your point of view. If you are looking at the back of the cursor and if degrees is positive, the cursor's nose is turned down.

####`roll degrees`
Alter the current heading by rolling right or left the given number of degrees. The direction of the turn depends on the sign of `degrees` and your point of view. If you are looking at the back of the cursor and if degrees is positive, the cursor's left side comes up and its right side comes down.

###Geometry Functions

####`translate x, y, z`
Modify any future movement or generated geometry by shifting it by the given offsets along the x-, y-, and z-axes. This transformation will be applied after any previous modifier. See `identity` to clear the transform modifier.

####`rotate x, y, z, degrees`
Modify any future movement or generated geometry by rotating along the given axis by the given degrees. This transformation will be applied after any previous modifier. See `identity` to clear the transform modifier.

####`scale x, y, z`
Modify any future movement or generated geometry by scaling by the given factors along the x-, y-, and z-axes. This transformation will be applied after any previous modifier. See `identity` to clear the transform modifier.

####`identity`
Clear any transformation modifiers.

####`reframe`
Interpret the current right axis as the x-axis, the current up axis as the y-axis, and the current forward-axis as the z-axis. TODO

####`push`
Save the current transformation modifier and cursor location and heading so that it may be restored later.

####`pop`
Restore the most recently saved transformation modifier and cursor location and heading.

####`reverse`
Reverse the list of vertices accumulated in the current path.

####`center`
Shift the list of vertices accumulated in the current path so they are centered on the origin.

####`coalesce threshold`
Remove any duplicate vertices in the current path. A duplicate vertex is within `threshold` units of its predecessor.

####`home`
Revisit the starting vertex on the current path.

###Mathematical Functions

####`max a, b`
Find the maximum of two numeric values.

####`min a, b`
Find the minimum of two numeric values.

####`log base, x`
Find the logarithm of `x` in the given base.

####`random lo, hi`
Compute a random integer between `lo` and `hi`, both inclusive and both of which are expected to be integers.

####`sign x`
Compute the sign of `x`, which -1 for `x < 0`, 1 for `x > 0`, and 0 otherwise.

###Trigonometric Functions

####`sin degrees`, `cos degrees`, `tan degrees`
Compute the sine, cosine, or tangent of the given degrees.

####`asin ratio`
Compute the inverse sine of the given ratio. The ratio is expected to be in \[-1, 1\].

####`acos ratio`
Compute the inverse cosine of the given ratio. The ratio is expected to be in \[-1, 1\].

####`atan ratio`
Compute the inverse tangent of the given ratio.

####`atan2 opposite, adjacent`
Compute the inverse tangent of the ratio `opposite / adjacent`. Unlike `atan`, this version of the inverse tangent yields a number of degrees that reflects which quadrant this ratio lands in.

###Array and String Functions

####`all array`
Determine if all the array's elements are true.

####`any array`
Determine if at least one of the array's elements is true.

####`size array`, `size string`
Get the number of items in `array` or the number of characters in `string`.

###Vector Functions
The following functions deal with vectors, which are really just arrays of numbers.

####`cross a, b`
Compute the cross product of `a` and `b`, which are assumed to be three-vectors.

####`dot a, b`
Compute the dot product of `a` and `b`, which are assumed to be three-vectors.

####`magnitude v`
Compute the magnitude of the vector `v`.

####`normalize v`
Compute a new vector having the same direction as `v` but with length 1.

##User Functions
In addition to the builtin functions, you may write your own. Multiline functions have this form:

{:.mup}
~~~
to sum3 a b c
  a + b + c
end
~~~

One-liners may be expressed more compactly:

{:.mup}
~~~
to sum3 a b c = a + b + c
~~~

Variables first appearing within the function are not visible beyond the function's lifetime. There is no explicit return statement. A function's return value is derived from the last expression executed:

{:.mup}
~~~
to average a b c
  sum = a + b + c
  sum // 3
end
print average 10, 20, 30 -- prints 20
print sum -- illegal, sum unknown
~~~

----------------------------------------------------------------------------------

{:style="text-align: center"}
[Home](introduction.html)
[Previous](surface.html){:.floatleft}
