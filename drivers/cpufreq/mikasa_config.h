/*
 * Mikasa Governor Configuration Header
 * 
 * This file contains compile-time configuration options
 * for the Mikasa performance governor
 */

#ifndef _MIKASA_CONFIG_H
#define _MIKASA_CONFIG_H

/* Governor identification */
#define MIKASA_GOVERNOR_NAME		"mikasa"
#define MIKASA_VERSION			"1.0.0"
#define MIKASA_AUTHOR			"anothernop"

/* Debug options */
#define MIKASA_DEBUG			0	/* Enable debug messages */
#define MIKASA_VERBOSE			1	/* Enable verbose logging */
#define MIKASA_STATS			1	/* Enable statistics collection */

/* Performance tuning constants */
#define MIKASA_SAMPLING_RATE_MIN	1000	/* Minimum sampling rate (μs) */
#define MIKASA_SAMPLING_RATE_MAX	1000000	/* Maximum sampling rate (μs) */
#define MIKASA_SAMPLING_RATE_DEFAULT	20000	/* Default sampling rate (μs) */

#define MIKASA_UP_THRESHOLD_MIN		1	/* Minimum up threshold (%) */
#define MIKASA_UP_THRESHOLD_MAX		100	/* Maximum up threshold (%) */
#define MIKASA_UP_THRESHOLD_DEFAULT	45	/* Default up threshold (%) */

#define MIKASA_DOWN_THRESHOLD_MIN	1	/* Minimum down threshold (%) */
#define MIKASA_DOWN_THRESHOLD_MAX	100	/* Maximum down threshold (%) */
#define MIKASA_DOWN_THRESHOLD_DEFAULT	25	/* Default down threshold (%) */

/* Hotplug configuration */
#define MIKASA_HOTPLUG_ENABLE		1	/* Enable CPU hotplug */
#define MIKASA_MIN_ONLINE_CPUS_MIN	1	/* Minimum online CPUs */
#define MIKASA_MIN_ONLINE_CPUS_MAX	8	/* Maximum online CPUs */
#define MIKASA_MIN_ONLINE_CPUS_DEFAULT	2	/* Default minimum online CPUs */

#define MIKASA_HOTPLUG_UP_THRESHOLD	65	/* Load threshold to hotplug in (%) */
#define MIKASA_HOTPLUG_DOWN_THRESHOLD	20	/* Load threshold to hotplug out (%) */
#define MIKASA_HOTPLUG_DELAY_MS		100	/* Hotplug delay (ms) */

/* Boost configuration */
#define MIKASA_BOOST_ENABLE		1	/* Enable boost feature */
#define MIKASA_BOOST_DURATION_DEFAULT	2000	/* Default boost duration (ms) */
#define MIKASA_BOOST_DURATION_MIN	100	/* Minimum boost duration (ms) */
#define MIKASA_BOOST_DURATION_MAX	10000	/* Maximum boost duration (ms) */

/* Touch boost configuration */
#define MIKASA_TOUCH_BOOST_ENABLE	1	/* Enable touch boost */
#define MIKASA_TOUCH_BOOST_DURATION_DEFAULT 500	/* Default touch boost (ms) */
#define MIKASA_TOUCH_BOOST_DURATION_MIN	50	/* Minimum touch boost (ms) */
#define MIKASA_TOUCH_BOOST_DURATION_MAX	2000	/* Maximum touch boost (ms) */

/* Input boost configuration */
#define MIKASA_INPUT_BOOST_ENABLE	1	/* Enable input boost */
#define MIKASA_INPUT_BOOST_FREQ_DEFAULT -1	/* Default input boost freq (-1 = max) */

/* Frequency scaling thresholds */
#define MIKASA_VERY_HIGH_LOAD_THRESHOLD	85	/* Very high load threshold (%) */
#define MIKASA_HIGH_LOAD_THRESHOLD	70	/* High load threshold (%) */
#define MIKASA_MEDIUM_LOAD_THRESHOLD	45	/* Medium load threshold (%) */
#define MIKASA_LOW_LOAD_THRESHOLD	15	/* Low load threshold (%) */

#define MIKASA_VERY_HIGH_FREQ_MULTIPLIER	100	/* Very high load freq (%) */
#define MIKASA_HIGH_FREQ_MULTIPLIER		90	/* High load freq (%) */
#define MIKASA_MEDIUM_FREQ_MULTIPLIER		75	/* Medium load freq (%) */
#define MIKASA_LOW_FREQ_MULTIPLIER		40	/* Low load freq (%) */

/* Workqueue configuration */
#define MIKASA_WQ_NAME			"mikasa_wq"
#define MIKASA_WQ_FLAGS			(WQ_HIGHPRI | WQ_UNBOUND)
#define MIKASA_WQ_MAX_ACTIVE		0	/* No limit on active work */

/* Timer configuration */
#define MIKASA_TIMER_NAME		"mikasa_timer"
#define MIKASA_TIMER_FLAGS		(TIMER_DEFERRABLE)

/* System integration */
#define MIKASA_INPUT_HANDLER_NAME	"mikasa"
#define MIKASA_SYSFS_GROUP_NAME		"mikasa"

/* Power management integration */
#define MIKASA_PM_SUSPEND_DISABLE	0	/* Keep governor active during suspend */
#define MIKASA_PM_RESUME_RESET		1	/* Reset state on resume */

/* CPU topology awareness */
#define MIKASA_TOPOLOGY_AWARE		1	/* Enable CPU topology awareness */
#define MIKASA_CLUSTER_AWARE		1	/* Enable cluster awareness */

