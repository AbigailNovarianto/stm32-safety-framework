# Hasil Ekstraksi Otomatis — 16 Jurnal ETS Bagian B

> **Catatan**: Data diekstrak otomatis dari PDF. Periksa dan lengkapi kolom yang masih kosong (—) secara manual.

---

## Tabel Ringkasan Jurnal

| No | Judul | Penulis | Tahun | Nama Jurnal | DOI | Topik |
|----|-------|---------|-------|-------------|-----|-------|
| 1 | Comparison of Embedded System Design for Industrial Applicat | — |  | IEEE Transactions on Indu | 10.1109/TII.2011.212 | #1 |
| 2 | Energy Efficient Dual Core RISC V Architecture for Edge AI | Cristian Andy Tanase | 2026 | This paper presents a dua | 10.3390/computers150 | #1 |
| 3 | IET Power Electronics   2025   Lin   Hardware‐Accelerated Di | — | 2025 | IET Power Electronics 202 | 10.1049/pel2.70043 | #1 |
| 4 | Quantization and Deployment of Deep Neural Networks | Pierre-Emmanuel Novac, Ghouthi | 2021 | Embedding Artificial Inte | 10.3390/s21092984 | #1 |
| 5 | RTOS Integrated Time Synchronization for Self Deployable Wir | Sarah Goossens, Valentijn De S | 2026 | black0 g 0 GThe deploymen | 10.3390/s26072121 | #2 |
| 6 | Ultralight Situation Aware Hypervisor With Adaptive Starvati | — | 2025 | IEEE Access;2025;13; ;10. | 10.1109/ACCESS.2025. | #2 |
| 7 | uTango An Open Source TEE for IoT Devices | — | 2022 | IEEE Access;2022;10; ;10. | 10.1109/ACCESS.2022. | #2 |
| 8 | Design and Implementation of an Atrial Fibrillation Detectio | Marek yliski, Amir Nassibi and | 2023 | At present, a medium-leve | 10.3390/s23177521 | #3 |
| 9 | LLVM Based Efficient Hybrid Cache and TCM Memory Allocation  | — | 2025 | IEEE Embedded Systems Let | 10.1109/LES.2025.362 | #3 |
| 10 | An Ultra Low Power Time Domain Level Crossing ADC With Adapt | — | 2025 | IEEE Transactions on Circ | 10.1109/TCSII.2025.3 | #4 |
| 11 | Front End Gateway System With Serial Communication Protocol  | — | 2023 | IEEE Access; ;PP;99;10.11 | 10.1109/ACCESS.2023. | #4 |
| 12 | A C Based Framework for Low Cost Real Time Embedded Systems | Ivan Cibrario Bertolotti | 2025 | This paper presents a fra | 10.3390/fi17060269 | #5 |
| 13 | Energy efficient mixed task handling on real time embedded s | Deepak Ramegowda | 2022 | Journal of Systems Archit | 10.1016/j.sysarc.202 | #5 |
| 14 | Implementation of a Universal Framework Using Design | Marek Babiuch and Petr Foltyne | 2024 | This article focuses on t | 10.3390/s24103116 | #5 |
| 15 | Embedded Control Barrier Functions Concept and Application t | — | 2025 | IEEE Transactions on Indu | 10.1109/TII.2025.358 | #6 |
| 16 | Security and Functional Safety for AI in Embedded Automotive | — | 2024 | IEEE Transactions on Circ | 10.1109/TCSII.2023.3 | #6 |

---

## Detail per Jurnal

### [01] Comparison of Embedded System Design for Industrial Applications

- **File**: `Comparison_of_Embedded_System_Design_for_Industrial_Applications.pdf`
- **Topik**: #1
- **Penulis**: —
- **Tahun**: 
- **DOI**: `10.1109/TII.2011.2124466`

