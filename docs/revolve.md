#Revolve
Like `extrude`, the `revolve` solidifier interprets the walked path as the cross section. Instead of elongating it, however, it spins it around some axis. We get to choose the axis, the number of degrees we spin, and how many facets appear in the revolved model.

For our example, let's create an axle with fixed wheels. We star by tracing out the cross section of the first wheel, the axle, and then second wheel. Let's use some variables to make the code easier to read:

{:.mup}
~~~
wheelThickness = 2
wheelRadius = 3
axleRadius = 1
axleLength = 5

yaw 90
moveto 0, 0, 0
move wheelRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius - axleRadius
yaw -90
move axleLength
yaw -90
move wheelRadius - axleRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius
~~~

![Axle Path](images/axle_path.png){:.bigimg}

Now we're ready to spin this path around. Since our axle is currently standing up, we'll spin around the y-axis:

{:.mup}
~~~
wheelThickness = 2
wheelRadius = 3
axleRadius = 1
axleLength = 5

yaw 90
moveto 0, 0, 0
move wheelRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius - axleRadius
yaw -90
move axleLength
yaw -90
move wheelRadius - axleRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius

**revolve 0, 1, 0, 360**
~~~

![Axle Square](images/axle_square.png){:.bigimg}

Hmm, our gas mileage is going to be pretty terrible with those square wheels. Why did this happen? The `revolve` solidifier depends on a special builtin variable named `nsides` to determine how often the cross section is replicated during the revolving. Its default value is 4.

To smooth the model out, we can increase the value of `nsides` at any point before we call `revolve`.

{:.mup}
~~~
wheelThickness = 2
wheelRadius = 3
axleRadius = 1
axleLength = 5

yaw 90
moveto 0, 0, 0
move wheelRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius - axleRadius
yaw -90
move axleLength
yaw -90
move wheelRadius - axleRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius

**nsides = 30**
revolve 0, 1, 0, 360
~~~

![Axle Rounded](images/axle_rounded.png){:.bigimg}

We don't have to revolve a full circle. We can get 3/4 of an axle by only revolving 270 degrees:

{:.mup}
~~~
wheelThickness = 2
wheelRadius = 3
axleRadius = 1
axleLength = 5

yaw 90
moveto 0, 0, 0
move wheelRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius - axleRadius
yaw -90
move axleLength
yaw -90
move wheelRadius - axleRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius

nsides = 30
revolve 0, 1, 0, **270**
~~~

![Axle Partial](images/axle_partial.png){:.bigimg}

Our axle started and ended on the y-axis, which happened to be our axis of revolution. The revolved path doesn't have to be anywhere near this axis. If we move the path away, say 3 units to the right, we'll get a hole in the center of the model. After moving, we'll need to close the path to get the inner walls of this hole. Let's see what happens when we move the axle path three units to the right:

{:.mup}
~~~
wheelThickness = 2
wheelRadius = 3
axleRadius = 1
axleLength = 5

yaw 90
moveto **3**, 0, 0
move wheelRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius - axleRadius
yaw -90
move axleLength
yaw -90
move wheelRadius - axleRadius
yaw 90
move wheelThickness
yaw 90
move wheelRadius
**moveto 3, 0, 0**

nsides = 30
revolve 0, 1, 0, 360
~~~

![Axle Hollow](images/axle_hollow.png){:.bigimg}

##Next
The `revolve` solidifier excels at generating radially symmetric models. Speaking of radially symmetric, let's next check out drawing with spheres using the [`dots`](dots.html) solidifier.