/* Thermal management */
#define MIKASA_THERMAL_AWARE		1	/* Enable thermal awareness */
#define MIKASA_THERMAL_THROTTLE_TEMP	80000	/* Throttle temperature (mC) */
#define MIKASA_THERMAL_RECOVERY_TEMP	75000	/* Recovery temperature (mC) */

/* Real-time optimizations */
#define MIKASA_RT_AWARE			1	/* Enable real-time task awareness */
#define MIKASA_RT_BOOST_THRESHOLD	30	/* RT task boost threshold (%) */

/* Gaming detection */
#define MIKASA_GAMING_DETECTION		1	/* Enable gaming detection */
#define MIKASA_GAMING_BOOST_DURATION	3000	/* Gaming boost duration (ms) */

/* Performance profiling */
#define MIKASA_PROFILING_ENABLE		0	/* Enable profiling (debug only) */
#define MIKASA_PROFILING_BUFFER_SIZE	1024	/* Profiling buffer size */

/* Safety checks */
#define MIKASA_SAFETY_CHECKS		1	/* Enable safety checks */
#define MIKASA_MAX_FREQ_CHANGE_RATE	10	/* Max frequency changes per second */

/* Compatibility options */
#define MIKASA_COMPATIBILITY_MODE	0	/* Enable compatibility mode */
#define MIKASA_LEGACY_INTERFACE		0	/* Enable legacy sysfs interface */

/* Feature flags */
#define MIKASA_FEATURE_TOUCH_BOOST	(1 << 0)
#define MIKASA_FEATURE_INPUT_BOOST	(1 << 1)
#define MIKASA_FEATURE_HOTPLUG		(1 << 2)
#define MIKASA_FEATURE_THERMAL		(1 << 3)
#define MIKASA_FEATURE_GAMING_DETECTION	(1 << 4)
#define MIKASA_FEATURE_RT_AWARE		(1 << 5)

#define MIKASA_DEFAULT_FEATURES		(MIKASA_FEATURE_TOUCH_BOOST | \
						 MIKASA_FEATURE_INPUT_BOOST | \
						 MIKASA_FEATURE_HOTPLUG | \
						 MIKASA_FEATURE_THERMAL)

/* Validation macros */
#define MIKASA_VALID_SAMPLING_RATE(x)	((x) >= MIKASA_SAMPLING_RATE_MIN && \
					 (x) <= MIKASA_SAMPLING_RATE_MAX)

#define MIKASA_VALID_THRESHOLD(x)	((x) >= 1 && (x) <= 100)

#define MIKASA_VALID_BOOST_DURATION(x)	((x) >= MIKASA_BOOST_DURATION_MIN && \
					 (x) <= MIKASA_BOOST_DURATION_MAX)

/* Compile-time feature selection */
#ifndef MIKASA_FEATURES
#define MIKASA_FEATURES			MIKASA_DEFAULT_FEATURES
#endif

/* Feature check macros */
#define MIKASA_HAS_TOUCH_BOOST		(MIKASA_FEATURES & MIKASA_FEATURE_TOUCH_BOOST)
#define MIKASA_HAS_INPUT_BOOST		(MIKASA_FEATURES & MIKASA_FEATURE_INPUT_BOOST)
#define MIKASA_HAS_HOTPLUG		(MIKASA_FEATURES & MIKASA_FEATURE_HOTPLUG)
#define MIKASA_HAS_THERMAL		(MIKASA_FEATURES & MIKASA_FEATURE_THERMAL)
#define MIKASA_HAS_GAMING_DETECTION	(MIKASA_FEATURES & MIKASA_FEATURE_GAMING_DETECTION)
#define MIKASA_HAS_RT_AWARE		(MIKASA_FEATURES & MIKASA_FEATURE_RT_AWARE)

/* Debug macros */
#if MIKASA_DEBUG
#define MIKASA_DEBUG_PRINT(fmt, args...) \
	pr_debug("[MIKASA] " fmt, ##args)
#else
#define MIKASA_DEBUG_PRINT(fmt, args...) do {} while(0)
#endif

#if MIKASA_VERBOSE
#define MIKASA_VERBOSE_PRINT(fmt, args...) \
	pr_info("[MIKASA] " fmt, ##args)
#else
#define MIKASA_VERBOSE_PRINT(fmt, args...) do {} while(0)
#endif

/* Error handling macros */
#define MIKASA_ERROR_PRINT(fmt, args...) \
	pr_err("[MIKASA] " fmt, ##args)

#define MIKASA_WARN_PRINT(fmt, args...) \
	pr_warn("[MIKASA] " fmt, ##args)

/* Statistics macros */
#if MIKASA_STATS
#define MIKASA_STATS_INC(counter)		((counter)++)
#define MIKASA_STATS_ADD(counter, value)	((counter) += (value))
#else
#define MIKASA_STATS_INC(counter)		do {} while(0)
#define MIKASA_STATS_ADD(counter, value)	do {} while(0)
#endif

/* Performance optimization macros */
#define MIKASA_LIKELY(x)		__builtin_expect(!!(x), 1)
#define MIKASA_UNLIKELY(x)		__builtin_expect(!!(x), 0)

/* Memory barriers */
#define MIKASA_MEMORY_BARRIER()		smp_mb()

/* Time utilities */
#define MIKASA_TIME_MS(x)		(ktime_to_ms(x))
#define MIKASA_TIME_US(x)		(ktime_to_us(x))
#define MIKASA_TIME_SINCE_MS(x)		(MIKASA_TIME_MS(ktime_sub(ktime_get(), (x))))

#endif /* _MIKASA_CONFIG_H */