**Abstract/Intro:**
```
—This paper presents a survey on embedded systems II. DESCRIPTIONOFEMBEDDEDSYSTEMPLATFORMS
designandapplications.Severalplatformsforembeddedsystems,
includingmicrocontrollers,microprocessors,field-programmable Theembeddedsystemsarenormallydefinedasthesoftware
gate arrays, digital signal processors, and application-specific
implementedinhardwareinordertorealizespecifiedreal-time
integrated circuits are discussed and compared. A survey of
functionalities. The normally used soft-core processing hard-
embedded system-based industrial applications is presented.
```

**Future Work:**
```
(Future Work section not found — cek manual)
```

---

### [02] Energy Efficient Dual Core RISC V Architecture for Edge AI

- **File**: `Energy-Efficient Dual-Core RISC-V Architecture for Edge AI.pdf`
- **Topik**: #1
- **Penulis**: Cristian Andy Tanase
- **Tahun**: 2026
- **DOI**: `10.3390/computers15040219`

**Abstract/Intro:**
```
Thispaperpresentsadual-coreRISC-Varchitecturedesignedforenergy-efficientAIaccel-
erationattheedge,featuringdynamicMACunitsharing,frequencyscaling(DFS),and
FIFO-basedresourcearbitration. ThesystemcomprisestwoRISC-Vcoresthatcompete
for shared computational resources—a single Multiply–Accumulate (MAC) unit and a
shared external memory subsystem—governed by a channel-based arbitration mecha-
nismwithCPU-prioritysemantics,whileeachcoremaintainsprivateinstructionanddata
caches.
```

**Future Work:**
```
limitationforfuturemulti-coreextensions:
1. MultipleMACunitswithhierarchicalarbitration: ReplicatetheMACdatapath(e.g.,
oneperpairofCPUcores)andintroduceatwo-levelarbiterthatpartitionscoresinto
clusters,eachwithitsownMACandlocalNPUaccess.
2. DedicatedNPUMACwithsharedbackup: AssignoneMACunitexclusivelytothe
NPU for guaranteed AI throughput, while the CPUs continue to share a separate
MACunit,therebydecouplingCPUandNPUcontentionentirely.
3. Bandwidth reservation: Reserve a minimum fraction of MAC cycles for the NPU
(e.g.,20%)regardlessofCPUdemand,convertingthecurrentbest-effortpolicyintoa
guaranteed-minimum-bandwidthschemeatthecostofslightlyincreasedworst-case
CPUlatency.
```

---

### [03] IET Power Electronics   2025   Lin   Hardware‐Accelerated Digital Power Control for High‐Frequency Hybrid Energy Storage (1)

- **File**: `IET Power Electronics - 2025 - Lin - Hardware‐Accelerated Digital Power Control for High‐Frequency Hybrid Energy Storage (1).pdf`
- **Topik**: #1
- **Penulis**: —
- **Tahun**: 2025
- **DOI**: `10.1049/pel2.70043`

**Abstract/Intro:**
```
In the rapidly evolving field of electric vehicles (EVs), efficient energy storage systems are crucial for widespread adoption.
Hybridenergystoragesystems(HESS),whichcombinelithiumbatterieswithsupercapacitors(SCs),offerapromisingsolution
byimprovingpowerdensityandoverallsystemefficiency.Thispaperpresentsacost-effectiveapproachtoimplementinghigh-
frequency current controllers within an HESS using the general-purpose microcontroller STM32G474RB. By leveraging its
built-infiltermathaccelerator(FMAC),atypeIIcompensatorisimplemented,achieving250kHzcurrentcontroland500kHz
switchingfrequency.
```

