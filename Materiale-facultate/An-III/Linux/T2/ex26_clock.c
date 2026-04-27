/*
 * ex26_clock.c  –  Ceas cu alarme (Xlib)
 *
 * Compilare: gcc -o ex26 ex26_clock.c -lX11
 * Rulare:    ./ex26
 *
 * Navigare:  Tab / click = schimba campul activ
 *            Enter       = adauga eveniment
 *            Esc         = iese
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>

/* dimensiuni fereastra principala */
#define WIN_W    520
#define WIN_H    480

/*  geometrie cifra 7-segmente */
#define DIG_W     28    /* latimea unei cifre       */
#define DIG_H     52    /* inaltimea unei cifre     */
#define SEG_T      5    /* grosimea unui segment    */
#define DIG_GAP    6    /* spatiu intre cifre       */
#define COLON_W   14    /* latimea separatorului :  */

/* pozitii layout (derivate, de sus in jos)  */
#define CLOCK_Y   20
#define LIST_Y    (CLOCK_Y + DIG_H + 28)
#define LIST_H    148
#define LIST_ROWS   7
#define INPUT_Y   (LIST_Y + LIST_H + 22)
#define BTN_Y     (INPUT_Y + 72)

/* fereastra alerta  */
#define ALERT_W      380
#define ALERT_H      150
#define ALERT_BTN_X  (ALERT_W / 2 - 40)
#define ALERT_BTN_Y  (ALERT_H - 36)

/* 
 * Lista de evenimente
*/
#define MAX_MSG     256
#define MAX_EVENTS   64

typedef struct {
    time_t time;
    char   msg[MAX_MSG];
} Event;

static Event events[MAX_EVENTS];
static int   num_events = 0;

static void event_add(time_t t, const char *msg)
{
    if (num_events >= MAX_EVENTS) return;
    events[num_events].time = t;
    strncpy(events[num_events].msg, msg, MAX_MSG - 1);
    int i = num_events++;
    /* insertion sort: mentine lista sortata dupa timp */
    for (; i > 0 && events[i].time < events[i-1].time; i--) {
        Event tmp = events[i]; events[i] = events[i-1]; events[i-1] = tmp;
    }
}

static void event_remove(int i)
{
    memmove(&events[i], &events[i+1], (num_events - i - 1) * sizeof(Event));
    num_events--;
}

/*
 * Globale X11
 */
static Display     *dpy;
static Window       win;
static Window       alert_win = None;
static GC           gc;
static XFontStruct *font;
static Atom         wm_delete;

/* culori */
static unsigned long col_bg, col_fg, col_on, col_off, col_red, col_input, col_btn;

static unsigned long alloc_color(const char *name)
{
    XColor screen, exact;
    XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
                     name, &screen, &exact);
    return screen.pixel;
}

/* stare campuri input */
static char field_time[9]      = "";   /* HH:MM:SS */
static char field_msg[MAX_MSG] = "";
static int  focus              = 0;    /* 0 = camp ora, 1 = camp mesaj */
static char alert_msg[MAX_MSG] = "";

/* 
 * Desen cifre 7-segmente
 *
 *   _         pozitia segmentelor:
 *  |_|          0 = sus (orizontal)
 *  |_|          1 = dreapta sus   2 = dreapta jos
 *               3 = jos (orizontal)
 *               4 = stanga jos    5 = stanga sus
 *               6 = mijloc (orizontal)
 *
 * Fiecare cifra e un byte: bitul n spune daca segmentul n e aprins.
*/
static const unsigned char DIGIT_SEGMENTS[10] = {
    0x3F, /* 0 → 0111111: toate mai putin mijlocul */
    0x06, /* 1 → 0000110: dreapta sus + dreapta jos */
    0x5B, /* 2 → 1011011 */
    0x4F, /* 3 → 1001111 */
    0x66, /* 4 → 1100110 */
    0x6D, /* 5 → 1101101 */
    0x7D, /* 6 → 1111101 */
    0x07, /* 7 → 0000111: sus + dreapta */
    0x7F, /* 8 → 1111111: toate */
    0x6F, /* 9 → 1101111 */
};

