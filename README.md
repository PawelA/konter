Right now it only works under X11 linux, but more targets shouldn't
be too hard to add. The collision engine is disabled right now. It is
there, but it's a bit buggy and too slow to handle the stanford
bunny. In short, it computes the minkowski sum of the terrain and the
player's hitbox (which must be a cuboid), and then performs pointwise
collision testing. Faster testing via kd/oct/bsp-trees or something
else is planned.