**Future Work:**
```
Future
research directions could explore advancements in ADC sam-
plingtimetomitigatesamplingdelaysorinvestigatealternative
methodologies to ameliorate the impact of extended sampling
periodsonsystemperformance.
Motor Power
SC Power AuthorContributions
Wenhao Lin: formal analysis, investigation, methodology, writing –
original draft. Guanying Chu: formal analysis, funding acquisition,
methodology,supervision,writing–review&editing.
FIGURE 17 EVdeploymenttest.
ConflictsofInterest
Currently,mostdigitalcontrollersimplementedusingDSPshave
Theauthorsdeclarenoconflictsofinterest.
not exceeded the control frequency achieved in this study [12,
13, 15, 16]. In [20], a 400 MHz FPGA was used to implement a DataAvailabilityStatement
cycle-by-cyclecontrolledbuckconverter.
```

---

### [04] Quantization and Deployment of Deep Neural Networks

- **File**: `Quantization and Deployment of Deep Neural Networks.pdf`
- **Topik**: #1
- **Penulis**: Pierre-Emmanuel Novac, Ghouthi Boukli Hacene, Alain Pegatoquet, Benot Miramond a
- **Tahun**: 2021
- **DOI**: `10.3390/s21092984`

**Abstract/Intro:**
```
:EmbeddingArtificialIntelligenceontolow-powerdevicesisachallengingtaskthathas
beenpartlyovercomewithrecentadvancesinmachinelearningandhardwaredesign. Presently,
deep neural networks can be deployed on embedded targets to perform different tasks such as
speechrecognition,objectdetectionorHumanActivityRecognition.However,thereisstillroomfor
optimizationofdeepneuralnetworksontoembeddeddevices.Theseoptimizationsmainlyaddress
powerconsumption,memoryandreal-timeconstraints,butalsoaneasierdeploymentattheedge.
Moreover,thereisstillaneedforabetterunderstandingofwhatcanbeachievedfordifferentuse
cases.
```

**Future Work:**
```
(Future Work section not found — cek manual)
```

---

### [05] RTOS Integrated Time Synchronization for Self Deployable Wireless Sensor Networks

- **File**: `RTOS-Integrated Time Synchronization for Self-Deployable Wireless Sensor Networks.pdf`
- **Topik**: #2
- **Penulis**: Sarah Goossens, Valentijn De Smedt, Lieven De Strycker and Liesbet Van der Perre
- **Tahun**: 2026
- **DOI**: `10.3390/s26072121`

**Abstract/Intro:**
```
ThedeploymentofWirelessSensorNetworks(WSNs)remainschallengingandtimecon-
sumingduetothemanualcommissioning,configuration,andmaintenanceofresource-
constrainedInternetofThings(IoT)devices. Achievingprecisenetwork-widetimesyn-
chronizationinsuchsystemsfurtherincreasesthisdeploymentcomplexity. Thispaper
presents a novel Real-Time Operating System (RTOS)-integrated time synchronization
methodthatdistributesanabsoluteCoordinatedUniversalTime(UTC)referenceacross
thenetworkusingasingleGlobalNavigationSatelliteSystem(GNSS)-enabledhost.
```

**Future Work:**
```
future work, the synchronization frequency at the nodes can be adaptively re-
ducedtolowerenergyconsumptionwhilemaintainingtimingaccuracy. Whilethehost
periodicallyperformsGNSSsynchronizationandbroadcastsupdates,nodescaninitially
synchronize frequently to achieve accurate alignment and subsequently increase their
listening interval once the timing error falls below a defined threshold. This adaptive
approachenablesfastinitialsynchronizationwhilereducingwirelessactivityandenergy
consumptionovertime.
Inaddition,thecurrentnodesynchronizationmechanismcanbeintegratedintoregu-
lardatacommunication. Byembeddingtimestampsindatapacketsandacknowledgments,
https://doi.org/10.3390/s26072121
Sensors2026,26,2121 28of31
clockoffsetestimationcanbeperformedwithoutadedicatedsynchronizationthread.
```

---

### [06] Ultralight Situation Aware Hypervisor With Adaptive Starvation Free Feedback Control for Embedded System

