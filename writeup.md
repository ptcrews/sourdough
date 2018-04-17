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
output measurements can be seen in the table below. As can be seen in the table,
the variation between measurements is pretty small.

![Variance Table](measurement_variance2.jpg)

## Exercise B: AIMD Scheme

Our simple AIMD scheme increased the window size by 1 every time a full window
size of acks was received. Additionally, our scheme halved the window size whenever
a drop was detected via a repeated ACK or a timeout. Our timeout was set as a
constant of 1 second. This scheme did not work particularly well. 
TODO: What happened to our AIMD results?? What constants did we settle on??

## Exercise C: Delay-Triggered Scheme

For this exercise, we modified controller.cc such that the window size would
be reduced whenever an ack was received with a delay greater than UPPER\_THRESH,
and would be increased whenever an ack was received with a delay less than
LOWER\_THRESH. We found that increasing and decreasing the window size by 1 for
each ack resulted in too rapid of changes, so we changed the window size variable
to be a double (which we would cast to an integer before returning it to sender.cc)
and instead incremented the variable by 0.1 per ack received when the ack delay fell
within one of the specified ranges. We tried several values for UPPER\_THRESH and 
LOWER\_THRESH, such as 900ms/300ms, which gave a power score of 3.545, and 
600ms/200ms, which gave a power score of 4.92. Upon recognizing that this method still
sometimes resulted in window sizes that were either too large or too small, we bounded
the possible window size which could result using LOWER\_BOUND and UPPER\_BOUND 
constants. After fiddling with different values for all four of these constants, we
settled on UPPER\_THRESH = 400ms, LOWER\_THRESH = 150ms, UPPER\_BOUND = 125 packets,
and LOWER\_BOUND = 30 packets. This resulted in a throughout of 4.73 Mbits/s, a signal
delay of 518 ms, and a power score of 9.13. These are the constants used in the git branch
labeled TODO:XXXXXXXXXXXXXX. 

## Exercise D: The Contest

### Initial Approach
Our initial approach built on the code from Exercise C, while combining some
insights from the BBR paper. In particular, our approach assumed that the
network path was not changing significantly, and that the minimum 
measured RTT latency over the 2 minute interval
is at least as large as the true RTT latency. As a consequence, our first
design kept track of the minimum RTT value observed, and adjusted the window size based on
the difference between the measured RTT of each received acknowledgement and the minimum RTT.
Specifically, we set a constant rtt\_delta in controller.hh, and for our initial
approach we would increase the window size if the measured RTT was less than 
minRTT + rtt\_delta, and would decrease the window size by 0.1 every time an 
ack was received such that the measured RTT was more then minRTT + rtt\_delta.
This approach was highly effective, and
significantly improved latency while keeping throughput reasonable.

### Additional Improvements

#### Multiple Window Size Adjustments
Our next thought in this process was that applying a constant adjustment to the
window size regardless of how far the measured RTT was from the minRTT + 
rtt\_delta boundary was not ideal behavior - when near the boundary, less 
adjustment was needed than when further from the boundary. To account for this,
we reduced the adjustment made when near the boundary, and increased the window
size adjustment when far from the boundary, by mapping 4 ranges of delay to 4 different
window size adjustments. This helped some, but we had some difficulty determining where 
exactly to place the cutoffs for these ranges, and how different the window size
adjustments should be. Our best attempts for this approach came when increasing
the window size by 0.3 for measurements that were 2\*rtt\_delta greater than
the minRTT, 0.1 for measurements only 1 rtt\_delta greater than the minRTT, and 
decreasing by 0.1 only when measurments were 0.5\*rtt\_delta less than minRTT + 
rtt\_delta.

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
than the estimated window size. This significantly improved throughput while
slightly increasing the measured latency.

#### Agressive Timeouts

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
