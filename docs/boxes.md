#Boxes
The `boxes` solidifier is almost identical to `spheres`---it very simply places a cube at each vertex on the path you walk. You can set each box's size independently.

With `spheres`, we set each sphere's size using `.radius`. This same variable is used to set a box's size, with each side measuring 2 * `.radius`.

Let's make a voxelated sphere by walking along its surface and placing vertices. Traversing the surface of a sphere in a serial fashion is an interesting and difficult problem. We're going to make our lives easier and first consider all locations in a cubic region circumscribing the sphere having radius `reach`:

{:.mup}
~~~
reach = 4

for z in -reach..reach
  for y in -reach..reach
    for x in -reach..reach
      moveto x, y, z
    end
  end
end
~~~

![Boxes Cube Path](images/boxes_cube_path.png){:.bigimg}

It's not lost on me how ugly that is.

Now, let's see how far each location is from the sphere, and only if the distance is pretty close to the sphere's radius will we move to that location:

{:.mup}
~~~
reach = 4
**threshold = 0.6**

for z in -reach..reach
  for y in -reach..reach
    for x in -reach..reach
      **distance = (x * x + y * y + z * z) ^ 0.5**
      **if |distance - reach| <= threshold**
        **moveto x, y, z**
      **end**
    end
  end
end
~~~

![Boxes Sphere Path](images/boxes_sphere_path.png){:.bigimg}

That's looking better. If you look closely, you'll see no locations inside the sphere. We've also eliminated many outside the sphere. We've culled out everything but the spherical shell. Now, let's put boxes at each of those locations. To make 1x1x1 cubes, we set `.radius` to 0.5 before our loops start:

{:.mup}
~~~
reach = 4
threshold = 0.6
**.radius = 0.5**

for z in -reach..reach
  for y in -reach..reach
    for x in -reach..reach
      distance = (x * x + y * y + z * z) ^ 0.5
      if |distance - reach| <= threshold
        moveto x, y, z
      end
    end
  end
end

**boxes**
~~~

![Boxes Sphere](images/boxes_sphere.png){:.bigimg}

##Next
The `boxes` solidifier excels at building voxelated creations algorithmically. Like `spheres`, it may result in overlapping geometry that will need to be cleaned up before printing. But it's simple. On the other end of the complexity spectrum, we've got the [`surface`](surface.html) solidifier, which is the least automatic of them all.

----------------------------------------------------------------------------------

{:style="text-align: center"}
[Home](introduction.html)
[Previous](spheres.html){:.floatleft}
[Next](surface.html){:.floatright}
