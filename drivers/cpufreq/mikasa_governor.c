
/*
 * Mikasa Governor - Extreme Performance Governor for Android (UMS9230 Final)
 * 
 * C90 compliant version compatible with UMS9230 kernel API
 * Designed for maximum performance with aggressive CPU scaling
 */

#include <linux/cpufreq.h>
#include <linux/cpumask.h>
#include <linux/sched.h>
#include <linux/tick.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/cpu.h>
#include <linux/pm_qos.h>
#include <linux/delay.h>
#include <linux/jiffies.h>

/* Governor Tunable Parameters */
#define MIKASA_SAMPLING_RATE		20000	/* 20ms - very aggressive */
#define MIKASA_UP_THRESHOLD		45	/* 45% load to scale up */
#define MIKASA_DOWN_THRESHOLD		25	/* 25% load to scale down */
#define MIKASA_SAMPLING_DOWN_FACTOR	1	/* No down sampling delay */
#define MIKASA_MIN_ONLINE_CPUS		2	/* Minimum online CPUs */
#define MIKASA_BOOST_DURATION		2000	/* 2 seconds boost */
#define MIKASA_TOUCH_BOOST_DURATION	500	/* 500ms touch boost */

/* Governor-specific data */
struct mikasa_cpu_dbs_info_s {
    u64 prev_cpu_idle;
    u64 prev_cpu_wall;
    struct cpufreq_policy *policy;
    struct delayed_work work;
    struct mutex timer_mutex;
    int boost_pulse;
    unsigned long last_boost_time;
    bool touch_boost_active;
    struct timer_list touch_boost_timer;
};

static DEFINE_PER_CPU(struct mikasa_cpu_dbs_info_s, mikasa_cpu_dbs_info);
static DEFINE_MUTEX(dbs_mutex);
static struct workqueue_struct *mikasa_wq;

/* Governor tunables */
static unsigned int dbs_sampling_rate = MIKASA_SAMPLING_RATE;
static unsigned int up_threshold = MIKASA_UP_THRESHOLD;
static unsigned int down_threshold = MIKASA_DOWN_THRESHOLD;
static unsigned int sampling_down_factor __maybe_unused = MIKASA_SAMPLING_DOWN_FACTOR;
static unsigned int min_online_cpus __maybe_unused = MIKASA_MIN_ONLINE_CPUS;
static unsigned int boost_duration = MIKASA_BOOST_DURATION;
static unsigned int touch_boost_duration __maybe_unused = MIKASA_TOUCH_BOOST_DURATION;

/* Performance statistics */
static struct {
    unsigned int total_boosts;
    unsigned int touch_boosts;
    u64 total_boost_time;
} mikasa_stats;

/* Helper functions */
static inline void mikasa_boost_pulse(struct mikasa_cpu_dbs_info_s *dbs_info)
{
    dbs_info->boost_pulse = 1;
    dbs_info->last_boost_time = jiffies;
    mikasa_stats.total_boosts++;
}

/* Touch boost timer callback */
static void mikasa_touch_boost_timer_callback(struct timer_list *t)
{
    struct mikasa_cpu_dbs_info_s *dbs_info = from_timer(dbs_info, t, touch_boost_timer);
    dbs_info->touch_boost_active = false;
}

