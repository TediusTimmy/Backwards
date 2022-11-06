The "Walk" activity has four stages.

1) The first stage is a dispatch stage, that I only made because I broke the
behavior into multiple states. It ensures that there is a correct direction in
the argument loop and queues up the three states.

2) The first thing that happens is that we look to ensure that the wall is
still to our right. It is a wall-following algorithm utilizing a right-hand
rule. So, we look to the right, allow the action to happen, and proceed to the

3) We have the feedback from the Look. If there isn't a wall to our right,
turn right and proceed to the next state. If there is a wall, we now look
ahead of us, to ensure that we aren't walking into a wall. We allow the action
to happen and proceed to the next state.

4) If there isn't a wall in front of us, walk forward and allow the action to
happen. If there is a wall in front of us, turn left. After either path, queue
up the first state and do it all over again.
