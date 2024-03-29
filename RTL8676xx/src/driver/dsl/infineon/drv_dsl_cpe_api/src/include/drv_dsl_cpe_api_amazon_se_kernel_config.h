/*
   This file has been manually derived by
   'src/include/drv_dsl_cpe_api_config.h' which is generated by configure.
   It is intended to be included within kernel build of DSL CPE API.
*/

/* Channels per line to support */
#define DSL_CHANNELS_PER_LINE 1

/* Include DSL DELT static data support */
/* #undef DSL_CPE_STATIC_DELT_DATA */

/* Disabled debug prints */
/* #undef DSL_DEBUG_DISABLE */

/* Include ADSL Data LED support */
#define INCLUDE_ADSL_LED

/* Include ADSL MIB support */
#define INCLUDE_DSL_ADSL_MIB

/* Include DSL CEOC support */
#define INCLUDE_DSL_CEOC

/* Add AMAZON_SE support */
#define INCLUDE_DSL_CPE_API_AMAZON_SE

/* Add DANUBE support */
/* #undef INCLUDE_DSL_CPE_API_DANUBE

/* Add VINAX support */
/* #undef INCLUDE_DSL_CPE_API_VINAX */

/* Include DSL PM channel counters support */
#define INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS

/* Include DSL PM channel thresholds support */
#define INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS

/* Include DSL PM datapath counters support */
#define INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS

/* Include DSL PM datapath thresholds support */
#define INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS

/* Include DSL PM history support */
#define INCLUDE_DSL_CPE_PM_HISTORY

/* Include DSL PM line counters support */
#define INCLUDE_DSL_CPE_PM_LINE_COUNTERS

/* Include DSL PM line thresholds support */
#define INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS

/* Include DSL PM optional parameters support */
#define INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS 

/* Include DSL PM showtime counters support */
#define INCLUDE_DSL_CPE_PM_SHOWTIME_COUNTERS

/* Include DSL PM total counters support */
#define INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS

/* Include ADSL showtime event logging support */
#define INCLUDE_DSL_CPE_TRACE_BUFFER

/* Include DSL PM support */
#define INCLUDE_DSL_PM

/* Package definitions has been renamed to avoid redefinition within kernel */
/* build as follows: */
/*    ORIGINAL_DEFINE  ->  DSL_CPE_API_ORIGINAL_DEFINE */

/* Name of package */
#define DSL_CPE_API_PACKAGE "drv_dsl_cpe_api"

/* Define to the address where bug reports for this package should be sent. */
#define DSL_CPE_API_PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define DSL_CPE_API_PACKAGE_NAME "DSL CPE API driver"

/* Define to the full name and version of this package. */
#define DSL_CPE_API_PACKAGE_STRING "DSL CPE API driver 3.3.2"

/* Define to the one symbol short name of this package. */
#define DSL_CPE_API_PACKAGE_TARNAME "drv_dsl_cpe_api"

/* Define to the version of this package. */
#define DSL_CPE_API_PACKAGE_VERSION "3.3.2"

/* Version number of package */
#define DSL_CPE_API_VERSION "3.3.2"
