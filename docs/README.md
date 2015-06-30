#Madeup
Madeup is a programming language for generating 3D models. It is a language for makings things *up*. Its speakers use commands like `move` and `yaw` to walk through space along the cross sections or skeletons of solid models. They then use commands like `extrude` or `revolve` to generate a solid model. The models can then be printed or imported in another 3D application.

Like any language, Madeup has a grammar and meaning that must be learned in order to express models. This document describes the language so that you can start making things up.

##Examples

##Generators

###`dot`

![Random Dots](images/random_dots.png){:.center}

{:.mup}
~~~
seed = 92

repeat 30
  radius = random 2, 10
  x = random -20, 20
  y = random -20, 20
  z = random -20, 20
  moveto x, y, z
end

nsides = 50
dot
~~~

###`box`

![Random Dots](images/random_dots.png){:.center}

{:.mup}
~~~
seed = 92

repeat 30
  radius = random 2, 10
  x = random -20, 20
  y = random -20, 20
  z = random -20, 20
  moveto x, y, z
end

nsides = 50
dot
~~~

###`extrude`

###`revolve`

###`polygon`

###`forget`

###`surface`

##Language Reference

###Literals

###Expressions and Operators

####Association (`(expr)`)

####Absolute Value (`|expr|`)

####Indexing and Selection (`[]`, `.property`)

####Logical Negation (`not`)

####Exponential (`^`)

####Multiplicative (`*`, `/`, `//`, `%`)

####Additive (`+`, `-`)

####Relational (`<`, `<=`, `>`, `>=`)

####Equality (`==`, `!=`)

####Assignment (`=`)

####Conjunction (`and`)

####Disjunction (`or`)

###Builtin Variables

###User Variables

###Loops

###Conditionals

###Builtin Functions

####`print message`

####`debug message`

####`moveto x, y, z`

####`move length`

####`yaw degrees`

####`pitch degrees`

####`roll degrees`

####`translate x, y, z`

####`rotate x, y, z, degrees`

####`scale x, y, z`

####`identity`

####`reverse`

####`sin degrees`

####`cos degrees`

####`tan degrees`

####`asin ratio`

####`acos ratio`

####`atan ratio`

####`atan2 opposite, adjacent`

####`max a, b`

####`min a, b`

####`log base, x`

###User Functions

