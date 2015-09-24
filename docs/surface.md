#Surface
The `surface` solidifier is the least automatic of all the solidifiers. Unlike the others, which let you focus on a model's skeleton or cross section, with `surface` you must visit every vertex in your model. To show what it does with your vertices, let's build the simplest surface I can think of: a plane.

{:.mup}
~~~
for row to 5
  for column to 6
    moveto column, row, 0
  end
end
~~~

![Surface Plane Path](images/surface_plane_path.png){:.bigimg}

When we issue a `surface` solidifier, it will treat our list of vertices as if they were part of a 2D grid and it will connect each vertex with its four adjacent neighbors to form a surface. (Vertices on the corners have only two neighbors, and those on edges have only three.) You must communicate the dimensions of this grid to `surface`:

{:.mup}
~~~
**width = 6**
**height = 5**

for row to **height**
  for column to **width**
    moveto column, row, 0
  end
end

**surface width, height**
~~~

![Surface Plane](images/surface_plane.png){:.bigimg}

A wireframe view might be more illuminating:

{:.mup}
~~~
width = 6
height = 5

for row to height
  for column to width
    moveto column, row, 0
  end
end

surface width, height
~~~

![Surface Plane Wireframe](images/surface_plane_wireframe.png){:.bigimg}

##Cone
The grid of vertices does not have to be flat. The vertices can be placed anywhere. Let's curl it around the y-axis to make a cylinder. We use a vertex's column index to determine where on the circular profile it lands. We convert the degrees to an xz Cartesian coordinate pair using some trigonometry:

{:.mup}
~~~
width = 6
height = 5

for row to height
  for column to width
    **proportion = column / (width - 1.0)**
    **degrees = 360 * proportion**
    **x = cos degrees**
    **z = sin degrees**
    **moveto x, row, z**
  end
end

surface width, height
~~~

![Surface Cylinder Path](images/surface_cylinder_path.png){:.midimg}

What happens when we render this?

{:.mup}
~~~
width = 6
height = 5

for row to height
  for column to width
    proportion = column / (width - 1.0)
    degrees = 360 * proportion
    x = cos degrees
    z = sin degrees
    moveto x, row, z
  end
end

surface width, height
~~~

![Surface Cylinder Inverted](images/surface_cylinder_inverted.png){:.midimg}

Unlike the other solidifiers, `surface` doesn't know what is inside and what is outside your model. It will orient the faces in a default way that may not be correct. Usually we can fix this just by walk the grid in the opposite direction. In our case, we can just negate the degrees:

{:.mup}
~~~
width = 6
height = 5

for row to height
  for column to width
    proportion = column / (width - 1.0)
    degrees = **-**360 * proportion
    x = cos degrees
    z = sin degrees
    moveto x, row, z
  end
end

surface width, height
~~~

![Surface Cylinder](images/surface_cylinder.png){:.midimg}

Before we make this a cone, let's smooth it out a bit by increasing the number of stops on the circle:

{:.mup}
~~~
width = **100**
height = 5

for row to height
  for column to width
    proportion = column / (width - 1.0)
    degrees = -360 * proportion
    x = cos degrees
    z = sin degrees
    moveto x, row, z
  end
end

surface width, height
~~~

![Surface Cylinder Smooth](images/surface_cylinder_smooth.png){:.midimg}

Now we taper the cylinder by adjusting the circle's radius in proportion to the row we are on in the grid:

{:.mup}
~~~
width = 100
height = 5

for row to height
  for column to width
    proportion = column / (width - 1.0)
    degrees = -360 * proportion
    **radius = row / (height - 1.0)**
    x = **radius * cos degrees**
    z = **radius * sin degrees**
    moveto x, row, z
  end
end

surface width, height
~~~

![Surface Cone](images/surface_cone.png){:.midimg}

##Next
The `surface` solidifier is best suited for building models described by 2D parametric equations. These include planes, cones, spheres, terrain, waves, Klein bottles, and an infinite number of others.

You've learned about each of the solidifiers currently available in Madeup. Perhaps you'd like to read in more detail about the [Madeup language](reference.html)?