- **File**: `Ultralight_Situation-Aware_Hypervisor_With_Adaptive_Starvation-Free_Feedback_Control_for_Embedded_System.pdf`
- **Topik**: #2
- **Penulis**: —
- **Tahun**: 2025
- **DOI**: `10.1109/ACCESS.2025.3594619`

**Abstract/Intro:**
```
In recent years, microcontrollers are increasingly using hypervisors to achieve efficient
and adaptable real-time performance in resource-constrained environments, enabling better resource
management and system isolation. However, traditional hypervisor scheduling approaches often rely on
staticschedulingorrequireallschedulingdecisionstobedeterminedatcompiletime,limitingtheirability
toadapttodynamicworkloadchangesandreal-timedemands.Toaddresstheseissues,thisstudyintroduces
aschedulingtechniquethatcombinesround-robinandpriorityscheduling.
```

**Future Work:**
```
future research
AWTmanagementacrossdiverseoperationalscenarios. is the integration of deadline-based scheduling with our
Regarding the scalability of the proposed framework for priority-basedapproach.Bycombiningthedynamicpriority
largerandmorecomplexreal-worldsystems,thelinearcom- adjustment capabilities of our current framework with
putational complexity(O(N)) of our scheduling algorithm deadline scheduling mechanisms, we aim to develop a
provides a strong foundation for predictable performance. more comprehensive scheduler that can better address hard
Asthenumberofconcurrentoperatingsystemsincreases,the real-time constraints while maintaining the benefits of
scheduling overhead grows manageably, not exponentially. adaptiveresourceallocation.
```

---

### [07] uTango An Open Source TEE for IoT Devices

- **File**: `uTango_An_Open-Source_TEE_for_IoT_Devices.pdf`
- **Topik**: #2
- **Penulis**: —
- **Tahun**: 2022
- **DOI**: `10.1109/ACCESS.2022.3152781`

**Abstract/Intro:**
```
SecurityisoneofthemainchallengesoftheInternetofThings(IoT).IoTdevicesaremainly
powered by low-cost microcontrollers (MCUs) that typically lack basic hardware security mechanisms to
separate security-critical applications from less critical components. Recently, Arm has started to release
Cortex-MMCUsenhancedwithTrustZonetechnology(i.e.,TrustZone-M),asystem-widesecuritysolution
aimingatprovidingrobustprotectionforIoTdevices.TrustedExecutionEnvironments(TEEs)relyingon
TrustZonehardwarehavebeenperceivedassafehavensforsecuringmobiledevices.
```

**Future Work:**
```
limitations and
NSVWisexecuting,delayingtheinterrupttobeserviced. possiblefutureimprovements.
D. CODEANDBINARYSIZE A. SECURESERVICES
uTangowasdevelopedfrom-scratchwithnodependencieson We intend to demonstrate a set of secure services, such as
compilerorexternallibraries.Table3reports(i)thenumber secure storage, cryptography operations, remote attestation,
ofSLoCand(ii)thebinarysize. and secure updates over-the-air. These secure services will
be encapsulated in dedicated NSVWs. However, depending
TABLE3. Sourcelinesofcode(SLoC)andbinarysize(bytes)bydirectory. on the peculiarities of the target platform, some hardware
modules may be hardwired to the secure world.
```

---

### [08] Design and Implementation of an Atrial Fibrillation Detection Algorithm on the ARM Cortex M4 Microcontroller

- **File**: `Design and Implementation of an Atrial Fibrillation Detection Algorithm on the ARM Cortex-M4 Microcontroller.pdf`
- **Topik**: #3
- **Penulis**: Marek yliski, Amir Nassibi and Danilo P. Mandic
- **Tahun**: 2023
- **DOI**: `10.3390/s23177521`

