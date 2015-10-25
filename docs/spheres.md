#Spheres
The `spheres` solidifier very simply places a faceted sphere at each vertex on the path you walk. You can set each sphere's radius independently.

Instead of making the canonical snowman model, let's generate a collection of spheres in random locations with radii in \[1, 2\]. We specify the radius by assigning a value to the builtin `.radius` variable right before the `moveto`. The leading `.` is significant. It implies that `.radius` is an attribute of a vertex.

{:.mup}
~~~
seed = 30

repeat 150
  x = random -10, 10
  y = random -10, 10
  z = random -10, 10
  .radius = 0.1 * random 10, 20
  moveto x, y, z
end
~~~

![Dots Path](images/dots_path.png){:.bigimg}

Note that the path is quite messy. The `spheres` solidifier focuses only the vertices, completely disregarding the connections between them.

Let's add in the solidifier call:

{:.mup}
~~~
seed = 30

repeat 150
  x = random -10, 10
  y = random -10, 10
  z = random -10, 10
  .radius = 0.1 * random 10, 20
  moveto x, y, z
end

**spheres**
~~~

![Dots Octahedrons](images/dots_octahedrons.png){:.bigimg}

Those aren't spheres. They're octahedrons. As with `revolve`, the smoothness of our model is determined by another builtin variable: `nsides`. Let's increase the value from the default value of 4:

{:.mup}
~~~
seed = 30

repeat 150
  x = random -10, 10
  y = random -10, 10
  z = random -10, 10
  .radius = 0.1 * random 10, 20
  moveto x, y, z
end

**nsides = 25**
spheres
~~~

![Dots Rounded](images/dots_rounded.png){:.bigimg}

Note that variable `nsides` is a model attribute and not a vertex attribute. It only needs to be set before the call to `spheres`.

##Worm
Let's consider one more example to illustrate an issue that you will occasionally encounter when building models. We draw a little sinusoidal worm with the following program:

{:.mup}
~~~
nsides = 50
.radius = 20
amplitude = 70

for y in 0,20..360
  x = amplitude * sin y
  moveto x, y, 0
end

spheres
~~~

![Dots Worm](images/dots_worm.png){:.midimg}

Our first example yielded a bunch of loose spheres, which is hardly a printable model. Now we have a model with overlapping spheres. Technically, this is a solid and should be printable, but the intersecting geometry will cause trouble for many printers. We'll first want to run our model through a repair tool like Netfabb. Someday Madeup may repair models automatically. Someday.

##Next
The `spheres` solidifier is useful for chemists and people wanting to add eyes to their models. Rounded models present challenges to many printers. The companion [`boxes`](boxes.html) solidifier, on the other hand, is happy to make very ordinary and printable models.