static void draw_segment(int x, int y, int seg, int on)
{
    XSetForeground(dpy, gc, on ? col_on : col_off);
    int half = DIG_H / 2;
    switch (seg) {
    case 0: XFillRectangle(dpy,win,gc, x+SEG_T,       y,            DIG_W-2*SEG_T, SEG_T        ); break;
    case 1: XFillRectangle(dpy,win,gc, x+DIG_W-SEG_T, y+SEG_T,      SEG_T,         half-2*SEG_T ); break;
    case 2: XFillRectangle(dpy,win,gc, x+DIG_W-SEG_T, y+half+SEG_T, SEG_T,         half-2*SEG_T ); break;
    case 3: XFillRectangle(dpy,win,gc, x+SEG_T,       y+DIG_H-SEG_T,DIG_W-2*SEG_T, SEG_T        ); break;
    case 4: XFillRectangle(dpy,win,gc, x,             y+half+SEG_T, SEG_T,         half-2*SEG_T ); break;
    case 5: XFillRectangle(dpy,win,gc, x,             y+SEG_T,      SEG_T,         half-2*SEG_T ); break;
    case 6: XFillRectangle(dpy,win,gc, x+SEG_T,       y+half,       DIG_W-2*SEG_T, SEG_T        ); break;
    }
}

static void draw_digit(int x, int y, int digit)
{
    for (int s = 0; s < 7; s++)
        draw_segment(x, y, s, (DIGIT_SEGMENTS[digit] >> s) & 1);
}

static void draw_colon(int x, int y)
{
    XSetForeground(dpy, gc, col_on);
    int r = SEG_T - 1;
    XFillArc(dpy, win, gc, x+COLON_W/2-r, y+DIG_H/3-r,   2*r, 2*r, 0, 360*64);
    XFillArc(dpy, win, gc, x+COLON_W/2-r, y+2*DIG_H/3-r, 2*r, 2*r, 0, 360*64);
}

/*
 * Functii ajutatoare pentru widget-uri
*/
static int text_width(const char *s)
{
    return font ? XTextWidth(font, s, strlen(s)) : (int)(strlen(s) * 7);
}

static void draw_text(Drawable d, int x, int y, const char *s, unsigned long color)
{
    XSetForeground(dpy, gc, color);
    XDrawString(dpy, d, gc, x, y, s, strlen(s));
}

static void draw_button(Drawable d, int x, int y, int w, int h,
                        const char *label, unsigned long bg)
{
    XSetForeground(dpy, gc, bg);
    XFillRectangle(dpy, d, gc, x, y, w, h);
    draw_text(d, x + (w - text_width(label)) / 2, y + h - 7, label, col_bg);
}

static void draw_field(int x, int y, int w,
                       const char *label, const char *value, int active)
{
    draw_text(win, x, y, label, col_fg);
    int box_y = y + 18;
    XSetForeground(dpy, gc, col_input);
    XFillRectangle(dpy, win, gc, x, box_y, w, 22);
    XSetForeground(dpy, gc, active ? col_on : col_off);
    XDrawRectangle(dpy, win, gc, x, box_y, w, 22);
    draw_text(win, x + 4, box_y + 16, value, col_fg);
    /* cursor text */
    if (active) {
        XSetForeground(dpy, gc, col_on);
        XFillRectangle(dpy, win, gc, x + 4 + text_width(value), box_y + 3, 2, 15);
    }
}

/*
 * Fereastra de alerta
*/
static void close_alert(void)
{
    if (alert_win != None) {
        XDestroyWindow(dpy, alert_win);
        alert_win = None;
        XFlush(dpy);
    }
}

static void redraw_alert(void)
{
    XSetForeground(dpy, gc, col_bg);
    XFillRectangle(dpy, alert_win, gc, 0, 0, ALERT_W, ALERT_H);

    /* header rosu */
    XSetForeground(dpy, gc, col_red);
    XFillRectangle(dpy, alert_win, gc, 0, 0, ALERT_W, 30);
    draw_text(alert_win, 10, 22, "! ALARMA !", col_fg);

    /* mesaj pe maxim 3 randuri de 50 caractere */
    int len = strlen(alert_msg), pos = 0, line = 0;
    while (pos < len && line < 3) {
        int n = (len - pos > 50) ? 50 : len - pos;
        draw_text(alert_win, 10, 50 + line * 20, alert_msg + pos, col_on);
        pos += n;
        line++;
    }

    draw_button(alert_win, ALERT_BTN_X, ALERT_BTN_Y, 80, 24, "Inchide", col_btn);
    XFlush(dpy);
}

