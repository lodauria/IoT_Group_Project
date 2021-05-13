#ifndef TIMER_CONVERSION_H
#define TIMER_CONVERSION_H

#define PUMP_LS     PUMP_CAPACITY

#define HZ2US(hz)   (1000000 / (hz))
#define US2S(us)    ((us) * 1e-6f)
#define US2MS(us)   ((us) * 1e-3f)
#define MS2US(ms)   ((ms) * 1000)
#define MS2S(ms)    ((ms) * 1e-3f)
#define HZ2S(hz)    US2S(HZ2US(hz))
#define S2MS(s)     (s*1000)
#define LITER2S(l)  (l*PUMP_LS)
#define L2ML(l)     (l*1000)
#define ML2L(ml)    (ml/1000)
#define ML2S(ml)    (LITER2S(ML2L(ml)))
#define ML2MS(ml)   (S2MS(LITER2S(ML2L(ml))))

#endif //TIMER_CONVERSION_H
