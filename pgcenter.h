/*
 * pgcenter: administrative console for PostgreSQL.
 * (C) 2015 by Alexey V. Lesovsky (lesovsky <at> gmail.com)
 */

#ifndef __PGCENTER_H__
#define __PGCENTER_H__

#define PROGRAM_NAME        "pgcenter"
#define PROGRAM_VERSION     0.1
#define PROGRAM_RELEASE     1
#define PROGRAM_AUTHORS_CONTACTS    "<lesovsky@gmail.com>"

/* sizes and limits */
#define BUFFERSIZE          4096
#define MAX_CONSOLE         8
#define TOTAL_CONTEXTS      3

#define LOADAVG_FILE        "/proc/loadavg"
#define STAT_FILE           "/proc/stat"
#define UPTIME_FILE         "/proc/uptime"
#define PGCENTERRC_FILE     ".pgcenterrc"

#define PGCENTERRC_READ_OK  0
#define PGCENTERRC_READ_ERR 1

/* connectins defaults */
#define DEFAULT_HOST        "/tmp"
#define DEFAULT_PORT        "5432"
#define DEFAULT_USER        "postgres"

#define HZ                  hz
unsigned int hz;

/* enum for query context */
enum context
{
    pg_stat_database,
    pg_stat_replication,
    pg_stat_user_tables
};

#define DEFAULT_QUERY_CONTEXT   pg_stat_database

/* struct for context list used in screen */
struct context_s
{
    enum context context;
    int order_key;
    bool order_desc;
};

/* Struct which define connection options */
struct screen_s
{
    int screen;
    bool conn_used;
    char host[BUFFERSIZE];
    char port[BUFFERSIZE];
    char user[BUFFERSIZE];
    char dbname[BUFFERSIZE];
    char password[BUFFERSIZE];
    char conninfo[BUFFERSIZE];
    bool log_opened;
    FILE *log;
    enum context current_context;
    struct context_s context_list[TOTAL_CONTEXTS];
};

#define SCREEN_SIZE (sizeof(struct screen_s))

/* struct which used for cpu statistic */
struct stats_cpu_struct {
    unsigned long long cpu_user;
    unsigned long long cpu_nice;
    unsigned long long cpu_sys;
    unsigned long long cpu_idle;
    unsigned long long cpu_iowait;
    unsigned long long cpu_steal;
    unsigned long long cpu_hardirq;
    unsigned long long cpu_softirq;
    unsigned long long cpu_guest;
    unsigned long long cpu_guest_nice;
};

#define STATS_CPU_SIZE (sizeof(struct stats_cpu_struct))

/*
 * Macros used to display statistics values.
 * NB: Define SP_VALUE() to normalize to %;
 */
#define SP_VALUE(m,n,p) (((double) ((n) - (m))) / (p) * 100)

/* enum for password purpose */
enum trivalue
{
    TRI_DEFAULT,
    TRI_NO,
    TRI_YES
};

/* struct for column widths */
struct colAttrs {
    char name[40];
    int width;
};

/* PostgreSQL answers */
#define PG_CMD_OK PGRES_COMMAND_OK
#define PG_TUP_OK PGRES_TUPLES_OK

#define PG_STAT_DATABASE_QUERY \
    "SELECT \
        datname, \
        xact_commit as commit, xact_rollback as rollback, \
        blks_read as reads, blks_hit as hits, \
        tup_returned as returned, tup_fetched as fetched, \
        tup_inserted as inserts, tup_updated as updates, tup_deleted as deletes, \
        conflicts, \
        temp_files as tmp_files, temp_bytes as tmp_bytes, \
        blk_read_time as read_t, blk_write_time as write_t \
    FROM pg_stat_database \
    ORDER BY datname"

#define PG_STAT_DATABASE_ORDER_MIN    1
#define PG_STAT_DATABASE_ORDER_MAX    14

#define PG_STAT_REPLICATION_QUERY \
    "SELECT \
        client_addr as client, application_name as name, \
        state, sync_state as mode, \
        pg_xlog_location_diff(sent_location, '0/0') as \"sent b/s\", \
        pg_xlog_location_diff(write_location, '0/0') as \"write b/s\", \
        pg_xlog_location_diff(flush_location, '0/0') as \"flush b/s\", \
        pg_xlog_location_diff(replay_location, '0/0') as \"replay b/s\", \
        pg_xlog_location_diff(sent_location,replay_location) as \"lag b/s\" \
    FROM pg_stat_replication \
    ORDER BY client_addr"

#define PG_STAT_REPLICATION_ORDER_MIN 4
#define PG_STAT_REPLICATION_ORDER_MAX 7

#define PG_STAT_USER_TABLES_QUERY \
    "SELECT \
        schemaname || '.' || relname as relation, \
        seq_scan, seq_tup_read, idx_scan, idx_tup_fetch, \
        n_tup_ins as inserts, n_tup_upd as updates, \
        n_tup_del as deletes, n_tup_hot_upd as hot_updates, \
        n_live_tup as live, n_dead_tup as dead \
    FROM pg_stat_user_tables \
    ORDER BY 1"

#define PG_STAT_USER_TABLES_ORDER_MIN 1
#define PG_STAT_USER_TABLES_ORDER_MAX 10

#endif