static void open_alert(const char *msg)
{
    close_alert(); /* inchide automat fereastra anterioara daca exista */
    strncpy(alert_msg, msg, MAX_MSG - 1);

    alert_win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                                     220, 220, ALERT_W, ALERT_H, 2, col_on, col_bg);
    XStoreName(dpy, alert_win, "Alarma!");
    XSelectInput(dpy, alert_win, ExposureMask | ButtonPressMask);
    XSetWMProtocols(dpy, alert_win, &wm_delete, 1);
    XMapRaised(dpy, alert_win);
    XFlush(dpy);
}

/*
 * Redesenare fereastra principala
*/
static void redraw(void)
{
    XSetForeground(dpy, gc, col_bg);
    XFillRectangle(dpy, win, gc, 0, 0, WIN_W, WIN_H);

    /* ── ceas 7-segmente ── */
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    int digits[6] = {
        lt->tm_hour / 10, lt->tm_hour % 10,
        lt->tm_min  / 10, lt->tm_min  % 10,
        lt->tm_sec  / 10, lt->tm_sec  % 10,
    };

    int clock_total_w = 6 * (DIG_W + DIG_GAP) + 2 * (COLON_W + DIG_GAP);
    int cx = (WIN_W - clock_total_w) / 2; /* centrat orizontal */

    for (int i = 0; i < 6; i++) {
        draw_digit(cx, CLOCK_Y, digits[i]);
        cx += DIG_W + DIG_GAP;
        if (i == 1 || i == 3) { /* colon dupa HH si dupa MM */
            draw_colon(cx, CLOCK_Y);
            cx += COLON_W + DIG_GAP;
        }
    }

    /* ── lista de evenimente ── */
    draw_text(win, 20, LIST_Y, "Evenimente planificate:", col_fg);
    XSetForeground(dpy, gc, col_off);
    XDrawRectangle(dpy, win, gc, 20, LIST_Y + 6, WIN_W - 40, LIST_H);

    if (num_events == 0) {
        draw_text(win, 30, LIST_Y + 26, "(niciun eveniment)", col_off);
    } else {
        for (int i = 0; i < num_events && i < LIST_ROWS; i++) {
            char buf[320];
            struct tm *et = localtime(&events[i].time);
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d  %s",
                     et->tm_hour, et->tm_min, et->tm_sec, events[i].msg);
            draw_text(win, 30, LIST_Y + 8 + (i + 1) * 20, buf, col_fg);
        }
    }

    /* ── campuri input ── */
    draw_field(20,  INPUT_Y, 120,       "Ora (HH:MM:SS):", field_time, focus == 0);
    draw_field(160, INPUT_Y, WIN_W-180, "Mesaj:",          field_msg,  focus == 1);

    /* ── butoane ── */
    draw_button(win, 20,          BTN_Y, 100, 26, "Adauga", col_btn);
    draw_button(win, WIN_W - 100, BTN_Y, 80,  26, "Iesire", col_red);

    XFlush(dpy);
}

/*
 * Logica input tastatura
*/
static void handle_key(char *buf, int maxlen, KeySym key, const char *str)
{
    int len = strlen(buf);
    if      (key == XK_BackSpace)            { if (len > 0) buf[len-1] = '\0'; }
    else if (key == XK_Delete)               { buf[0] = '\0'; }
    else if (str[0] >= 32 && len < maxlen-1) { buf[len] = str[0]; buf[len+1] = '\0'; }
}

static int parse_time(const char *s, time_t *out)
{
    int h, m, sec;
    if (sscanf(s, "%d:%d:%d", &h, &m, &sec) != 3) return 0;
    if (h < 0 || h > 23 || m < 0 || m > 59 || sec < 0 || sec > 59) return 0;

    time_t now = time(NULL);
    struct tm et = *localtime(&now);
    et.tm_hour = h; et.tm_min = m; et.tm_sec = sec; et.tm_isdst = -1;
    time_t t = mktime(&et);
    if (t <= now) t += 86400; /* daca ora e in trecut, programeaza pentru maine */
    *out = t;
    return 1;
}

static void add_event(void)
{
    time_t t;
    if (!parse_time(field_time, &t) || field_msg[0] == '\0') return;
    event_add(t, field_msg);
    field_time[0] = field_msg[0] = '\0';
    focus = 0;
}

