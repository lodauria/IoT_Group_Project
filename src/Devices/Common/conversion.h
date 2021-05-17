#ifndef IOTGROUPPROJECT_CONVERSION_H
#define IOTGROUPPROJECT_CONVERSION_H

//Time
#define US2MS(x) (x/1000)
#define MS2S(x)  (x/1000)
#define US2S(x)  (MS2S(US2MS(x)))
#define MS2US(x) (x*1000)
#define HZ2MS(x) (1000/x)

//Distance
#define MM2M(x) (x/1000)


#endif //IOTGROUPPROJECT_CONVERSION_H