/* Main frequency scaling logic */
static void mikasa_check_cpu(struct cpufreq_policy *policy)
{
    struct mikasa_cpu_dbs_info_s *dbs_info = &per_cpu(mikasa_cpu_dbs_info, policy->cpu);
    unsigned int max_load = 0;
    unsigned int avg_load = 0;
    unsigned int load;
    unsigned int freq_target;
    unsigned int freq_next;
    unsigned int i;
    u64 idle_time, wall_time;

    mutex_lock(&dbs_info->timer_mutex);

    /* Get CPU usage statistics */
    idle_time = get_cpu_idle_time(policy->cpu, &wall_time, 0);
    
    if (wall_time <= dbs_info->prev_cpu_wall || 
        idle_time <= dbs_info->prev_cpu_idle) {
        pr_debug("Mikasa: CPU time went backwards\
");
        goto out;
    }

    /* Calculate current load */
    wall_time = wall_time - dbs_info->prev_cpu_wall;
    idle_time = idle_time - dbs_info->prev_cpu_idle;

    if (wall_time == 0) {
        load = 0;
    } else {
        load = 100 * (wall_time - idle_time) / wall_time;
    }

    dbs_info->prev_cpu_idle = get_cpu_idle_time(policy->cpu, &wall_time, 0);
    dbs_info->prev_cpu_wall = wall_time;

    max_load = load;
    avg_load = load;

    /* Check load on all online CPUs for better decision making */
    for_each_online_cpu(i) {
        if (i != policy->cpu) {
            u64 cpu_idle, cpu_wall;
            unsigned int cpu_load;
            
            cpu_idle = get_cpu_idle_time(i, &cpu_wall, 0);
            if (cpu_wall > 0) {
                cpu_load = 100 * (cpu_wall - cpu_idle) / cpu_wall;
                max_load = max(max_load, cpu_load);
                avg_load += cpu_load;
            }
        }
    }
    avg_load /= num_online_cpus();

    pr_debug("Mikasa: CPU%d - load: %d%%, max: %d%%, avg: %d%%\
", 
            policy->cpu, load, max_load, avg_load);

    /* Determine target frequency */
    freq_target = policy->cur;

    /* Check for boost conditions */
    if (dbs_info->boost_pulse || 
        (dbs_info->touch_boost_active) ||
        (time_before(jiffies, dbs_info->last_boost_time + msecs_to_jiffies(boost_duration)))) {
        /* Boost to maximum frequency */
        freq_target = policy->max;
        dbs_info->boost_pulse = 0;
        pr_debug("Mikasa: CPU%d boosted to max frequency\
", policy->cpu);
    } else {
        /* Normal scaling logic */
        if (max_load > up_threshold) {
            /* Scale up aggressively */
            if (max_load > 85) {
                freq_target = policy->max;  /* Immediate max for very high load */
            } else if (max_load > 70) {
                freq_target = policy->max * 90 / 100;  /* 90% of max */
            } else {
                freq_target = policy->max * 75 / 100;  /* 75% of max */
            }
        } else if (avg_load < down_threshold) {
            /* Scale down conservatively to maintain performance */
            if (avg_load < 15) {
                freq_target = policy->min;  /* Minimum for very low load */
            } else {
                freq_target = policy->min + (policy->max - policy->min) * 40 / 100;
            }
        }
    }

    /* Apply the new frequency if different */
    if (freq_target != policy->cur) {
        unsigned int freq, diff, best_diff;
        
        /* Find nearest frequency in table */
        freq_next = policy->min;
        best_diff = abs(policy->max - freq_target);
        
        if (policy->freq_table) {
            for (i = 0; policy->freq_table[i].frequency != CPUFREQ_TABLE_END; i++) {
                freq = policy->freq_table[i].frequency;
                diff = abs(freq - freq_target);
                
                if (diff < best_diff) {
                    best_diff = diff;
                    freq_next = freq;
                }
            }
        } else {
            /* No frequency table, use direct targeting */
            freq_next = clamp_val(freq_target, policy->min, policy->max);
        }
        
        if (freq_next != policy->cur) {
            __cpufreq_driver_target(policy, freq_next, CPUFREQ_RELATION_H);
            pr_debug("Mikasa: CPU%d frequency changed from %d to %d MHz (load: %d%%)\
",
                    policy->cpu, policy->cur/1000, freq_next/1000, max_load);
        }
    }

out:
    mutex_unlock(&dbs_info->timer_mutex);
}

