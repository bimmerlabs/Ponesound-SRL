# Ponesound-SRL
C++ wrapper for using the Ponesound library with SRL.

Supports CDDA, PCM, and ADX playback.

PCM and ADX Streaming will come in the future.

## Use
Copy the `Sample` and the `modules_extra` folders to your SRL folder.

Also requires the SMPC module, download that here:
* https://github.com/bimmerlabs/smpc-SRL
  
To include a module in your project, add this line to your project’s Makefile:
```
MODULES_EXTRA = ponesound smpc
```
Include it in your code:
```
#include <ponesound.hpp>
using namespace SRL::Ponesound;
```

## Credits
Original Ponesound driver by Ponut64
* https://github.com/ponut64/SCSP_poneSound

Thanks to robertoduarte for the starting point & ReyeMe for the advice.



