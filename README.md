# implicit-crowds

[Implicit Crowds: Optimization Integrator for Robust Crowd Simulation](http://people.cs.clemson.edu/~ioannis/implicit-crowds/)<br />
[Ioannis Karamouzas](http://people.cs.clemson.edu/~ioannis/
), Nick Sohre, [Rahul Narain](rahul.narain.name/), and [Stephen J. Guy](www-users.cs.umn.edu/~sjguy/
)<br/>
Clemson University, University of Minnesota

# Instructions
This software has been tested on Windows 7 and 10 with Visual Studio 2015. <br/>
It depends on [Eigen](eigen.tuxfamily.org/) and [callisto](www.nieuwenhuisen.nl/callisto/), 
which are included in the code. Callisto is used for visualization purposes and requires DirectX 9.0c. 
If you want to compile the code with x64 support, you should disable the visualizer. <br/>

After the code is compiled, to simulate a scenario go to the bin folder and run:</br>
"ImplicitCrowds -dt 0.2 -frames 80 -scenario ../../data/3_agents.csv -parameters ../../data/implicit.ini" <br/>

The *-dt* flag sets the size of the time step, the *-frames* indicates the maximum number of simulation steps, 
the *-scenario* takes as input the scenario file, and the *-parameters* flag reads the parameters related to the implicit crowd code. 
All but the *-scenario* flag are optional.

# TODO
* Add more scenarios
* Replace callisto with OpenGL
* Replace the distance energy with uncertainty in velocity based on http://www-users.cs.umn.edu/~foro0012/UTTC/


# Citation
<p>BibTex:</p>
<pre><code>
@article{karamouzas17,
 author = {Karamouzas, Ioannis and Sohre, Nick and Narain, Rahul and Guy, Stephen J.},
 title = {Implicit Crowds: Optimization Integrator for Robust Crowd Simulation},
 journal = {ACM Transactions on Graphics},
 volume = {36},
 number = {4},
 year = {2017},
 doi = {10.1145/3072959.3073705},
} 
</code></pre>