**Abstract/Intro:**
```
: At present, a medium-level microcontroller is capable of performing edge computing
and can handle the computation of neural network kernel functions. This makes it possible to
implementacompleteend-to-endsolutionincorporatingsignalacquisition,digitalsignalprocessing,
andmachinelearningfortheclassificationofcardiacarrhythmiasonasmallwearabledevice. In
thiswork, wedescribethedesignandimplementationofseveralclassifiersforatrialfibrillation
detectiononageneral-purposeARMCortex-M4microcontroller.
```

**Future Work:**
```
(Future Work section not found — cek manual)
```

---

### [09] LLVM Based Efficient Hybrid Cache and TCM Memory Allocation for Low Latency

- **File**: `LLVM-Based_Efficient_Hybrid_Cache_and_TCM_Memory_Allocation_for_Low-Latency.pdf`
- **Topik**: #3
- **Penulis**: —
- **Tahun**: 2025
- **DOI**: `10.1109/LES.2025.3625376`

**Abstract/Intro:**
```
—Cache memory has been introduced to accelerate
Cortex-M7
embedded system performance and is automatically managed DTCM
DTCM RAM
withoutprogrammerinterventionthroughhardware-basedcache L1 cache
controllers. However, since it operates based on typical variable
accesspatterns,opportunitiesforcomplementaryapproachesre-
AXI-to-AHB
main.Incontrast,ScratchpadMemory(SPM)orTightlyCoupled
Memory (TCM), which is closely coupled with the processor
core, require direct programmer control over data placement
AHB-BM
or compiler optimization support.
```

**Future Work:**
```
limitations: [3], [4]
coupled memory (TCM). Cache memory provides the advan- proposed additional hardware structures for SPM allocation,
tage of automatic system acceleration without programmer while [5] and [6] inadequately considered the development
intervention through dedicated cache controllers. However, environments commonly used in existing MCUs. Work by
since it operates based on typical variable access patterns, it [7] introduced a contention-aware approach using cache miss
cannot optimize the placement of all variables, and suffers counts as allocation metrics, but still requires dedicated hard-
from significant area overhead and high energy consumption. ware components and OS-level modifications, limiting its
In contrast, TCM can compensate for these shortcomings.
```

---

### [10] An Ultra Low Power Time Domain Level Crossing ADC With Adaptive Sampling Rate

- **File**: `An_Ultra-Low-Power_Time-Domain_Level-Crossing_ADC_With_Adaptive_Sampling_Rate.pdf`
- **Topik**: #4
- **Penulis**: —
- **Tahun**: 2025
- **DOI**: `10.1109/TCSII.2025.3592482`

**Abstract/Intro:**
```
—This brief presents a novel ultra-low-power (ULP)
time-domain level-crossing (TD-LC) analog-to-digital converter
(ADC) with an adaptive sampling rate. By integrating a non-
uniform LC sampling technique, the proposed TD-LC ADC
further reduces power consumption compared to conventional
TD ADCs. A voltage-to-time converter (VTC) is employed to
convert the input voltage signal into a time signal, which is
then subtracted from a time signal generated by a digital-
to-time converter (DTC), converting the digital output from Fig.1. BlockdiagramofconventionalTDADC.
the previous digital output.
```

**Future Work:**
```
(Future Work section not found — cek manual)
```

---

### [11] Front End Gateway System With Serial Communication Protocol Conversion and Edge Computing Platforms

- **File**: `Front-End_Gateway_System_With_Serial_Communication_Protocol_Conversion_and_Edge_Computing_Platforms.pdf`
- **Topik**: #4
- **Penulis**: —
- **Tahun**: 2023
- **DOI**: `10.1109/ACCESS.2023.3307631`

**Abstract/Intro:**
```
This study presents a front-end gateway system with asynchronous and synchronous serial
communication protocol conversion between the universal asynchronous receiver/transmitter (UART)
protocol and inter-integrated circuit (I2C) protocol. The valid data and sensing data can be integrated using
the I2C protocol and sent to an edge computing platform for automated data analysis. Our system reduces
data processing time and the number of buses.
```

