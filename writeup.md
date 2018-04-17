---
geometry: margin=2cm
header-includes:
    - \usepackage{titlesec}
    - \titlespacing{\section}{0pt}{\parskip}{-\parskip}
    - \titlespacing{\subsection}{0pt}{\parskip}{-\parskip}
    - \titlespacing{\subsubsection}{0pt}{\parskip}{-\parskip}
---
# Programming Assignment 1: Congestion Control Contest
## Hudson Ayers, Paul Crews | CS 244
## Final Statistics: TODO

## Overview

## Exercise A: Window Size
![Window Graph](varying_window.png)

For this exercise, we wrote scripts to conduct two different tests. First,
we wrote a script that edited the controller.cc file to return different window
sizes from the window_size() function. This script tested 20 different window sizes
ranging from 10 packets to 200 packets. The results of our test can be seen in the
plot above. It is clear from the plot that increasing the window size improves throughput
at the expense of delay, and that for window sizes above 50 packets there was
little gain in throughput but significant expense in delay.

Next, we wrote a script that ran the test for a single fixed window
size of 50 packets 20 times. The averages and standard deviation of each of the
output measurements can be seen in the table below.

![Variance Table](measurement_variance2.jpg)

## Exercise B: AIMD Scheme

## Exercise C: Delay-Triggered Scheme

## Exercise D: The Contest
For the contest, our initial design was 

### Initial Approach
Our initial approach built on the code from Exercise C, while combining some
insights from the BBR paper. In particular, our approach assumed that the
network path was not changing significantly, and that the minimum RTT latency
is at least as large as the true RTT latency. As a consequence, our first
design kept track of the minimum RTT value, and adjusted the window size based
on any changes in this RTT. This approach was highly effective, and
significantly improved latency while keeping throughput reasonable.

### Additional Improvements

#### Quadratic Window Size Adjustments
After looking at the network traces, we noticed that the window size would
remain small for extended periods of time even though latency had dropped
back to a reasonable value. We attribute this to the fact that our
implementation did not attempt to measure the "true" bandwidth of the network.
After experimenting with several approaches, we decided to estimate the
current bandwidth by using an exponentially-weighted moving average, then
when under "good" network conditions (that is, when the measured RTT is close
to the true RTT) we rapidly expand the window to match the expanded size.
Similarly, if the window is over-extended and we start to see increases in the
measured RTT, we quickly cut the window size to be either equal to or less
than the estimated window size. This significantly improve throughput while
slightly increasing the measured latency.

#### Selecting Ideal Constants

### Overtraining Resistance

### Unsuccessful Efforts

#### Time Based Window Size Restoration

### Final Design

### Further Work

## Exercise E: Name


---
Warmup exercise A [10%]: Vary the fixed window size by editing controller.cc to
see what happens. Make a 2D graph of throughput vs. 95-percentile signal delay
(similar to what is seen on the contest analysis URLs) as you vary this value.
What is the best single window size that you can find to maximize the overall
"score" (log throughput/delay)? How repeatable are the measurements taken with
the same window size over multiple runs?

Warmup exercise B [40%]: Implement a simple AIMD scheme, similar to TCP's
congestion-avoidance phase. How well does this work? What constants did you
choose?

Warmup exercise C [20%]: Implement a simple delay-triggered scheme, where the
window rises or falls based on whether the round-trip-time crosses some
threshold value. Experiment with a few thresholds or tweaks and report on what
worked the best.

Exercise D [50%]: the contest. Try different approaches and work to maximize
your score on the final evaluation. Be wary about "overtraining": after the
contest is over, we will collect new network traces and then run everybody's
entries over the newly-collected evaluation trace. In your report, please
explain your approach, including the important decisions you had to make and
how you made them.  Include illustrative plots.

Exercise E [0%]: pick a cool name for your scheme!