/* Workqueue function */
static void mikasa_dbs_timer(struct work_struct *work)
{
    struct mikasa_cpu_dbs_info_s *dbs_info = 
        container_of(work, struct mikasa_cpu_dbs_info_s, work.work);
    struct cpufreq_policy *policy = dbs_info->policy;

    mikasa_check_cpu(policy);

    /* Schedule next check */
    queue_delayed_work_on(dbs_info->policy->cpu, mikasa_wq, &dbs_info->work,
                         usecs_to_jiffies(dbs_sampling_rate));
}

/* Governor initialization and cleanup */
static int mikasa_init_cpu_dbs_info(struct cpufreq_policy *policy)
{
    struct mikasa_cpu_dbs_info_s *dbs_info = &per_cpu(mikasa_cpu_dbs_info, policy->cpu);
    
    dbs_info->policy = policy;
    dbs_info->boost_pulse = 0;
    dbs_info->last_boost_time = jiffies;
    dbs_info->touch_boost_active = false;
    
    mutex_init(&dbs_info->timer_mutex);
    INIT_DELAYED_WORK(&dbs_info->work, mikasa_dbs_timer);
    
    timer_setup(&dbs_info->touch_boost_timer, mikasa_touch_boost_timer_callback, 0);
    
    /* Get initial CPU times */
    dbs_info->prev_cpu_idle = get_cpu_idle_time(policy->cpu, &dbs_info->prev_cpu_wall, 0);
    
    return 0;
}

static int mikasa_start(struct cpufreq_policy *policy)
{
    int ret;

    mutex_lock(&dbs_mutex);
    ret = mikasa_init_cpu_dbs_info(policy);
    mutex_unlock(&dbs_mutex);

    if (ret)
        return ret;

    /* Start the timer work */
    queue_delayed_work_on(policy->cpu, mikasa_wq, 
                         &per_cpu(mikasa_cpu_dbs_info, policy->cpu).work,
                         usecs_to_jiffies(dbs_sampling_rate));

    pr_info("Mikasa governor started on CPU%d\
", policy->cpu);
    return 0;
}

static void mikasa_stop(struct cpufreq_policy *policy)
{
    struct mikasa_cpu_dbs_info_s *dbs_info = &per_cpu(mikasa_cpu_dbs_info, policy->cpu);

    cancel_delayed_work_sync(&dbs_info->work);
    del_timer_sync(&dbs_info->touch_boost_timer);

    pr_info("Mikasa governor stopped on CPU%d\
", policy->cpu);
}

/* Governor registration structure */
static struct cpufreq_governor mikasa_governor = {
    .name = "mikasa",
    .owner = THIS_MODULE,
    .init = mikasa_init_cpu_dbs_info,
    .start = mikasa_start,
    .stop = mikasa_stop,
};

/* Module initialization */
static int __init mikasa_governor_init(void)
{
    int ret;

    /* Create workqueue */
    mikasa_wq = alloc_workqueue("mikasa_wq", WQ_HIGHPRI | WQ_UNBOUND, 0);
    if (!mikasa_wq) {
        pr_err("Mikasa: Failed to create workqueue\
");
        return -ENOMEM;
    }

    /* Register governor */
    ret = cpufreq_register_governor(&mikasa_governor);
    if (ret) {
        pr_err("Mikasa: Failed to register governor: %d\
", ret);
        destroy_workqueue(mikasa_wq);
        return ret;
    }

    pr_info("Mikasa governor loaded successfully\
");
    pr_info("Mikasa: Sampling rate: %u us, Up threshold: %u%%\
", 
            dbs_sampling_rate, up_threshold);

    return 0;
}

/* Module cleanup */
static void __exit mikasa_governor_exit(void)
{
    cpufreq_unregister_governor(&mikasa_governor);
    destroy_workqueue(mikasa_wq);
    
    pr_info("Mikasa governor unloaded\
");
}

MODULE_AUTHOR("Android Performance Team");
MODULE_DESCRIPTION("Mikasa Governor - Extreme Performance Governor for Android (UMS9230)");
MODULE_LICENSE("GPL");

module_init(mikasa_governor_init);
module_exit(mikasa_governor_exit);
