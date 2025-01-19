# Nuclear Technique Course nucTech simulation

---

## README File

**Julien Bordes** (a), *email*: julien.bordes@york.ac.uk

*(a) School of Physics, Engineering and Technology, University of York, York, 
     YO10 5DD, United Kingdom*

The nucTech simulation was developed for the University of York's Nuclear 
Technique course to demonstatre the definition of source and geometry in Geant4. 

---

## 1. PREPARING ALIASES AND ENVIRONMENT SETUP

Open your bash configuration file:

```bash
gedit ~/.bashrc &
```

Add the following lines to load the required modules:

```bash
module purge
ml devel/CMake/3.26.3-GCCcore-12.3.0   # ml alias for 'module load'
ml phys/Geant4/11.2.2-GCC-12.3.0
ml phys/ROOT/6.30.06-foss-2023a
```

Reload your bash settings:

```bash
source ~/.bashrc
```

## 2. SETTING UP THE Geant4 APPLICATION

Navigate to the simulation directory:

```bash
cd NucTech
```

Create a build directory and change into it:

```bash
mkdir build
cd build
```

Configure the build system with CMake:

```bash
cmake ..
```

Compile the code:

```bash
make
```

## 3. RUNNING THE SIMULATION
### 3.1. Interactive mode

To run the simulation interactively with a visualisation:

```bash
./nucTech
```

This opens the Geant4 interactive window, where you can enter commands manually.

### 3.2. Batch mode

To run the simulation in batch mode using a macro file (e.g., run.mac), type:

```bash
./nucTech run.mac
```

By default, the simulation writes its output to a file named out.root. You can 
change its name by providing an additional argument:

```bash
./nucTech run.mac newName.root
```

In this example, the output file would be renamed newName.root.

## 4. DEFINING THE SOURCE

For example, to switch to a 200 MeV proton beam edit the beam.mac file:

```bash
gedit beam.mac &
```

Modify the beam parameters in beam.mac as follows:

```bash
/gps/particle proton        # Set particle type to proton
/gps/energy/mono 200 MeV    # Set beam energy to 200 MeV
/run/beamOn 1000            # Number of events (adjust as needed)
```
Run the simulation in batch mode:

```bash
./nucTech run.mac
```

Note: In the run.mac file you can replace beam.mac by another macro defining 
another source such as ionBeam.mac or radioactiveSrc.mac .

## 5. PROCESSING THE SIMULATED DATA

After the simulation completes, analyze the output file using the ROOT macro 
processSim.C

Open a ROOT session:

```bash
root -l
```

In the ROOT prompt, compile and execute the analysis macro:

```bash
.L processSim.C+
processSim()
```

### 3.2. Batch mode

By default, processSim() uses the following parameter values:

```bash
const TString inFileName   = "out.root";               // The ROOT file to analyze
const TString outFileName  = "energyDistribution.png"; // Output plot filename
const int     nEvts        = -1;                       // Number of events to process (-1 means all)
const double  E_threshold  = 0.;                       // Energy threshold for analysis
const double  E_resolution = 0.;                       // Energy resolution smearing
```

To process a different input file or produce a different output plot, you can 
pass arguments to the function explicitly. For example:

processSim("simOutput.root", "beamPlots.png", 5000, 0.01, 5.0)

Here, the parameters are:

```bash
    Input ROOT file: "simOutput.root"
    Output image file: "beamPlots.png"
    Number of simulated events: 5000
    Energy threshold (in MeV): 0.01
    Energy resolution (FWHM in %): 5.0
```

### 3.2. Batch mode

The analysis produces several plots:

Top Left: Energy Spectrum
Top Right: Depth-Dose Curve (Energy deposited vs. depth)
Bottom Left: Transverse Beam Profile (XY, source view)
Bottom Right: Vertical Beam Profile (YZ, side view)