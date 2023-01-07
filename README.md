# ATPG FAN
## v2023   &emsp; LaDSII
## CONTENTS

> **I.    Introduction**<br />
> **II.   Directory Structure**<br />
> **III.  Usage**<br />
> **IV.   Revision History**<br />
> **V.    Authors**<br />


## I. INTRODUCTION

**ATPG and fault simulation tool based on the FAN algorithm.** <br />
**Has Fault Collapsing, Staitc Test Compression, Dynamic Test Compression and Multiple Fault Orderings.** <br />

## II. DIRECTORY STRUCTURE

> **Main files :** ```pkg/core/```<br />
> **FAN's commands files :** ```pkg/fan/```<br />
> **Interface files :** ```pkg/interface/```<br />
> **Commands, options and arguments manager files :** ```pkg/common/```<br />
> **Circuit files :** ```netlist/```<br />
> **Pattern files :** ```pat/```<br />
> **Script files :** ```script/```<br />


## III. USAGE

> **Compile:**<br />
> &emsp;	```make clean``` <br />
> &emsp;	```make``` <br />
<br />

> **Run ATPG:**<br />
> Command: &emsp;	```./bin/opt/fan -f script/atpg.script``` <br />
<br />

> **Run fault simulation:**<br />
> Command: &emsp;	```./bin/opt/fan -f script/fsim.script``` <br />


## IV. REVISION HISTORY

**v2013 - Initial release.** <br />
**v2014 - Code Modifications.** <br />
**v2023 - Code refactor for open source project.** <br />


## V. AUTHORS

**Original Authors -** Lin Hio-Ting, Hsu Po-Ya, Liao Kuan-Yu  <br />
**v2013 -** Han Cheng-You, Chen Ching-Yu, Chiang Kuan-Ying, Wang Ying-Hsu, Chen Po-Hao, Lin Kuo-Yu, Pan Chun-Han, Li Chia-An, Tsai Chia-Ling, Hsu Ling-Yun <br />
**v2014 -** He Yu-Hao, Cai Zong-Yan, How Bo-Fan, Li Yu-Qing, Huang Guan-Yan, Xiao Zhi-Long, Lin Kun-Wei, Lin Shi-Yao, Chen You-Wei, Li Guan-De <br />
**v2023 -** Wang Wei-Shen, Chang Hsin-Tzu, Pan Yu-Hung, Liang Zhe-Jia <br />


***Copyright (c) 2013 LaDS. All Rights Reserved.***