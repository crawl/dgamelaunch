/* IMPORTANT defines */

#ifndef __DGAMELAUNCH_H
#define __DGAMELAUNCH_H

#include "config.h"
#include <sys/param.h>
#include <sys/types.h>
#include <time.h>

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif


/* max # of different games playable from within this dgl */
#define DIFF_GAMES 3

struct dg_user
{
  char *username;
  char *email;
  char *env;
  char *password;
  int flags;
};

struct dg_banner
{
  char **lines;
  unsigned int len;
};

struct dg_game
{
  char *ttyrec_fn;
  char *name;
  char *date;
  char *time;
  time_t idle_time;
  int ws_row, ws_col; /* Window size */
};

struct dg_config
{
  char* game_path;
  char* game_name;
  char* lockfile;
  char* passwd;
  char* rcfile;
  char* spool;
  char* savefilefmt;
  char* inprogressdir;
    int num_args; /* # of bin_args */
    char **bin_args; /* args for game binary */
    char *rc_fmt;
};

struct dg_globalconfig
{
    char* chroot;
    char* dglroot;
    char* banner;
    unsigned long max;
    int max_newnick_len; /* maximum length of new registered names. must be less than 20 chars. */
    char* shed_user;
    char* shed_group;
    uid_t shed_uid;
    gid_t shed_gid;
};

/* Global variables */
extern char* config; /* file path */
extern struct dg_config **myconfig;
extern char *chosen_name;
extern int loggedin;
extern int silent;
extern int set_max;

extern struct dg_globalconfig globalconfig;

extern int num_games;

/* dgamelaunch.c */
extern void create_config(void);
extern void ttyrec_getmaster(void);
extern char *gen_ttyrec_filename(void);
extern char *gen_inprogress_lock(int game, pid_t pid, char *ttyrec_filename);
extern void catch_sighup(int signum);
extern void loadbanner(int game, struct dg_banner *ban);
extern void drawbanner(unsigned int start_line, unsigned int howmany);
extern char *dgl_format_str(int game, struct dg_user *me, char *str);
extern struct dg_game **populate_games(int game, int *l);
extern void inprogressmenu(int gameid);
extern void change_email(void);
extern int changepw(int dowrite);
extern void domailuser(char *username);
extern void drawmenu(void);
extern void freefile(void);
extern void initcurses(void);
extern void loginprompt(int from_ttyplay);
extern void newuser(void);
extern void autologin(char *user, char *pass);
extern int passwordgood(char *cpw);
extern int readfile(int nolock);
extern int userexist(char *cname, int isnew);
extern void write_canned_rcfile(int game, char *target);
extern void editoptions(int game);
extern void writefile(int requirenew);
extern void graceful_exit(int status);
extern int purge_stale_locks(int game);
extern int menuloop(void);
extern void ttyrec_getpty(void);
#if !defined(BSD) && !defined(__linux__)
extern int mysetenv (const char* name, const char* value, int overwrite);
#else
# define mysetenv setenv
#endif
#ifndef HAVE_SETPROCTITLE
void compat_init_setproctitle(int argc, char *argv[]);
void setproctitle(const char *fmt, ...);
#endif

/* strlcpy.c */
extern size_t strlcpy (char *dst, const char *src, size_t siz);
extern size_t strlcat (char *dst, const char *src, size_t siz);

/* mygetnstr.c */
extern int mygetnstr(char *buf, int maxlen, int doecho);

#endif
