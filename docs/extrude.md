#Extrude
The `extrude` solidifier interprets the walked path as the cross section of a model and elongates it along some direction. As an example, let's extrude a star.

As we trace out closed polygons, our thinking about the algorithm will be more fruitful if we recognize that we will need to turn a total 360 degrees. In the case of a star, we have five arms, each of which will net us 72 degrees toward the 360.

Let's start with a single arm. We move out to its point, then turn and trace out the other side of the arm. Note that we effectively turn 72 degrees clockwise. At the tip of the arm, we turned 144 degrees clockwise, but we lost 72 when at the end we turned counterclockwise to get ready for the next arm.

{:.mup}
~~~
moveto 0, 0, 0

move 10
yaw 144
move 10
yaw -72
~~~

![Star Arm Path](images/star_arm_path.png){:.bigimg}

Now we draw five arms instead of one. We wrap our work for one arm up in a `repeat` loop:

{:.mup}
~~~
moveto 0, 0, 0

**repeat 5**
  move 10
  yaw 144
  move 10
  yaw -72
**end**
~~~

![Star Path](images/star_path.png){:.bigimg}

The path is closed, and now we can pull it out to make a solid. We'll pull along the positive z-axis for 4 units:

{:.mup}
~~~
moveto 0, 0, 0

repeat 5
  move 10
  yaw 144
  move 10
  yaw -72
end

**extrude 0, 0, 1, 4**
~~~

![Star Extruded](images/star_extruded.png){:.bigimg}

The pulling direction doesn't have to be perpendicular to the path. Also, for the extrusion to yield a solid, the path will be automatically closed if you haven't closed it yourself.

##Next
The `extrude` solidifier is great for stretching your model's cross section out into a solid. But what if instead of pulling the cross section out, we spun it around an axis? Read on to learn about [`revolve`](revolve.html).
