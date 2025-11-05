/* UMS9230 Overclock Voltage Configuration */
/* WARNING: Incorrect voltage can permanently damage your device! */

#ifndef __UMS9230_OC_VOLTAGE_H__
#define __UMS9230_OC_VOLTAGE_H__

/* Voltage levels in microvolts (uV) */
static const unsigned long ums9230_oc_voltage_table[] = {
    /* Freq(KHz)  Voltage(uV) */
    614400,     650000,
    768000,     700000,
    1024000,    750000,
    1228800,    800000,
    1400000,    850000,
    1536000,    900000,
    1690000,    925000,
    1820000,    950000,
    2002000,    1000000,  /* Stock max */
    2150000,    1050000,  /* OC +50mV */
    2300000,    1100000,  /* OC +100mV */
    2450000,    1150000,  /* OC +150mV */
};

#endif /* __UMS9230_OC_VOLTAGE_H__ */
