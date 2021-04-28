/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 32;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usrbh		    = 22;
static const char buttonbar[]       = " Menu";
static const char *fonts[]          = { "monogramextended:style=medium:size=16", "Font Awesome 5 Free Solid:size=8" };
static const char dmenufont[]       = "monogramextended:style=medium:size=16";
static const char col_gray1[]       = "#37343d";
static const char col_gray2[]       = "#37343d";
static const char col_gray3[]       = "#ffffff";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#603ea3";
static const char *colors[][3]      = {
/*               fg         bg         border   */
[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 6,       0,           -1 },
	{ "Firefox", "Toolkit", NULL, 1 << 6, 1, -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     		arrange function */
	{ " Tiled",		tile },    /* first entry is default */
	{ " Floating",		NULL },    /* no layout function means floating behavior */
	{ " Maximized",	monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-h", "22", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *fmgrcmd[]  = { "pcmanfm", NULL };
static const char *mutecmd[]  = { "amixer", "-q", "set", "Master", "toggle", NULL };
static const char *volupcmd[] = { "amixer", "-q", "set", "Master", "2%+", "unmute", NULL };
static const char *voldncmd[] = { "amixer", "-q", "set", "Master", "2%-", "unmute", NULL };
static const char *brupcmd[]  = { "xbacklight", "-inc", "10", NULL };
static const char *brdncmd[]  = { "xbacklight", "-dec", "10", NULL };
static const char *touchcmd[] = { "touchpad-toggle", NULL };
static const char *scrlock[]  = { "slock", NULL };
static const char *scrotcmd[] = { "scrot", "-t", "25", NULL };
static const char *scrotfcs[] = { "scrot", "--focused", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ Mod1Mask|ShiftMask,           XK_t,      spawn,          {.v = termcmd } },
	{ MODKEY,			XK_e,      spawn,          {.v = fmgrcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ Mod1Mask,                     XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY|ShiftMask,		XK_l,      spawn,          {.v = scrlock } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ ControlMask|ShiftMask,        XK_Escape, quit,           {0} },

	{ 0,	XF86XK_AudioMute,		spawn,	{.v = mutecmd } },
	{ 0,	XF86XK_AudioLowerVolume,	spawn,	{.v = voldncmd } },
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,	{.v = volupcmd } },
	{ 0,	XF86XK_MonBrightnessUp,		spawn,	{.v = brupcmd } },
	{ 0,	XF86XK_MonBrightnessDown,	spawn,	{.v = brdncmd } },
	{ 0,	XF86XK_TouchpadToggle,		spawn,	{.v = touchcmd } },

	{ 0,				XK_Print,  spawn,          {.v = scrotcmd } },
	{ ShiftMask,			XK_Print,  spawn,          {.v = scrotfcs } },
	{ ControlMask,			XK_Print,  spawn,          SHCMD("sleep 1s; scrot --select") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkButton,            0,              Button1,        spawn,          {.v = dmenucmd } },
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

