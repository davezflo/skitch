# Skitch 1.0 Manual

## Concept
Skitch is an experimental, interpreted language enabling you to create and manipulate simple objects in 3d. The original design goals include:

1. Describing 3d objects using a simple grid scheme
2. Establishing a "game loop"
3. Applying rotation and translation
4. Enabling input mechanisms 
5. Playing with object states

## Elements of a skitch program
The elements of a skitch program include:

1. Shape descriptions
2. An object generator
3. Actions and interactions

## Shape Descriptions
We will use some examples to explain how to describe objects. Below is the definition of a pyramid. We begin by using the keyword **shape** followed by the variable name "pyramid". We then define each point. A point is a **p** followed by a number (i.e. p0, p1, p2, etc). On that same line, we define the x, y, and z coordinates of the point. In the example below, p0 is the point (-0.2, 0.0, 0.2). The value of the variable e is ultimately replaced by 0.2 when the program runs. 

```C
use colors
num e = 0.2

shape pyramid {
    p0 -e 0.0 e
    p1 0.0 e 0.0
    p2 e 0.0 e
    p3 e 0.0 -e
    p4 -e 0.0 -e

    @ p0 p1 p2 red red
    @ p2 p1 p3 green green
    @ p3 p1 p4 blue blue
    @ p4 p1 p0 purple purple
    @ p0 p2 p3 yellow yellow
    @ p3 p4 p0 yellow yellow 
}
```
A few things to note here as well:

1. You can define floating point variables as num varname = float.
2. All numbers must have a decimal point to be valid.
3. You can negate variables (i.e. -e is equivalent to -0.2).

### Defining Faces
After we define the points (p0 through p4) we can then use them to define a face. To define a face, we use the **@** symbol followed by three points and two colors. **@ p0 p1 p2 red red** defines a face using p0, p1, and p2 where both the front and back side of the face are red. Faces are only defined with three points. This makes rendering simpler. 


### A word about color definitions
Note too, that we **use** colors. Colors is another .sk file that contains the following:

```C
color purple = 0xff 0x00 0xff 0xff
color red = 0xff 0x00 0x00 0xff
color green = 0x00 0xff 0x00 0xff
color darkgreen = 0x00 0x55 0x00 0xff
color blue = 0x00 0x00 0xff 0xff
color yellow = 0xff 0xff 0x00 0xff
color white = 0xff 0xff 0xff 0xff
color black = 0x00 0x00 0x00 0x00
color gray = 0x55 0x55 0x55 0xff
```

A color is defined using the keyword **color**. The variable name for the color follows and we set it equal to a 4-part color definition (red_channel, green_channel, blue_channel, alpha). Note that we are using hex for the different components. In hindsight, the syntax and the decision to use hex in one instance and floating point in the other seems odd, but - it is what it is. 

### The Grid
I attempted to make the description of objects as simple as possible. To do this:

* (0.0,0.0,0.0) is that the center of the screen. 
* (-1.0, 0.0, 0.0) is the left side of the screen and (1.0, 0.0, 0.0) is the right side of the screen. 
* (0.0, -1.0, 0.0) is the top of the screen and (0.0, 1.0, 0.0) is the bottom of the screen.
* The z-component depends on your choice of the camera location, but typically I like to think of z < 0.0 is closer to the camera and z > 0.0 is getting farther out. 

Generally, though. Camera position will skew all of this. By default, the camera is set at (0.0, 0.0, -5.0). This skews the placement of the object somewhat.  

## Generator
A generator is the primary mechanism to establish an entity within the 3d space. It is...regrettable in it's over-engineering and likely the first thing I tackle after I complete this manual. Let's keep it simple for now. A generator:

```C
generator genpyramid {
    pyramid mypyramid(1.0)
    o: 0.0 0.0 0.0
}
```
Within the above listing, we are creating a **generator** called genpyramid. Within that, we are declaring 1.0 instances of a pyramid type (shape) called mypyramid. Note that while I'd like to use _ in the name of the variable mypyramid, I can't as variable names can only be made up of letters in the alphabet. 

The next line which begins with **o:** tells us where to place *mypyramid* - dead center of the screen or the origin point. So *mypyramid* will originate at (0.0, 0.0, 0.0). Note that when you run this, skitch will find the center of the object and align that with the origin point. 

Now - with all this information - you should be able to run the following code and play around with a few things:

```C
color red = 0xff 0x00 0x00 0xff
color green = 0x00 0xff 0x00 0xff
color blue = 0x00 0x00 0xff 0xff
color purple = 0xff 0x00 0xff 0xff
color brown = 0xff 0xff 0x00 0xff

num e = 0.5

shape pyramid {
    p0 -e 0.0 e
    p1 0.0 e 0.0
    p2 e 0.0 e
    p3 e 0.0 -e
    p4 -e 0.0 -e
    @ p0 p1 p2 red red
    @ p2 p1 p3 green green
    @ p3 p1 p4 blue blue
    @ p4 p1 p0 purple purple
    @ p0 p2 p3 brown brown
    @ p3 p4 p0 brown brown
}

generator genpyramid {
    pyramid mypyramid(1.0)
    o: 0.0 0.0 0.0
}
```

1. Note that the blue face is facing you when you run this. Can you make the red face face you?
2. Can you cause the pyramid to shift slightly to the left? What about up? 
3. Can you make the pyramid get closer to you? Farther away? How did you do that?

## Actions
With the basics down, we can now start to look at actions and action sequences. An action allows us to do something with the object we've generated. Let's have a look at the changes to the code:

```C
action rotate {
    init:
        o.ry += 1.0!
        o.rx += 2.0!
        _ -> init
}

generator genpyramid {
    pyramid mypyramid(1.0)
    a: rotate()
    o: 0.0 0.0 0.0
}

```
Here, we are defining an action that we're calling rotate. All actions define at least one action sequence called **init:**. An action sequence defines a sequence of statements which can be:

1. An assignment
2. A transition
3. A thread execution
4. A ticket punch (used like a semaphore)

There's a lot to actions, so let's start simply and then build from there. In the above code, the **action** *rotate* has one action sequence called init. When executed, the action operates within the context of the object. That context defines a variable called **o** which represents the object to which the action is attached (in this case, *mypyramid*). 

The contextual object **o** defines several member values that can be used to manipulate it:

* rx - x rotation.
* ry - y rotation. 
* rx - x rotation.
* tx - translation along the x-axis.
* ty - translation along the y-axis.
* tz - translation along the z-axis. 

So, our action *rotate* first causes whatever object it is attached to (mypyramid in this case) to rotate first in the y-direction by 1 degree (all rotations are in degrees) and then in the x-direction by 2 degrees. It then repeats. The underscore (_) indicates a default match (we'll get to this in more detail later). When it matches, it will goto (->) the label that follows it, in this case *init*. 

### Attaching Actions
With the rotate action defined, let's see how we can attach it to the generated object. To do this, we use **a:**, followed by the name of the action to attach (rotate, in this case). Note that *rotate* is followed by (). We can pass parameters to actions, as we'll see later. 

#### Some Other Notes (probably ill-placed)
In addition to the contextual **o**, we also have **c** for camera and **p** for player. How do we know who the "player" is? There can be only one player and you generate it like any other object:

```C
generator genplayer {
    someshape player(1.0)
    o: 0.0 0.0 0.0
}
```
Player is a special object within skitch and this allows you to call it out specifically in actions that are attached to other objects. More on this later.

## Maths and other Symbols

Mathematical statements can be included in action sequences. The following mathematical operators are available:

* +, -, /, *, %
* cos, sin
* +=

The following comparison operators are also available: >, <, >=, <=. As earlier mentioned, when transitioning from on sequence to another -> is used. 

Examples are useful - here are a few valid statements in an action sequence:

```C
action sampleaction {
    init:
        angle = angle speed + 360.0 %!
        c.tx = o.tx angle 180.0 / pi * cos r * +!
        c.lz <= c.tz -> back
        _ -> init
}
```
Let's examine each statement in the **init** action sequence. As you can see by the first statement, math in skitch is done with reverse polish notation or RPN. In this form, you first introduce the arguments for an operator and then introduce the operator. So:

1. angle = (angle+speed)%360. We first add speed to angle and then compute modulo 360. This will always give us an angle between 0 and 359. 
2. c.tx = cos((angle/180.0)*pi)*r+o.tx. Looking at this, you can see that RPN is merely a stack based system. We put o.tx, angle, and 180.0 on the stack. The operator / then pops angle and 180.0 off the stack and computes the division and places the result back on the stack. We then add pi onto the stack and multiply (angle/180)*pi and add that to the stack and so on. 
3. c.lz <= c.tz -> back. If c.lz is less than or equal to c.tz, then goto back. 
4. _ -> init. If you get here, then goto init. 

Finally - note the exclamation mark at the end of these mathematical statements. They note the end of the statement. 

