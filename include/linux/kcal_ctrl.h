#ifndef _LINUX_KCAL_CTRL_H
#define _LINUX_KCAL_CTRL_H

struct kcal_lut_data {
    int red;
    int green;
    int blue;
    int minimum;
    int enable;
    int invert;
};

extern struct device_attribute dev_attr_kcal;
extern struct device_attribute dev_attr_kcal_min;
extern struct device_attribute dev_attr_kcal_enable;
extern struct device_attribute dev_attr_kcal_invert;

extern void kcal_update_display(void);
extern struct kcal_lut_data *get_kcal_data(void);

#endif /* _LINUX_KCAL_CTRL_H */
