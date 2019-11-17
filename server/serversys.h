#ifndef __SERVERSYS_H_INC
#define __SERVERSYS_H_INC
char *timeStr(void);

#define LogEvent(text) \
        { \
        char *__M_tm_loc_var = timeStr(); \
        fprintf(logFileG, "[%s] %s\n", __M_tm_loc_var, (text));\
        free(__M_tm_loc_var); \
        }

#define LogError(text) \
        { \
        char *__M_tm_loc_var = timeStr(); \
        fprintf(logFileG, "[%s] %s | line: %d | function: %s | file: %s\n", __M_tm_loc_var, (text), __LINE__, \
             __PRETTY_FUNCTION__, __FILE__); \
        free(__M_tm_loc_var); \
        }

char *gets_s(char *s, int n) 
{
    fflush(stdin);
    int i, k = getchar();
    if (k == EOF)
         return NULL;
    for (i = 0; (k != EOF && k != '\n') && n--; i++) {
        s[i] = k;
        k = getchar();
        if (k == EOF && !feof(stdin))
            return NULL;
    }
    s[i] = '\0';
    return s;    
}

void MsgMelody() {
    Beep(450, 200);
    Beep(750, 200);
}

static const char *isTwoSgn(int x) {
    return x < 10 ? "0" : "";
}

char *makeIP(unsigned int ip_int) {
    char *ip = (char*)malloc(16);
    sprintf(ip, "%u.%u.%u.%u", 
        ((byte*)&ip_int)[0],
        ((byte*)&ip_int)[1],
        ((byte*)&ip_int)[2],
        ((byte*)&ip_int)[3]
    );
    return ip;
}

char *timeStr() {
    char *str = (char*)malloc(64);
    time_t t = time(NULL);
    struct tm *tmCl = localtime(&t);
    sprintf(str, "%s%d.%s%d.%d %s%d:%s%d:%s%d", 
        isTwoSgn(tmCl->tm_mday), tmCl->tm_mday,
        isTwoSgn(tmCl->tm_mon), tmCl->tm_mon + 1, tmCl->tm_year + 1900,
        isTwoSgn(tmCl->tm_hour), tmCl->tm_hour,
        isTwoSgn(tmCl->tm_min), tmCl->tm_min,
        isTwoSgn(tmCl->tm_sec), tmCl->tm_sec);
    return str;
}

#endif