**Future Work:**
```
(Future Work section not found — cek manual)
```

---

### [12] A C Based Framework for Low Cost Real Time Embedded Systems

- **File**: `A C-Based Framework for Low-Cost Real-Time Embedded Systems.pdf`
- **Topik**: #5
- **Penulis**: Ivan Cibrario Bertolotti
- **Tahun**: 2025
- **DOI**: `10.3390/fi17060269`

**Abstract/Intro:**
```
: Thispaperpresentsaframeworkthatenablesprogrammerstodeployembedded
real-timefirmwareofInternetofThings(IoT)devicesmoreconvenientlythanusingplain
C/C++-languageprogramming,byabstractingawayfromlow-leveldetailsandtheadhoc
managementofmultiple,diversenetworktechnologies. Moreover,unlikeotherproposals,
theframeworkisabletoaccommodatebothtimeandevent-drivenapplications. Experi-
mentalresultsshowthatforModbus-CANcommunication,theworst-casetimeoverhead
oftheframeworkislessthan6%ofthetotalcombinedprocessingandcommunicationtime.
```

**Future Work:**
```
(Future Work section not found — cek manual)
```

---

### [13] Energy efficient mixed task handling on real time embedded systems using FreeRTOS

- **File**: `Energy efficient mixed task handling on real-time embedded systems using FreeRTOS.pdf`
- **Topik**: #5
- **Penulis**: Deepak Ramegowda
- **Tahun**: 2022
- **DOI**: `10.1016/j.sysarc.2022.102708`

**Abstract/Intro:**
```
JournalofSystemsArchitecture131(2022)102708
ContentslistsavailableatScienceDirect
JournalofSystemsArchitecture
journalhomepage:www.elsevier.com/locate/sysarc
Energyefficientmixedtaskhandlingonreal-timeembeddedsystemsusing
FreeRTOS
DeepakRamegowda,ManLin∗
St.FrancisXavierUniversity,Canada
A R T I C L E I N F O A B S T R A C T
Keywords: WiththeincreaseduseofembeddeddevicesinIndustrialelectronics,likerelays,batterylifehasgainedmore
DVFS and more attention. Modern processors can use Dynamic Voltage Frequency Scaling (DVFS) techniques for
Powermanagement energy reduction and temperature control. Ho
```

**Future Work:**
```
future work, we want to take application-specific task execution
tobeperformeduponaperiodictaskarrival.Theresponsetimeisthe characteristicsintheFreeRTOSkernelandextendtheCCMTModelto
differencebetweenthearrivaltimeandcompletiontime. beusedinmultipleapplicationsonreal-timeprocessors.
8. Conclusionandfuturework Declarationofcompetinginterest
In this study, we describe the implementation and evaluation of The authors declare that they have no known competing finan-
theCCMTDVFStechniqueforMixedTasksetonanembeddeddevice cial interests or personal relationships that could have appeared to
poweredbyFreeRTOSwiththegoalofenergysaving. influencetheworkreportedinthispaper.
Wehaveachievedthefollowing:
Dataavailability
1. We have successfully extended the CCMT technique to handle
MixedTaskset.
```

---

### [14] Implementation of a Universal Framework Using Design

- **File**: `Implementation of a Universal Framework Using Design.pdf`
- **Topik**: #5
- **Penulis**: Marek Babiuch and Petr Foltynek
- **Tahun**: 2024
- **DOI**: `10.3390/s24103116`

**Abstract/Intro:**
```
:Thisarticlefocusesontheareaofsoftwaredevelopmentformicrocontrollersanddetails
theimplementationofmodernprogrammingpracticesandprinciplesinembeddedsystemsand
IoTapplications.Thisarticleexplainshowweimplementedpreviouslyunimplementedprinciples
andapplieddesignpatternsforqualitysoftwaredesignonmicrocontrollers,whicharecurrently
onlyusedfordevelopingapplicationsonthehigherlayersoftheIoTreferencemodel. Acustom
modularframeworkformicrocontrollersispresented, basedonapplyingSOLIDprinciplesand
adaptingdesignpatternsspecifictothemicrocontrollers’applicationdevelopmentneeds.
```