/*
 * main
*/
int main(void)
{
    dpy = XOpenDisplay(NULL);
    if (!dpy) { fputs("Nu pot deschide display-ul X\n", stderr); return 1; }

    /* culori */
    col_bg    = BlackPixel(dpy, DefaultScreen(dpy));
    col_fg    = WhitePixel(dpy, DefaultScreen(dpy));
    col_on    = alloc_color("#00FF41"); /* verde aprins – segmente active   */
    col_off   = alloc_color("#003808"); /* verde inchis – segmente inactive */
    col_red   = alloc_color("red2");
    col_input = alloc_color("#101020"); /* fond camp input */
    col_btn   = alloc_color("#1A4A8A"); /* fond buton      */

    /* fereastra principala */
    win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                               100, 100, WIN_W, WIN_H, 2, col_on, col_bg);
    XStoreName(dpy, win, "Ceas cu alarme");
    XSelectInput(dpy, win, ExposureMask | KeyPressMask | ButtonPressMask);

    wm_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, win, &wm_delete, 1);

    gc   = XCreateGC(dpy, win, 0, NULL);
    font = XLoadQueryFont(dpy, "-*-helvetica-medium-r-*-*-14-*-*-*-*-*-*-*");
    if (!font) font = XLoadQueryFont(dpy, "fixed");
    if (font)  XSetFont(dpy, gc, font->fid);

    XMapWindow(dpy, win);

    int xfd     = ConnectionNumber(dpy);
    int running = 1;

    while (running) {
        /* asteapta maxim 500ms un eveniment X */
        fd_set fds; FD_ZERO(&fds); FD_SET(xfd, &fds);
        struct timeval tv = { 0, 500000 };
        select(xfd + 1, &fds, NULL, NULL, &tv);

        /* verifica daca a sosit momentul unui eveniment planificat */
        time_t now = time(NULL);
        while (num_events > 0 && events[0].time <= now) {
            open_alert(events[0].msg);
            event_remove(0);
        }

        /* proceseaza toate evenimentele X in asteptare */
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);

            /* ── eveniment pentru fereastra de alerta ── */
            if (alert_win != None && e.xany.window == alert_win) {
                if (e.type == Expose && e.xexpose.count == 0)
                    redraw_alert();
                else if (e.type == ButtonPress) {
                    int x = e.xbutton.x, y = e.xbutton.y;
                    if (x >= ALERT_BTN_X && x <= ALERT_BTN_X + 80 &&
                        y >= ALERT_BTN_Y && y <= ALERT_BTN_Y + 24)
                        close_alert();
                } else if (e.type == ClientMessage &&
                           (Atom)e.xclient.data.l[0] == wm_delete)
                    close_alert();
                continue;
            }

            /* ── eveniment pentru fereastra principala ── */
            switch (e.type) {

            case Expose:
                if (e.xexpose.count == 0) redraw();
                break;

            case ButtonPress: {
                int x = e.xbutton.x, y = e.xbutton.y;
                /* selectie camp input prin click */
                if (y >= INPUT_Y + 18 && y <= INPUT_Y + 40) {
                    if      (x >= 20  && x <= 140)        focus = 0;
                    else if (x >= 160 && x <= WIN_W - 20)  focus = 1;
                }
                if (x >= 20 && x <= 120 && y >= BTN_Y && y <= BTN_Y + 26) add_event();
                if (x >= WIN_W - 100   && y >= BTN_Y && y <= BTN_Y + 26)  running = 0;
                redraw();
                break;
            }

            case KeyPress: {
                char key_str[8] = {0};
                KeySym key;
                XLookupString(&e.xkey, key_str, 7, &key, NULL);
                if      (key == XK_Tab)    focus ^= 1;
                else if (key == XK_Return) { if (focus == 0) focus = 1; else add_event(); }
                else if (key == XK_Escape) running = 0;
                else if (focus == 0)       handle_key(field_time, sizeof(field_time), key, key_str);
                else                       handle_key(field_msg,  sizeof(field_msg),  key, key_str);
                redraw();
                break;
            }

            case ClientMessage:
                if ((Atom)e.xclient.data.l[0] == wm_delete) running = 0;
                break;
            }
        }

        redraw(); /* actualizeaza ceasul la fiecare ≤500ms */
    }

    close_alert();
    if (font) XFreeFont(dpy, font);
    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}