**Future Work:**
```
(Future Work section not found — cek manual)
```

---

### [15] Embedded Control Barrier Functions Concept and Application to Safety Critical Control Design of High Relative Degree Systems (1)

- **File**: `Embedded_Control_Barrier_Functions_Concept_and_Application_to_Safety-Critical_Control_Design_of_High-Relative-Degree_Systems (1).pdf`
- **Topik**: #6
- **Penulis**: —
- **Tahun**: 2025
- **DOI**: `10.1109/TII.2025.3584464`

**Abstract/Intro:**
```
—This article proposes a novel safety-critical I. INTRODUCTION
control (SCC) framework based on embedded control
SAFETYisacriticalperformanceindexinmoderncontrol
barrier functions (EMB-CBF-SCC) for high-order strict-
feedback nonlinear affine control systems. It is aimed at systemdesign,alongsidetraditionalstability,rapidity,and
reconcilingthepotentialconflictbetweenpredesignedde- accuracycriteria.Whenmobilerobotsormanipulatorsperform
sired trajectory and multiple safety constraints that could complextasks,theyareinevitablyrestrictedbyexternalenviron-
have different high relative degrees.
```

**Future Work:**
```
limitationinthe Fig.4. Case2:Thedesiredtrajectoryviolatesanglelimitationinthe
pale red area. (a) Joint angle curves of the different algorithms. (b) pale red area. (a) Joint angle curves of the different algorithms. (b)
Controlinputcurvesofthedifferentalgorithms.(c)Safetymodification Jointangularvelocitycurvesofthedifferentalgorithms.(c)Jointangular
process(NIVC⇒MIVC⇒SIVC)ofEMB-CBF-SCCtoarmaturecurrent. acceleration curves of the different algorithms. (d) Safety modification
(d)StateerrorsandsafetyconstraintvaluecurvesofEMB-CBF-SCC. process(NIVC⇒MIVC⇒SIVC)ofEMB-CBF-SCCtoarmaturecurrent.
nonlineartransformationfunctionbringslargefluctuationnear
the safety boundary, and causes opposite movement outside (cid:11) (cid:12)
it.
```

---

### [16] Security and Functional Safety for AI in Embedded Automotive SystemA Tutorial (1)

- **File**: `Security_and_Functional_Safety_for_AI_in_Embedded_Automotive_SystemA_Tutorial (1).pdf`
- **Topik**: #6
- **Penulis**: —
- **Tahun**: 2024
- **DOI**: `10.1109/TCSII.2023.3334273`

**Abstract/Intro:**
```
—The tutorial explores key security and functional
safetychallengesforArtificialIntelligence(AI)inembeddedauto-
motive systems, including aspects from adversarial attacks, long
lifecyclesofproducts,andlimitedenergyresourcesofautomotive
platformswithinsafety-criticalenvironmentsindiverseusecases.
It provides a set of recommendations for how the security
and safety engineering of machine learning can address these
challenges.Italsoprovidesanoverviewofcontemporarysecurity
andfunctionalsafetyengineeringpractices,encompassingup-to-
date legislative and technical prerequisites.
```

**Future Work:**
```
limitation leading to an incomplete or wrong
tion [11]. Table I summarizes the above attacks [12], which perception of the environment [57]. Safety issues can also
can be cataloged according to the compromise of the security refer to a situation where a system, particularly one involving
attributes. Specifically, training data is a critical component deep learning or complex algorithms, exhibits behavior that
in the development of neural networks. Its key aspects of is difficult to understand or predict, which makes it hard to
the quality of training data are: representativeness, accuracy, argue and assess system safety.
```

---

